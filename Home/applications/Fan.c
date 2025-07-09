#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <stdlib.h>
#define DBG_TAG "Fan"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "Fan.h"
#include "infrared_nec.h"
#include "MQ-2.h"
#include "Rec_Singal.h"

#define AIN1 GET_PIN(B, 12)
#define AIN2 GET_PIN(B, 13)

#define Fan_On rt_pin_write(AIN2, PIN_LOW)
#define Fan_Off rt_pin_write(AIN2, PIN_HIGH)

/*定义结构体数组*/
//struct Home_Fan HF[Max_Num] = {
//    {"pwm4", 3, RT_NULL}
//};

/**************/
//阿里云
/*定义一个变量用来传递参数，0为关闭，1为开*/
uint8_t Fan_State;
/*定义一个风扇锁，当锁定时，自动调节状态下，将不受传感器调控*/
uint8_t Fan_Lock;
/**************/

/*定义变量用来接受红外键码值和重复值*/
uint8_t NEC_Key;
uint8_t NEC_Repeat;
/*定义两个状态，用来做红外重复值状态转换*/
static uint8_t Last_State, Now_State;

/*获取烟雾传感器的电压值*/
rt_uint32_t Get_MQ2_Volt_value;

static rt_uint32_t Fan_State1, Fan_State2, Fan_State3;
rt_uint32_t Fan_Get_Volt;

uint8_t Fan_State_Flag;
uint8_t Fan_Key_Flag;

int Fan_Init(void) {
    rt_pin_mode(AIN1, PIN_MODE_OUTPUT);
    rt_pin_mode(AIN2, PIN_MODE_OUTPUT);
    rt_pin_write(AIN1, PIN_HIGH);
    rt_pin_write(AIN2, PIN_HIGH);
    return RT_EOK;
}

void Fan_thread_Start(void) {
    /*定义动态风扇线程句柄*/
    rt_thread_t TidFan = RT_NULL;
    /*对风扇句柄进行初始化*/
    TidFan = rt_thread_create("Fan", Fan_thread_entry, RT_NULL, 1024, 20, 10);
    /*如果该线程创建成功，那么就启动线程*/
    if (TidFan != RT_NULL) {
        rt_thread_startup(TidFan);
    }
    /*否则就输出错误信息*/
    else {
        LOG_D("can not create Fan thread!");
    }
}

void Fan_Get_NEC(void) {
    NEC_Key = infrared_Get_Key();
    NEC_Repeat = infrared_Get_Repeat();
}

//int Fan_SetSpeed(enum Fan_ID ID, rt_uint32_t Sensor_Pulse) {
//    /*先初始化脉宽*/
//    rt_uint32_t period = 20000000;
//    rt_uint32_t pulse = Sensor_Pulse;
//
//    /*如果ID号大于最大值，那么就默认为第一个设备*/
//    if (ID >= Max_Num) {
//        ID = 0;
//    }
//
//    if (HF[ID].pwmDev) {
//        /*设置风扇pwm参数*/
//        rt_pwm_set(HF[ID].pwmDev, HF[ID].pwmChanel, period, pulse);
//        /*给pwm设备使能*/
//        rt_pwm_enable(HF[ID].pwmDev, HF[ID].pwmChanel);
//        return RT_EOK;
//    }
//    else {
//        /*否则输出对应的错误信息*/
//        LOG_E("Fan init error!");
//        return RT_ERROR;
//    }
//}

void Fan_thread_entry() {
    Fan_Init();
    while (1) {
        /*接收此时的红外键码值和重复值*/
        Fan_Get_NEC();
        /*获取烟雾浓度对应的电压值*/
//        Get_MQ2_Volt_value = Get_MQ2_Volt();

//        if (Get_MQ2_Volt_value > 100) {
//            Fan_State = 1;
//        }
//        else {
//            if (Fan_Key_Flag == 0) {
//                Fan_State = 0;
//            }
//        }
      //  rt_kprintf("%d\n", Get_MQ2_Volt_value);
        Last_State = Now_State;
        Now_State = NEC_Repeat;

        Fan_Get_Volt = 330;
        if (Fan_Get_Volt >= 245 && Fan_Get_Volt <= 280) {
            Fan_Get_Volt = 7000;
        }

        Fan_State1 = Fan_State2;
        Fan_State2 = Fan_State3;
        Fan_State3 = Fan_Get_Volt;

       // rt_kprintf("%d %d %d\n",Fan_State1, Fan_State2, Fan_State3);
        if (Fan_State1 >= 290 && Fan_State2 >= 290 && Fan_State3 >= 290) {
            Fan_State_Flag = 1;
        }
//            Fan_Toggle_Flag = 1;
//        }

        if (Fan_State_Flag == 1 && Fan_State1 == 7000 && Fan_State2 == 7000 && Fan_State3 == 7000) {
            Fan_State_Flag = 0;
            Fan_Lock = 0;
            Fan_State = 1;
        }

        if (Fan_State1 == 0 && Fan_State2 == 7000 && Fan_State3 == 7000 || Fan_State1 == 7000 && Fan_State2 == 0 && Fan_State3 == 7000 || Fan_State1 == 7000 && Fan_State2 == 7000 && Fan_State3 == 0 || Fan_State1 == 7000 && Fan_State2 == 0 && Fan_State3 == 0 || Fan_State1 == 0 && Fan_State2 == 7000 && Fan_State3 == 0 || Fan_State1 == 0 && Fan_State2 == 0 && Fan_State3 == 7000) {
            Fan_Lock = 1;
            Fan_State = 0;
        }

        if (NEC_Key == Key_5 && Last_State == 0 && Now_State != 0) {
            /*如果此时是关着的，那么按下后就开启*/
            if (Fan_State == 0) {
                Fan_State = 1;
            }
            /*如果此时是开着的，按下后就关闭*/
            else if (Fan_State == 1) {
                Fan_State = 0;
            }
        }

        if (NEC_Key == Key_8 && Last_State == 0 && Now_State != 0) {
            Fan_Key_Flag = 0;
        }


        if (Fan_State == 1) {
            Fan_On;
        }
        /*否则关闭排气扇*/
        if (Fan_State == 0) {
            Fan_Off;
        }
        rt_thread_mdelay(50);
    }
}
