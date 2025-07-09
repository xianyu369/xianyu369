#ifndef APPLICATIONS_AHT10_SENSOR_H_
#define APPLICATIONS_AHT10_SENSOR_H_

/*AHT10线程入口函数*/
void AHT10_thread_entry();
/*AHT10线程开始函数*/
void AHT10_thread_Start(void);
/*获取AHT10温度函数*/
int Get_AHT10_Temp(void);
/*获取AHT10湿度参数*/
int Get_AHT10_Humi(void);

#endif
