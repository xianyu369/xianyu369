#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define DBG_TAG "Mode_Select"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <board.h>
#include "infrared_nec.h"
#include "Mode_Select.h"

/*********************************/
//阿里云参数
uint8_t APP_ON_OFF;
/*********************************/

/*宏定义三个颜色的灯，每个灯表示一种操作系统*/
#define LED_auto GET_PIN(E, 9)
#define LED_nec GET_PIN(E, 8)
#define LED_app GET_PIN(E, 7)
/*宏定义灯的开关，方便后续调用*/
#define LED_ON(pin) rt_pin_write(pin, PIN_LOW)
#define LED_OFF(pin) rt_pin_write(pin, PIN_HIGH)

uint8_t Control_Mode = 1;
/*定义NEC键码和重复值变量*/
uint8_t Mode_NEC_Key;
uint8_t Mode_NEC_Repeat;
/*定义两个变量，用来处理红外，类比于按键思想*/
//static uint8_t Last_State, Now_State;

void Remind_LED_Init(void) {
    /*初始化引脚模式*/
    rt_pin_mode(LED_auto, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_nec, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_app, PIN_MODE_OUTPUT);
    /*初始化将自动控制状态的灯开启*/
    LED_ON(LED_auto);
    LED_ON(LED_nec);
    LED_ON(LED_app);
}

void ModeChange_Get_NEC(void) {
    Mode_NEC_Key = infrared_Get_Key();
    Mode_NEC_Repeat = infrared_Get_Repeat();
}

void ModeChange_thread_Start(void) {
    /*定义线程句柄*/
    rt_thread_t TidModeChange = RT_NULL;

    /*动态创建线程*/
    TidModeChange = rt_thread_create("TidModeChange", ModeChange_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果成功创建线程就开启该线程*/
    if (TidModeChange != RT_NULL) {
        rt_thread_startup(TidModeChange);
    }
    /*否则输出错误信息*/
    else {
        LOG_D("can not create ModeChange thread!");
    }
}

void ModeChange_thread_entry() {
    /*先进行初始化操作*/
    Remind_LED_Init();
    while (1) {
        rt_thread_mdelay(50);
    }
}

uint8_t Get_Mode_State(void) {
    return Control_Mode;
}
