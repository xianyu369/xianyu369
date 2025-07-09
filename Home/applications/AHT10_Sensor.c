#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <stdlib.h>
#define DBG_TAG "AHT10"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "AHT10_Sensor.h"
#include "aht10.h"

/*定义全局变量温湿度*/
int AHT10_Humi = 25, AHT10_Temp = 12;

void AHT10_thread_Start(void) {
    /*定义AHT10线程句柄*/
    rt_thread_t TidAHT10 = RT_NULL;

    /*动态创建AHT10线程*/
    TidAHT10 = rt_thread_create("AHT10", AHT10_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果创建成功，那么就开启线程*/
    if (TidAHT10 != RT_NULL) {
        rt_thread_startup(TidAHT10);
    }
    /*否则输出错误信息*/
    else {
        LOG_D("can not create AHT10 thread!");
    }
}

void AHT10_thread_entry() {
    aht10_device_t AHT10_Dev = aht10_init("i2c1");
    while (1) {
        AHT10_Humi = (int)aht10_read_humidity(AHT10_Dev);
        AHT10_Temp = (int)aht10_read_temperature(AHT10_Dev);
        //rt_kprintf("humidity: %d %% , Temperature: %d\n", AHT10_Humi, AHT10_Temp);
        rt_thread_mdelay(1000);
    }
}

int Get_AHT10_Temp(void) {
    return AHT10_Temp;
}

int Get_AHT10_Humi(void) {
    return AHT10_Humi;
}
