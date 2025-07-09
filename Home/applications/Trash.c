#include <rtthread.h>
#include <rtdevice.h>
#define DBG_TAG "trash"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "Trash.h"
#include "SR04.h"

rt_uint32_t period = 20000000;
rt_uint32_t pulse = 500000;

static unsigned char Last_State, Now_State;
unsigned char Temp_State;
unsigned char Trash_State;

/*定义结构体变量*/
struct Home_Trash HT[Trash_Max_Num] = {
        {"pwm4", 4, RT_NULL}
};

/*定义一个变量，用来接收超声波测量的距离值*/
int Trash_Dis;

int Trash_Init() {
    int i;
    for (i = 0; i < Trash_Max_Num; i ++ ) {
        HT[i].pwmDev = (struct rt_device_pwm *)rt_device_find(HT[i].DeviceName);
        if (HT[i].pwmDev == RT_NULL) {
            LOG_E("can not find this pwmDevice (Trash)!");
            return -1;
        }
    }
    return RT_EOK;
}

void Trash_thread_Start(void) {
    /*创建一个垃圾桶线程句柄*/
    rt_thread_t TidTrash = RT_NULL;
    /*动态创建垃圾桶线程*/
    TidTrash = rt_thread_create("TidTrash", Trash_thread_entry, RT_NULL, 1024, 20, 10);

    /*如果创建成功，那么就开启线程*/
    if (TidTrash != RT_NULL) {
        rt_thread_startup(TidTrash);
    }
    /*否则输出错误信息*/
    else {
        LOG_D("can not create Trash thread!");
    }
}

int Trash_Open(enum Trash_ID ID) {
    /*当脉宽为1.5ms，总长为20ms时，为90度角，即开启*/
    pulse = 1500000;

    /*如果ID名字不在枚举列表里，就默认为第一个设备*/
    if (ID > Trash_Max_Num) {
        ID = 0;
    }

    /*如果该设备存在*/
    if (HT[ID].pwmDev) {
        /*那么就进行pwm设置*/
        rt_pwm_set(HT[ID].pwmDev, HT[ID].pwmChanel, 20000000, pulse);
        /*使能pwm通道*/
        rt_pwm_enable(HT[ID].pwmDev, HT[ID].pwmChanel);
        return RT_EOK;
    }
    else {
        /*否则输出对应的错误信息*/
        LOG_D("Trash not init!");
        return RT_ERROR;
    }
}

int Trash_Close(enum Trash_ID ID) {
    /*当脉宽为0.5ms，总长为20ms时，为0度角，即关闭*/
    pulse = 500000;

    /*如果ID名字不在枚举列表里，就默认为第一个设备*/
    if (ID > Trash_Max_Num) {
        ID = 0;
    }

    /*如果该设备存在*/
    if (HT[ID].pwmDev) {
        /*那么就进行pwm设置*/
        rt_pwm_set(HT[ID].pwmDev, HT[ID].pwmChanel, 20000000, pulse);
        /*使能pwm通道*/
        rt_pwm_enable(HT[ID].pwmDev, HT[ID].pwmChanel);
        return RT_EOK;
    }
    else {
        /*否则输出对应的错误信息*/
        LOG_D("Trash not init!");
        return RT_ERROR;
    }
}

void Trash_thread_entry() {
    /*必须先进行初始化*/
    Trash_Init();
    while (1) {
        /*获取距离值*/
        Trash_Dis = Get_Sr04_Dis();

        if (Trash_Dis <= 100 ) {
            Temp_State = 1;
        }
        else {
            Temp_State = 0;
        }

        Last_State = Now_State;
        Now_State = Temp_State;

        //rt_kprintf("Dis: %d\n", Trash_Dis);
        /*如果实际值小于10cm，那么就打开，否则就关闭*/
        if (Last_State == 0 && Now_State == 1) {
            Trash_Open(Trash_Num);
        }
        if (Last_State == 1 && Now_State == 0) {
            Trash_Close(Trash_Num);
        }
        rt_thread_mdelay(50);
    }
}
