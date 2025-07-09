#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <rtdevice.h>
#include <board.h>
#include "Window.h"
#include "Home_LED.h"
#include "Fan.h"
#include "AHT10_Sensor.h"
#include "AP3216C_Sensor.h"
#include "infrared_nec.h"
#include "BUZZ.h"
#include "Trash.h"
#include "SR04.h"
#include "MQ-2.h"
#include "BUZZ.h"
//#include "Home_LOT.h"
#include "Mode_Select.h"
#include "Rec_Singal.h"
#include "Uart.h"

int main(void)
{
    ModeChange_thread_Start();
    Uart_thread_start();
    //Uart_Send_thread_start();
    //MQ2_thread_Start();
    BUZZ_thread_Start();
    infrared_thread_Start();
    LED_thread_Start();
    Singal_thread_start();
    AP3216C_thread_start();
    AHT10_thread_Start();
    Fan_thread_Start();
    Window_thread_Start();
    SR04_thread_Start();
    Trash_thread_Start();
    return RT_EOK;
}
