#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <stdlib.h>
#define DBG_TAG "Home_LED"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "Home_LED.h"
#include "infrared_nec.h"
#include "Rec_Singal.h"

/*******************/
//阿里云变量
/*默认为开启灯*/
uint8_t LivingRoom_LED_State;
uint8_t BedRoom_LED_State;
uint8_t WC_LED_State;
uint8_t FoodRoom_LED_State;
/*******************/


/*宏定义四个灯，客厅，厨房，卫生间，卧室*/
#define LivingRoom_LED GET_PIN(B, 10)
#define BedRoom_LED GET_PIN(B, 11)
#define WC_LED GET_PIN(B, 8)
#define FoodRoom_LED GET_PIN(B, 9)

/*定义四个Flag，用来记录*/
#define Home_LED_ON(pin) rt_pin_write(pin, PIN_LOW)
#define Home_LED_OFF(pin) rt_pin_write(pin, PIN_HIGH)

/*定义一个变量用来接受红外编码值*/
uint8_t NEC_Key;
/*定义一个变量用来接受红外重复值*/
uint8_t NEC_Repeat;
/*用来处理红外遥控的变量*/
static uint8_t Last_State;
static uint8_t Now_State;

/*用来记录灯的状态值*/
static rt_uint32_t LED_State1, LED_State2, LED_State3;
rt_uint32_t Temp_State;
uint8_t LED_State1_Flag;
uint8_t LED_State2_Flag;
uint8_t LED_State3_Flag;
uint8_t LED_State4_Flag;

