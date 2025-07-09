#ifndef APPLICATIONS_HOME_LED_H_
#define APPLICATIONS_HOME_LED_H_

/*灯光初始化函数*/
void LED_Init(void);
/*灯光控制线程入口函数*/
void LED_thread_entry();
/*灯光线程开启函数*/
void LED_thread_Start(void);
/*获取红外键码值*/
void LED_Get_NEC(void);

#endif
