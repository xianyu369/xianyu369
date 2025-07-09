#include <rtdevice.h>
#include <rtthread.h>
#include "drv_common.h"
#include <stdlib.h>
#define DBG_TAG "servo"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "Window.h"
#include "infrared_nec.h"
#include "Rec_Singal.h"

int Read_Water = 230;
/*************/
/*定义变量用来表示现在窗户是否开启, 初始状态为关闭*/
uint8_t LivingRoom_Window_State;
uint8_t BedRoom_Window_State;
uint8_t LivingRoom_Window_Flag;
uint8_t BedRoom_Window_Flag;
/*************/

/*定义静态变量用来做红外接收状态转换*/
static uint8_t Last_State;
static uint8_t Now_State;
/*定义红外接收键码值以及重复值*/
uint8_t NEC_Key;
uint8_t NEC_Repeat;

rt_uint32_t Window_Temp_Volt;
static rt_uint32_t Window_State1, Window_State2, Window_State3;
uint8_t Window_State1_Flag;
uint8_t Window_State2_Flag;
uint8_t Read_Flag = 1;

/*定义结构体数组*/
struct Window_Servo WS[Servo_Num] = {
        {"pwm4", 1, RT_NULL},
        {"pwm4", 2, RT_NULL}
};

/*设备初始化*/
int Window_Init(void) {
    int i;
    for (i = 0; i < Servo_Num; i ++ ) {
        WS[i].pwmDev = (struct rt_device_pwm *)rt_device_find(WS[i].DeviceName);
        if (WS[i].pwmDev == RT_NULL) {
            LOG_E("can not find this pwmDevice (Servo)!");
            return -1;
        }
    }
    return RT_EOK;
}

int Window_Close(enum Window_ID ID) {
    uint32_t pulse;
    uint32_t period;
    if (ID == LivingRoom_ID) {
            /*当脉宽长度为1.5ms，总长度为20ms时，为90度，即开窗*/
            pulse = 1500000;
            /*定义PWM周期*/
            period = 20000000;
        }

        if (ID == BedRoom_ID) {
            /*当脉宽长度为0.5ms，总长度为20ms时，为90度，即开窗*/
                    pulse = 500000;
                    /*定义PWM周期*/
                    period = 20000000;
        }

    /*如果ID名大于了最大容量，说明设备名字错误，此时默认第一个设备名字*/
    if (ID >= Servo_Num) {
        ID = 0;
    }

    if (WS[ID].pwmDev) {
        /*设置pwm参数*/
        rt_pwm_set(WS[ID].pwmDev, WS[ID].pwmChanel, period, pulse);
        /*使能pwm设备*/
        rt_pwm_enable(WS[ID].pwmDev, WS[ID].pwmChanel);
        return RT_EOK;
    }
    else {
        /*否则输出对应的错误信息*/
        LOG_E("Servo not init!");
        return RT_ERROR;
    }
}

void Window_Get_NEC(void) {
    /*获取红外两个值*/
    NEC_Key = infrared_Get_Key();
    NEC_Repeat = infrared_Get_Repeat();
}

int Window_Open(enum Window_ID ID) {
    uint32_t pulse;
    uint32_t period;
    if (ID == LivingRoom_ID) {
        /*当脉宽长度为0.5ms，总长度为20ms时，为90度，即开窗*/
        pulse = 500000;
        /*定义PWM周期*/
        period = 20000000;
    }

    if (ID == BedRoom_ID) {
        /*当脉宽长度为1.5ms，总长度为20ms时，为90度，即开窗*/
                pulse = 1500000;
                /*定义PWM周期*/
                period = 20000000;
    }

    /*如果ID名字大于枚举最大容量，那么说明设备名字不对，此时将其默认为第一个设备*/
    if (ID >= Servo_Num) {
        ID = 0;
    }

    if (WS[ID].pwmDev) {
        /*设置pwm设备参数*/
        rt_pwm_set(WS[ID].pwmDev, WS[ID].pwmChanel, period, pulse);
        /*使能pwm设备*/
        rt_pwm_enable(WS[ID].pwmDev, WS[ID].pwmChanel);
        return RT_EOK;
    }
    else {
        /*否则输出对应的错误信息*/
        LOG_E("Servo not init!");
        return RT_ERROR;
    }
}

void Window_thread_Start(void) {
    /*动态定义Window线程句柄*/
    rt_thread_t TidWindow = RT_NULL;
    /*对句柄进行初始化创建线程*/
    TidWindow = rt_thread_create("Window", Window_thread_entry, RT_NULL, 1024, 20, 20);
    /*如果该句柄创建成功，那么开启该线程*/
    if (TidWindow != RT_NULL) {
        rt_thread_startup(TidWindow);
    }
    /*否则，输出错误信息*/
    else {
        LOG_D("can not create Door thread!");
    }
}

