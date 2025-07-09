#ifndef APPLICATIONS_BUZZ_H_
#define APPLICATIONS_BUZZ_H_

/*蜂鸣器线程入口函数*/
void BUZZ_thread_entry();
/*蜂鸣器线程开始函数*/
void BUZZ_thread_Start(void);
/*蜂鸣器初始化函数*/
void BUZZ_Init(void);
/*获取键码值和重复值函数*/
void BUZZ_Get_NEC(void);

#endif