void LED_Init(void) {
    /*将四个灯配置成推挽输出模式*/
    rt_pin_mode(LivingRoom_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(BedRoom_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(WC_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(FoodRoom_LED, PIN_MODE_OUTPUT);

    /*将四个灯初始化开启*/
    Home_LED_ON(LivingRoom_LED);
    Home_LED_ON(BedRoom_LED);
    Home_LED_ON(WC_LED);
    Home_LED_ON(FoodRoom_LED);
}

void LED_Get_NEC(void) {
    NEC_Key = infrared_Get_Key();
    NEC_Repeat = infrared_Get_Repeat();
}

void LED_thread_Start(void) {
    /*定义初始化句柄*/
    rt_thread_t TidLED = RT_NULL;
    /*对灯初始化句柄进行线程创建*/
    TidLED = rt_thread_create("TidLED", LED_thread_entry, RT_NULL, 1024, 20, 10);
    /*如果该句柄创建成功，那么就开启线程*/
    if (TidLED != RT_NULL) {
        rt_thread_startup(TidLED);
    }
    /*否则输出相应的错误信息*/
    else {
        LOG_D("can not create LED thread!");
    }
}

void LED_thread_entry() {
    /*先将四个灯初始化*/
    LED_Init();
    while (1) {
        LED_Get_NEC();
        /*只有红外和手机端可以控制灯的亮灭*/
        /*红外系统下*/
        Last_State = Now_State;
        Now_State = NEC_Repeat;


        Temp_State = 330;
        if (Temp_State <= 25) {
            Temp_State = 0;
        }
        if (30 <= Temp_State && Temp_State <= 54) {
            Temp_State = 1000;
        }

        if (55 <= Temp_State && Temp_State <= 90) {
            Temp_State = 2000;
        }

        if (100 <= Temp_State && Temp_State <= 127) {
            Temp_State = 3000;
        }

        if (130 <= Temp_State && Temp_State <= 160) {
            Temp_State = 4000;
        }

        //if (Temp_State <= 5) {Temp_State = 0;}
        LED_State1 = LED_State2;
        LED_State2 = LED_State3;
        LED_State3 = Temp_State;

        if (LED_State1 >= 290 && LED_State2 >= 290 && LED_State3 >= 290) {
            LED_State1_Flag = 1;
            LED_State2_Flag = 1;
            LED_State3_Flag = 1;
            LED_State4_Flag = 1;
        }
//            Toggle1_Flag = 1;
//            Toggle2_Flag = 1;
//            Toggle3_Flag = 1;
//            Toggle4_Flag = 1;
//        }
        //rt_kprintf("%d  %d  %d\n", LED_State1, LED_State2, LED_State3);
        if (LED_State1 == 1000 && LED_State2 == 1000 && LED_State3 == 1000 && LED_State1_Flag == 1) {
            LED_State1_Flag = 0;
            LivingRoom_LED_State = 1;
        }
        if (LED_State1 == 0 && LED_State2 == 1000 && LED_State3 == 0 || LED_State1 == 1000 && LED_State2 == 0 && LED_State3 == 0 || LED_State1 == 0 && LED_State2 == 0 && LED_State3 == 1000 || LED_State1 == 1000 && LED_State2 == 1000 && LED_State3 == 0 || LED_State1 == 1000 && LED_State2 == 0 && LED_State3 == 1000 || LED_State1 == 0 && LED_State2 == 1000 && LED_State3 == 1000) {
            LivingRoom_LED_State = 0;
        }

        if (LED_State2_Flag == 1 && LED_State1 == 2000 && LED_State2 == 2000 && LED_State3 == 2000) {
            LED_State2_Flag = 0;
            BedRoom_LED_State = 1;
        }
        if (LED_State1 == 0 && LED_State2 == 2000 && LED_State3 == 0 || LED_State1 == 2000 && LED_State2 == 0 && LED_State3 == 0 || LED_State1 == 0 && LED_State2 == 0 && LED_State3 == 2000 || LED_State1 == 2000 && LED_State2 == 2000 && LED_State3 == 0 || LED_State1 == 2000 && LED_State2 == 0 && LED_State3 == 2000 || LED_State1 == 0 && LED_State2 == 2000 && LED_State3 == 2000) {
            BedRoom_LED_State = 0;
        }

        if (LED_State3_Flag == 1 && LED_State1 == 3000 && LED_State2 == 3000 && LED_State3 == 3000) {
            LED_State3_Flag = 0;
            FoodRoom_LED_State = 1;
        }
        if (LED_State1 == 0 && LED_State2 == 3000 && LED_State3 == 0 || LED_State1 == 3000 && LED_State2 == 0 && LED_State3 == 0 || LED_State1 == 0 && LED_State2 == 0 && LED_State3 == 3000 || LED_State1 == 3000 && LED_State2 == 3000 && LED_State3 == 0 || LED_State1 == 3000 && LED_State2 == 0 && LED_State3 == 3000 || LED_State1 == 0 && LED_State2 == 3000 && LED_State3 == 3000) {
            FoodRoom_LED_State = 0;
        }

        if (LED_State4_Flag == 1 && LED_State1 == 4000 && LED_State2 == 4000 && LED_State3 == 4000) {
            LED_State4_Flag = 0;
            WC_LED_State = 1;
        }
        if (LED_State1 == 0 && LED_State2 == 4000 && LED_State3 == 0 || LED_State1 == 4000 && LED_State2 == 0 && LED_State3 == 0 || LED_State1 == 0 && LED_State2 == 0 && LED_State3 == 4000 || LED_State1 == 4000 && LED_State2 == 4000 && LED_State3 == 0 || LED_State1 == 4000 && LED_State2 == 0 && LED_State3 == 4000 || LED_State1 == 0 && LED_State2 == 4000 && LED_State3 == 4000) {
            WC_LED_State = 0;
        }

        if (NEC_Key == Key_1 && Last_State == 0 && Now_State != 0) {
            /*如果客厅灯关闭状态，那么遥控按下后就开启*/
            if (LivingRoom_LED_State == 0) {
                LivingRoom_LED_State = 1;
            }
            /*否则，就关闭*/
            else if (LivingRoom_LED_State == 1) {
                LivingRoom_LED_State = 0;
            }
        }

        if (NEC_Key == Key_2 && Last_State == 0 && Now_State != 0) {
            /*如果厨房灯关闭状态，那么遥控按下后就开启*/
            if (FoodRoom_LED_State == 0) {
                FoodRoom_LED_State = 1;
            }
            /*否则，就关闭*/
            else if (FoodRoom_LED_State == 1) {
                FoodRoom_LED_State = 0;
            }
        }

        if (NEC_Key == Key_3 && Last_State == 0 && Now_State != 0) {
            /*如果卫生间灯关闭状态，那么遥控按下后就开启*/
            if (WC_LED_State == 0) {
                WC_LED_State = 1;
            }
            /*否则，就关闭*/
            else if (WC_LED_State == 1) {
                WC_LED_State = 0;
            }
        }
        if (NEC_Key == Key_4 && Last_State == 0 && Now_State != 0) {
            /*如果卧室灯关闭状态，那么遥控按下后就开启*/
            if (BedRoom_LED_State == 0) {
                BedRoom_LED_State = 1;
            }
            /*否则，就关闭*/
            else if (BedRoom_LED_State == 1) {
                BedRoom_LED_State = 0;
            }
        }

        /*卧室灯*/
        if (BedRoom_LED_State == 1) {
            Home_LED_ON(BedRoom_LED);
        }
        else if (BedRoom_LED_State == 0) {
            Home_LED_OFF(BedRoom_LED);
        }

        /*客厅灯*/
        if (LivingRoom_LED_State == 1) {
            Home_LED_ON(LivingRoom_LED);
        }
        else if (LivingRoom_LED_State == 0) {
            Home_LED_OFF(LivingRoom_LED);
        }

        /*厕所灯*/
        if (WC_LED_State == 1) {
            Home_LED_ON(WC_LED);
        }
        else if (WC_LED_State == 0) {
            Home_LED_OFF(WC_LED);
        }

        /*厨房灯*/
        if (FoodRoom_LED_State == 1) {
            Home_LED_ON(FoodRoom_LED);
        }
        else if (FoodRoom_LED_State == 0) {
            Home_LED_OFF(FoodRoom_LED);
        }
        rt_thread_mdelay(50);
    }
}
