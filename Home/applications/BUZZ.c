#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define DBG_TAG "BUZZ"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <board.h>
#include "BUZZ.h"
#include "MQ-2.h"
#include "AHT10_Sensor.h"
#include "infrared_nec.h"

/*宏定义蜂鸣器的GPIO口*/
#define BUZZ GET_PIN(B, 2)
/*宏定义蜂鸣器开和关*/
#define BUZZ_ON rt_pin_write(BUZZ, PIN_HIGH)
#define BUZZ_OFF rt_pin_write(BUZZ, PIN_LOW)

/****************/
/*定义一个蜂鸣器锁，当开锁状态时，才进行自动检测*/
uint8_t BUZZ_Lock;
/*定义一个蜂鸣器状态，1为开，0为关*/
uint8_t BUZZ_State;

/****************/
/*定义两个变量，用来接收红外键码值和重复值*/
uint8_t NEC_Key;
uint8_t NEC_Repeat;
/*定义两个状态变量，用来处理红外接收*/
static uint8_t Last_State;
static uint8_t Now_State;

/*定义两个变量，一个用来接收温度，一个用来接收烟雾浓度*/
int Temper;
rt_uint32_t MQ2_Volt;

void BUZZ_thread_Start(void) {
    /*创建蜂鸣器线程句柄*/
    rt_thread_t TidBUZZ = RT_NULL;

    /*动态创建蜂鸣器线程*/
    TidBUZZ = rt_thread_create("BUZZ", BUZZ_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果创建蜂鸣器成功，那就开启该线程*/
    if (TidBUZZ != RT_NULL) {
        rt_thread_startup(TidBUZZ);
    }
    /*否则输出错误信息*/
    else {
        LOG_D("can not create BUZZ thread!");
    }
}

void BUZZ_Get_NEC(void) {
    /*获取红外键码值以及重复值*/
    NEC_Key = infrared_Get_Key();
    NEC_Repeat = infrared_Get_Repeat();
}

void BUZZ_Init(void) {
    /*配置蜂鸣器输出模式为推挽输出*/
    rt_pin_mode(BUZZ, PIN_MODE_OUTPUT);
    /*关闭蜂鸣器*/
    BUZZ_OFF;
}

/*之后实现*/
void BUZZ_thread_entry() {
    /*蜂鸣器初始化函数*/
    BUZZ_Init();
    while (1) {
        /*获取红外数据*/
        BUZZ_Get_NEC();
        /*获取温度值和烟雾传感器电压值*/
        Temper = Get_AHT10_Temp();
        MQ2_Volt = Get_MQ2_Volt();
        Last_State = Now_State;
        Now_State = NEC_Repeat;
       // rt_kprintf("Temp:%d\n", Temper);
        /*如果没被锁住的话*/
        if (BUZZ_Lock == 0) {
            /*如果温度或者烟雾超过阈值，那么就响，否则关闭*/
            if (Temper >= 35) {
                BUZZ_State = 1;
            }
            else {
                BUZZ_State = 0;
            }
        }
        else if (BUZZ_Lock == 1) {
            BUZZ_State = 0;
        }

        if (NEC_Key == Key_ON_or_OFF && Last_State == 0 && Now_State != 0) {
            if (BUZZ_Lock == 1) {
                BUZZ_Lock = 0;
            }
            else if (BUZZ_Lock == 0) {
                BUZZ_Lock = 1;
            }
        }

        if (BUZZ_State == 1) {
            //还未修改
            BUZZ_ON;
        }
        else if (BUZZ_State == 0) {
            BUZZ_OFF;
        }
        rt_thread_mdelay(50);
    }
}
