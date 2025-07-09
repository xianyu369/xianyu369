#ifndef APPLICATIONS_MODE_SELECT_H_
#define APPLICATIONS_MODE_SELECT_H_

/*三套控制系统*/
/*
 * 1.自动控制(蓝色灯)
 * 2.红外遥控控制(绿色灯)
 * 3.手机端控制(红色灯)
*/
/*默认状态为自动控制*/

/*定义模式线程入口函数*/
void ModeChange_thread_entry();
/*定义模式改变线程开始函数*/
void ModeChange_thread_Start(void);
/*定义模式获取红外键码值函数*/
void ModeChange_Get_NEC(void);
/*获取系统目前的状态值*/
uint8_t Get_Mode_State(void);
/*对指示灯初始化操作函数*/
void Remind_LED_Init(void);

#endif
