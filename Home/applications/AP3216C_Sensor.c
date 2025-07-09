#include <rtdevice.h>
#include <rtthread.h>
#include "drv_common.h"
#define DBG_TAG "AP3216C"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "ap3216c.h"
#include "AP3216C_Sensor.h"

/*定义全局变量AP3216C光强*/
int ap3216c_ps;

void AP3216C_thread_start(void) {
    /*初始化AP3216C线程句柄*/
    rt_thread_t TidAP3216C = RT_NULL;
    /*动态创建AP3216C线程*/
    TidAP3216C = rt_thread_create("AP3216C", AP3216C_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果动态创建成功，那么就开启该线程*/
    if (TidAP3216C != RT_NULL) {
        rt_thread_startup(TidAP3216C);
    }
    /*如果创建不成功那么就输出错误信息*/
    else {
        LOG_D("can not create AP3216C thread!");
    }
}

void AP3216C_thread_entry() {
    /*初始化ap3216c*/
    ap3216c_device_t AP3216C_Dev = ap3216c_init("i2c2");

    while (1) {
        /*读取ap3216c的光强值*/
        ap3216c_ps = (int)ap3216c_read_ps_data(AP3216C_Dev);
        //rt_kprintf("ps: %d\n", ap3216c_ps);
        rt_thread_mdelay(1000);
    }
}
