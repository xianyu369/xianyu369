#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>
#include <string.h>
#define DBG_TAG "UART"
#define DBG_LVL DBG_LOG
#include <serial.h>
#include <rtdbg.h>
#include <AHT10_Sensor.h>

#define UART_NAME "uart2"
rt_device_t u1_dev;
struct rt_semaphore sem;
struct serial_configure u1_configs = RT_SERIAL_CONFIG_DEFAULT;

unsigned int Count_Uart;
extern uint8_t BUZZ_Lock;
extern uint8_t Fan_State;
extern uint8_t Fan_Lock;
extern uint8_t LivingRoom_LED_State;
extern uint8_t BedRoom_LED_State;
extern uint8_t WC_LED_State;
extern uint8_t FoodRoom_LED_State;
extern uint8_t LivingRoom_Window_State;
extern uint8_t BedRoom_Window_State;
extern uint8_t LivingRoom_Window_Flag;
extern uint8_t BedRoom_Window_Flag;

rt_err_t rx_callback(rt_device_t dev, rt_size_t size) {
    rt_sem_release(&sem);
    return RT_EOK;
}

void Uart_Init(void)
{
    rt_err_t ret = 0;
    u1_dev = rt_device_find("uart2");

    if (u1_dev == RT_NULL) {
        LOG_E("can not find the u1_dev...\r\n");
        return;
    }

    ret = rt_device_open(u1_dev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    if (ret < 0) {
        LOG_E("can not open u1_dev...\r\n");
        return;
    }

    rt_device_control(u1_dev, RT_DEVICE_CTRL_CONFIG, (void *)&u1_configs);

    rt_device_set_rx_indicate(u1_dev, rx_callback);
    rt_sem_init(&sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    //rt_device_write(u1_dev, 0, "makabaka\r\n", rt_strlen("makabaka\r\n"));

}

void Uart_Send_thread_entry() {
    //Uart_Init();
    while (1) {
//        float temp, humi;  // 假设这是 AHT10 读取的温度值
//        char send_buf[32], send_buf1[32];  // 存储格式化后的字符串
//
//        temp = (float)Get_AHT10_Temp();
//        humi = (float)Get_AHT10_Humi();
//        /* 格式化字符串，如 "aht10_temp:25.6" */
//        snprintf(send_buf, sizeof(send_buf), "aht10_temp:%.1f\r\n", temp);
//        snprintf(send_buf1, sizeof(send_buf1), "aht10_humi:%.1f\r\n", humi);
//        /* 发送字符串（不包含末尾的 '\0'） */
//        rt_device_write(u1_dev, 0, send_buf, strlen(send_buf));
//        rt_device_write(u1_dev, 0, send_buf1, strlen(send_buf1));
       // rt_device_write(u1_dev, 0, "aht10_temp:", rt_strlen("aht10_temp:"));
//        rt_device_write(u1_dev, 0, &vl, sizeof(vl));
//        rt_device_write(u1_dev, 0, "\r\n", rt_strlen("\r\n"));
        rt_thread_mdelay(600000);
    }
}

void Uart_thread_entry() {

    char buff;
    Uart_Init();
    while (1) {
        //rt_kprintf("OK\r\n");
        while (rt_device_read(u1_dev, 0, &buff, 1) != 1) {
            rt_sem_take(&sem, RT_WAITING_FOREVER);
        }
        //Count_Uart ++;
        //if (Count_Uart == 100) {
            //rt_device_write(u1_dev, 0, "makabaka\r\n", rt_strlen("makabaka\r\n"));
            //rt_device_write(u1_dev, 0, "'aht10_temp':", rt_strlen("'aht10_temp':"));
            //rt_device_write(u1_dev, 0, &vl, sizeof(vl));
            //rt_device_write(u1_dev, 0, "\n", rt_strlen("\n"));
           // Count_Uart = 0;
        //}
        //rt_device_write(u1_dev, 0, "makabaka\r\n", rt_strlen("makabaka\r\n"));
        //rt_device_write(u1_dev, 0, &vl, sizeof(vl));
        if (buff == '6') {
            LivingRoom_LED_State = 1;
        }
        if (buff == '7') {
            LivingRoom_LED_State = 0;
        }
        if (buff == '8') {
            BedRoom_LED_State = 1;
        }
        if (buff == '9') {
            BedRoom_LED_State = 0;
        }
        if (buff == 'a') {
            FoodRoom_LED_State = 1;
        }
        if (buff == 'b') {
            FoodRoom_LED_State = 0;
        }
        if (buff == 'c') {
            WC_LED_State = 1;
        }
        if (buff == 'd') {
            WC_LED_State = 0;
        }
        if (buff == 'e') {
            Fan_State = 0;
        }
        if (buff == 'f') {
            Fan_State = 1;
        }
        if (buff == 'g') {
            BUZZ_Lock = 0;
        }
        if (buff == 'h') {
            BUZZ_Lock = 1;
        }
        if (buff == 'i') {
            LivingRoom_Window_State = 0;
        }
        if (buff == 'j') {
            LivingRoom_Window_State = 1;
        }
        if (buff == 'k') {
            BedRoom_Window_State = 0;
        }
        if (buff == 'l') {
            BedRoom_Window_State = 1;
        }
        rt_thread_mdelay(20);
    }
}

void Uart_Send_thread_start(void) {
    rt_thread_t TidUart_Send = RT_NULL;

    TidUart_Send = rt_thread_create("TidUart_Send", Uart_Send_thread_entry, RT_NULL, 1024, 20, 10);

    if (TidUart_Send != RT_NULL) {
        rt_thread_startup(TidUart_Send);
    }
    else {
        LOG_D("can not create Uart_Send thread!");
        return;
    }
}

void Uart_thread_start(void) {
    rt_thread_t TidUart = RT_NULL;

    TidUart = rt_thread_create("TidUart", Uart_thread_entry, RT_NULL, 1024, 20, 10);

    if (TidUart != RT_NULL) {
        rt_thread_startup(TidUart);
    }
    else {
        LOG_D("can not create Uart thread!");
        return;
    }
}

