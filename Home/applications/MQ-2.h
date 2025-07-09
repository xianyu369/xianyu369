#ifndef APPLICATIONS_MQ_2_H_
#define APPLICATIONS_MQ_2_H_

/*MQ2adc设备初始化函数*/
void MQ2_Init(void);
/*MQ2烟雾报警器线程开始函数*/
void MQ2_thread_Start(void);
/*MQ2线程入口函数*/
void MQ2_thread_entry();
/*MQ2获取值函数*/
rt_uint32_t Get_MQ2_Volt(void);

#endif
