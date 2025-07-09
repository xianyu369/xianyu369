#ifndef APPLICATIONS_WINDOW_H_
#define APPLICATIONS_WINDOW_H_

/*定义窗户的ID名称*/
enum Window_ID {
    LivingRoom_ID = 0,
    BedRoom_ID = 1,
    Servo_Num
};

//pwm设备名字长度
#define DEVICE_NAME_LEN 16

/*定义舵机结构体*/
struct Window_Servo {
    char DeviceName[DEVICE_NAME_LEN];//设备名称
    char pwmChanel;//pwm通道名称
    struct rt_device_pwm * pwmDev;//pwm设备句柄
};

/*门线程内部执行代码函数*/
void Window_thread_entry();
/*门线程启动*/
void Window_thread_Start(void);
/*舵机初始化接口，使用前必须初始化*/
int Window_Init(void);
/*定义开门和关门函数*/
int Window_Close(enum Window_ID ID);
int Window_Open(enum Window_ID ID);
/*获取红外键码值和重复值函数*/
void Window_Get_NEC(void);

#endif
