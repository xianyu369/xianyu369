#ifndef APPLICATIONS_FAN_H_
#define APPLICATIONS_FAN_H_

/*定义风扇的ID名称*/
enum Fan_ID {
    Fan_Num = 0,
    Max_Num
};

/*设置pwm设备名称长度*/
#define DEVICE_NAME_LEN 16

/*定义风扇结构体*/
struct Home_Fan {
    char DeviceName[DEVICE_NAME_LEN];//设备名称
    char pwmChanel;//pwm通道名称
    struct rt_device_pwm *pwmDev;//pwm设备句柄
};

/*风扇线程函数*/
void Fan_thread_entry();
/*风扇线程启动*/
void Fan_thread_Start(void);
/*风扇pwm设备初始化函数*/
int Fan_Init(void);
/*风扇转速设置，根据后续的烟雾传感器设置转速*/
int Fan_SetSpeed(enum Fan_ID ID, rt_uint32_t pulse);
/*此函数用来获取红外键码值以及重复值*/
void Fan_Get_NEC(void);

#endif
