#ifndef APPLICATIONS_INFRARED_NEC_H_
#define APPLICATIONS_INFRARED_NEC_H_

/*遥控命令码宏定义表*/
#define Key_ON_or_OFF 0xA2
#define Key_up 0x62
#define Key_left 0x22
#define Key_right 0xC2
#define Key_down 0xA8
#define Key_center 0x02
#define Key_Vol_plus 0x90
#define Key_Vol_sub 0xE0
#define Key_Alientex 0xE2
#define Key_1 0x68
#define Key_2 0x98
#define Key_3 0xB0
#define Key_4 0x30
#define Key_5 0x18
#define Key_6 0x7A
#define Key_7 0x10
#define Key_8 0x38
#define Key_9 0x5A
#define Key_0 0x42
#define Key_Last 0x52

/*红外线程入口函数*/
void infrared_thread_entry();
/*红外线程开始函数*/
void infrared_thread_Start(void);
/*获取红外键码值*/
uint8_t infrared_Get_Key(void);
/*获取红外键码重复值*/
uint8_t infrared_Get_Repeat(void);

#endif
