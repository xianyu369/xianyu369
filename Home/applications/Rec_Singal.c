#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define DBG_TAG "SINGAL"
#define DBG_LVL DBG_LOG
#include <board.h>
#include <rtdbg.h>
#include "Rec_Singal.h"

#define ADC_DEV_NAME "adc1"
#define ADC_DEV_CHANNEL 13
#define REF_VOLT 330
#define CONVERT_BITS 12
/*定义ADC设备句柄*/
rt_adc_device_t adc_dev;
rt_uint32_t value;
rt_uint32_t Volt;

void Singal_Init(void) {
    /*初始化设备句柄*/
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);

    if (adc_dev == RT_NULL) {
        rt_kprintf("can not find device %s\n", ADC_DEV_NAME);
        return;
    }
    else {
        rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    }
}

void Singal_thread_start(void) {
    /*线程句柄定义*/
    rt_thread_t TidSingal = RT_NULL;

    TidSingal = rt_thread_create("TidSingal", Singal_thread_entry, RT_NULL, 1024, 20, 10);

    if (TidSingal != RT_NULL) {
        rt_thread_startup(TidSingal);
    }
    else {
        LOG_D("can not create TidSingal thread!");
    }
}
void Singal_thread_entry() {
    /*使用前一定要初始化*/
    Singal_Init();

    while (1) {
        /*读取ADC的值*/
        value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);

        /*转换成电压值*/
        Volt = value * REF_VOLT/((1UL << CONVERT_BITS) - 1);
        /*打印电压值*/
        //rt_kprintf("volt:%d.%02d\n", Volt / 100, Volt % 100);

        rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);

        rt_thread_mdelay(100);
    }
}

rt_uint32_t Get_Volt(void) {
    return Volt;
}
