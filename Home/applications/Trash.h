#ifndef APPLICATIONS_TRASH_H_
#define APPLICATIONS_TRASH_H_

/*定义垃圾桶的名字*/
enum Trash_ID {
    Trash_Num = 0,
    Trash_Max_Num
};

#define DEVICE_NAME_LEN 16

/*定义垃圾桶结构体*/
struct Home_Trash {
    char DeviceName[DEVICE_NAME_LEN];//设备名称
    char pwmChanel;//pwm通道名称
    struct rt_device_pwm *pwmDev;//pwm设备名称
};

/*垃圾桶线程入口函数*/
void Trash_thread_entry();
/*垃圾桶线程开始函数*/
void Trash_thread_Start(void);
/*垃圾桶pwm初始化函数*/
int Trash_Init(void);
/*垃圾桶开关函数*/
int Trash_Open(enum Trash_ID ID);
/*垃圾桶关闭函数*/
int Trash_Close(enum Trash_ID ID);

#endif
