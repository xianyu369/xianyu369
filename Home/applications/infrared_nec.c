#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#define DBG_TAG "nec"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include <board.h>
#include "infrared_nec.h"
#include <infrared.h>

/*定义红外接受结构体变量*/
struct infrared_decoder_data infrared_data;
/*定义两个变量，用来接收红外的键码值和重复值*/
uint8_t NEC_Key;
uint8_t NEC_Repeat;

void infrared_thread_Start(void) {
    /*红外线程句柄*/
    rt_thread_t TidNec = RT_NULL;

    /*动态创建线程*/
    TidNec = rt_thread_create("nec", infrared_thread_entry, RT_NULL, 1024, 20, 12);

    /*如果线程创建成功*/
    if (TidNec != RT_NULL) {
        rt_thread_startup(TidNec);
    }
    /*否则输出错误提示信息*/
    else {
        LOG_D("can not create NEC thread!");
    }
}

void infrared_thread_entry() {
    ir_select_decoder("nec");
    while (1) {
        if (infrared_read("nec", &infrared_data) == RT_EOK) {
            NEC_Repeat = infrared_data.data.nec.repeat;
            NEC_Key = infrared_data.data.nec.key;
            /*此行代码用来测试红外键码值*/
            rt_kprintf("Key:0x%x   Repeat:%d\n", NEC_Key, NEC_Repeat);
        }
//        else {
//            NEC_Repeat = 0;
//            NEC_Key = 0;
//        }
        rt_thread_mdelay(10);
    }
}

uint8_t infrared_Get_Key(void) {
    return NEC_Key;
}

uint8_t infrared_Get_Repeat(void) {
    return NEC_Repeat;
}
