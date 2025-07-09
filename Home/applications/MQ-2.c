#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define DBG_TAG "MQ-2"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <board.h>
#include "MQ-2.h"

/*宏定义设备名称以及通道，参考电压，和转换位数*/
#define ADC_DEV_NAME "adc1"
#define ADC_DEV_CHANNEL 3
#define REFER_VOLTAGE 330
#define CONVERT_BITS 12
/*定义adc设备句柄*/
rt_adc_device_t adc_dev;
rt_uint32_t value;
rt_uint32_t Volt;

void MQ2_Init(void) {
    /*使用前必须要初始化*/
    /*先初始化设备句柄*/
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);

    /*如果没有初始化成功,那么就退出函数并打印错误信息*/
    if (adc_dev == RT_NULL) {
        rt_kprintf("can not find device %s\n", ADC_DEV_NAME);
        return;
    }
    /*如果初始化成功，那么就使能ADC通道*/
    else {
        rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    }
}

void MQ2_thread_Start(void) {
    /*创建线程句柄*/
    rt_thread_t TidMQ2 = RT_NULL;
    /*动态创建初始化线程*/
    TidMQ2 = rt_thread_create("TidMQ2", MQ2_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果线程创建成功，那么就开启线程*/
    if (TidMQ2 != RT_NULL) {
        rt_thread_startup(TidMQ2);
    }
    /*否则输出错误信息*/
    else {
        LOG_D("can not create MQ2 thread!");
    }
}

void MQ2_thread_entry() {
    /*使用前必须初始化*/
    MQ2_Init();
    while (1) {

        /*读取ADC的值*/
        value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);

        /*转换成电压值*/
        Volt = value * REFER_VOLTAGE/((1UL << CONVERT_BITS) - 1);
        /*打印电压值*/
        //rt_kprintf("eaka!\n");
       // rt_kprintf("MQ2_volt:%d\n", Volt);

        rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);
        rt_thread_mdelay(100);
    }
}

rt_uint32_t Get_MQ2_Volt(void) {
    return Volt;
}