void Window_thread_entry() {
    //使用前一定要进行门初始化函数，否则关门开门函数无法使用
    Window_Init();
    Window_Close(BedRoom_ID);
    Window_Close(LivingRoom_ID);
    while (1) {
        /*获取红外键码值*/
        Window_Get_NEC();
        Last_State = Now_State;
        Now_State = NEC_Repeat;

        Window_Temp_Volt = 330;

        if (Window_Temp_Volt <= 25) {
            Window_Temp_Volt = 0;
        }
        if (Window_Temp_Volt >= 175 && Window_Temp_Volt <= 200) {
            Window_Temp_Volt = 5000;
        }

        if (Window_Temp_Volt >= 210 && Window_Temp_Volt <= 240) {
            Window_Temp_Volt = 6000;
        }
        Window_State1 = Window_State2;
        Window_State2 = Window_State3;
        Window_State3 = Window_Temp_Volt;

        //rt_kprintf("%d  %d  %d\n", Window_State1, Window_State2, Window_State3);
        if (Window_State1 >= 290 && Window_State2 >= 290 && Window_State3 >= 290) {
            Window_State1_Flag = 1;
            Window_State2_Flag = 1;
        }

        if (Window_State1_Flag == 1 && Window_State1 == 5000 && Window_State2 == 5000 && Window_State3 == 5000) {
            Window_State1_Flag = 0;
            Window_Open(LivingRoom_ID);
            LivingRoom_Window_State = 1;
        }
        if (Window_State1 == 5000 && Window_State2 == 0 && Window_State3 == 0 || Window_State1 == 0 && Window_State2 == 5000 && Window_State3 == 0 || Window_State1 == 0 && Window_State2 == 0 && Window_State3 == 5000 || Window_State1 == 0 && Window_State2 == 5000 && Window_State3 == 5000 || Window_State1 == 5000 && Window_State2 == 0 && Window_State3 == 5000 || Window_State1 == 5000 && Window_State2 == 5000 && Window_State3 == 0) {
            Window_Close(LivingRoom_ID);
            LivingRoom_Window_State = 0;
        }

        if (Window_State2_Flag == 1 && Window_State1 == 6000 && Window_State2 == 6000 && Window_State3 == 6000) {
            Window_State2_Flag = 0;
            Window_Open(BedRoom_ID);
            BedRoom_Window_State = 1;
        }
        if (Window_State1 == 6000 && Window_State2 == 0 && Window_State3 == 0 || Window_State1 == 0 && Window_State2 == 6000 && Window_State3 == 0 || Window_State1 == 0 && Window_State2 == 0 && Window_State3 == 6000 || Window_State1 == 0 && Window_State2 == 6000 && Window_State3 == 6000 || Window_State1 == 6000 && Window_State2 == 0 && Window_State3 == 6000 || Window_State1 == 6000 && Window_State2 == 6000 && Window_State3 == 0) {
            Window_Close(BedRoom_ID);
            BedRoom_Window_State = 0;
        }

        /*如果按键按下，那么打开或者关闭客厅窗户*/
        if (NEC_Key == Key_6 && Last_State == 0 && Now_State != 0) {
            if (LivingRoom_Window_State == 1) {
                Window_Close(LivingRoom_ID);
                LivingRoom_Window_State = 0;
            }
            else if (LivingRoom_Window_State == 0) {
                Window_Open(LivingRoom_ID);
                LivingRoom_Window_State = 1;
            }
        }
        /*如果按键按下，那么打开或者关闭卧室窗户*/
        if (NEC_Key == Key_7 && Last_State == 0 && Now_State != 0) {
            if (BedRoom_Window_State == 1) {
                Window_Close(BedRoom_ID);
                BedRoom_Window_State = 0;
            }
            else if (BedRoom_Window_State == 0) {
                Window_Open(BedRoom_ID);
                BedRoom_Window_State = 1;
            }
        }
        /*如果卧室标志位为1，那么就打开窗户*/
        if (BedRoom_Window_State == 1 && BedRoom_Window_Flag == 0) {
            Window_Open(BedRoom_ID);
            BedRoom_Window_Flag = 1;
        }
        /*否则关闭*/
        else if (BedRoom_Window_State == 0 && BedRoom_Window_Flag == 1){
            Window_Close(BedRoom_ID);
            BedRoom_Window_Flag = 0;
        }

        /*如果客厅标志位为1，那么就打开窗户*/
        if (LivingRoom_Window_State == 1 && LivingRoom_Window_Flag == 0) {
            Window_Open(LivingRoom_ID);
            LivingRoom_Window_Flag = 1;
        }
        /*否则就关闭窗户*/
        else if (LivingRoom_Window_State == 0 && LivingRoom_Window_Flag == 1){
            Window_Close(LivingRoom_ID);
            LivingRoom_Window_Flag = 0;
        }

       // rt_kprintf("Read_Water:%d\n", Read_Water);
        if (Read_Flag == 1 && Read_Water <= 220) {
            Window_Open(LivingRoom_ID);
            Window_Open(BedRoom_ID);
            Read_Flag = 0;
        }
        else if (Read_Flag == 0 && Read_Water > 220) {
            Window_Close(LivingRoom_ID);
            Window_Close(BedRoom_ID);
            Read_Flag = 1;
        }
        rt_thread_mdelay(50);
    }
}
