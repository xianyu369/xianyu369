//#include "rtthread.h"
//#include "dev_sign_api.h"
//#include "mqtt_api.h"
//#include "cJSON.h"
//#include <board.h>
//#include<rtdbg.h>
//#include<stdlib.h>
//#include<string.h>
//#include"drv_common.h"
//#include <rtdevice.h>
//
//extern char Params[];
//extern int Read_Water;
//extern int AHT10_Humi;
//extern int AHT10_Temp;
//extern int ap3216c_ps;
//extern uint8_t BUZZ_Lock;
//extern uint8_t Fan_State;
//extern uint8_t Fan_Lock;
//extern uint8_t LivingRoom_LED_State;
//extern uint8_t BedRoom_LED_State;
//extern uint8_t WC_LED_State;
//extern uint8_t FoodRoom_LED_State;
//extern uint8_t LivingRoom_Window_State;
//extern uint8_t BedRoom_Window_State;
//extern uint8_t LivingRoom_Window_Flag;
//extern uint8_t BedRoom_Window_Flag;
//
//char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
//char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
//char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};
//
//void *HAL_Malloc(uint32_t size);
//void HAL_Free(void *ptr);
//void HAL_Printf(const char *fmt, ...);
//int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
//int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
//int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
//uint64_t HAL_UptimeMs(void);
//int HAL_Snprintf(char *str, const int len, const char *fmt, ...);
//
//#define EXAMPLE_TRACE(fmt, ...)  \
//    do { \
//        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
//        HAL_Printf(fmt, ##__VA_ARGS__); \
//        HAL_Printf("%s", "\r\n"); \
//    } while(0)
//
//static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
//{
//    rt_kprintf("salaffd   %s--\n", msg);
//    iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;
//
//    cJSON *json;
//   // cJSON *params;
//    //cJSON *aht10_humi;   cJSON *aht10_temp;   cJSON *AP3216C_PS;
//    cJSON *buzz_lock; cJSON *fan_state;     cJSON *fan_lock;    cJSON *livingroom_led_state;
//    cJSON *bedroom_led_state;    cJSON *wc_led_state;    cJSON *foodroom_led_state;
//    cJSON *livingroom_window_state;    cJSON *bedroom_window_state;     cJSON *read_water;
//    cJSON *params;
//    switch (msg->event_type) {
//        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
//            json = cJSON_Parse(topic_info->payload);//转化为JSon格式
//            rt_kprintf("json-%s-sadada\n", topic_info->payload);
////            aht10_humi = cJSON_GetObjectItemCaseSensitive(json, "aht10_humi");
////            aht10_temp = cJSON_GetObjectItemCaseSensitive(json, "aht10_temp");
////            AP3216C_PS = cJSON_GetObjectItemCaseSensitive(json, "AP3216C_PS");
//            params = cJSON_GetObjectItemCaseSensitive(json, "params");
//            read_water = cJSON_GetObjectItemCaseSensitive(params, "rain_num");
//            buzz_lock = cJSON_GetObjectItemCaseSensitive(json, "buzz_lock");
//            fan_state = cJSON_GetObjectItemCaseSensitive(json, "fan_state");
//            fan_lock = cJSON_GetObjectItemCaseSensitive(json, "fan_lock");
//            livingroom_led_state = cJSON_GetObjectItemCaseSensitive(json, "livingroom_led_state");
//            bedroom_led_state = cJSON_GetObjectItemCaseSensitive(json, "bedroom_led_state");
//            wc_led_state = cJSON_GetObjectItemCaseSensitive(json, "wc_led_state");
//            foodroom_led_state = cJSON_GetObjectItemCaseSensitive(json, "foodroom_led_state");
//            livingroom_window_state = cJSON_GetObjectItemCaseSensitive(json, "livingroom_window_state");
//            bedroom_window_state = cJSON_GetObjectItemCaseSensitive(json, "bedroom_window_state");
////          LED1 = cJSON_GetObjectItemCaseSensitive(params, "LED1");
////            if (cJSON_IsNumber(aht10_humi)) {
////                AHT10_Humi= aht10_humi->valueint;
////                       rt_kprintf("%d\n",AHT10_Humi);                      }
////            if (cJSON_IsNumber(aht10_temp)) {
////                AHT10_Temp= aht10_temp->valueint;
////                       rt_kprintf("%d\n",AHT10_Temp);                      }
////            if (cJSON_IsNumber(AP3216C_PS)) {
////                ap3216c_ps= AP3216C_PS->valueint;
////                       rt_kprintf("%d\n",ap3216c_ps);                      }
//            if (cJSON_IsNumber(read_water)) {
//                Read_Water= read_water->valueint;
//                       rt_kprintf("Water:%d\n",Read_Water);                      }
//            if (cJSON_IsNumber(buzz_lock)) {
//                BUZZ_Lock= buzz_lock->valueint;
//                       rt_kprintf("%d\n",BUZZ_Lock);                      }
//
//            if (cJSON_IsNumber(fan_state)) {
//                Fan_State = fan_state->valueint;
//                       rt_kprintf("%d\n",Fan_State);                      }
//            if (cJSON_IsNumber(fan_lock)) {
//                Fan_Lock = fan_lock->valueint;
//                       rt_kprintf("%d\n",Fan_Lock);                      }
//            if (cJSON_IsNumber(livingroom_led_state)) {
//                LivingRoom_LED_State = livingroom_led_state->valueint;
//                                   rt_kprintf("%d\n",LivingRoom_LED_State);                      }
//            if (cJSON_IsNumber(bedroom_led_state)) {
//                BedRoom_LED_State = bedroom_led_state->valueint;
//                                   rt_kprintf("%d\n",BedRoom_LED_State);                      }
//            if (cJSON_IsNumber(wc_led_state)) {
//                WC_LED_State = wc_led_state->valueint;
//                                   rt_kprintf("%d\n",WC_LED_State);                      }
//            if (cJSON_IsNumber(foodroom_led_state)) {
//                FoodRoom_LED_State = foodroom_led_state->valueint;
//                                   rt_kprintf("%d\n",FoodRoom_LED_State);                      }
//            if (cJSON_IsNumber(livingroom_window_state)) {
//                LivingRoom_Window_State = livingroom_window_state->valueint;
//                                   rt_kprintf("%d\n",LivingRoom_Window_State);                      }
//            if (cJSON_IsNumber(bedroom_window_state)) {
//                BedRoom_Window_State = bedroom_window_state->valueint;
//                                   rt_kprintf("%d\n",BedRoom_Window_State);                      }
//            /* print topic name and topic message */
//            EXAMPLE_TRACE("Message Arrived:");
//            EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
//            EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
//            EXAMPLE_TRACE("\n");
//            break;
//        default:
//            break;
//    }
//}
//
//
//
//static int example_subscribe(void *handle)
//{
//    int res = 0;
//    const char *fmt = "/k19jp8qxcE8/devicetx/user/get";
//    char *topic = NULL;
//    int topic_len = 0;
//
//    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
//    topic = HAL_Malloc(topic_len);
//    if (topic == NULL) {
//        EXAMPLE_TRACE("memory not enough");
//        return -1;
//    }
//    memset(topic, 0, topic_len);
//    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);
//
//    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
//    if (res < 0) {
//        EXAMPLE_TRACE("subscribe failed");
//        HAL_Free(topic);
//        return -1;
//    }
//
//    HAL_Free(topic);
//    return 0;
//}
//
//static int example_publish(void *handle)
//{
//    int             res = 0;
//  //  const char     *fmt = "/k19jp8qxcE8/devicetx/user/pub";
//    const char     *fmt = "/%s/%s/user/pub";
//    char           *topic = NULL;
//    int             topic_len = 0;
//    char           *payload = NULL;
//
//    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
//    topic = HAL_Malloc(topic_len);
//    if (topic == NULL) {
//
//        return -1;
//    }
//    memset(topic, 0, topic_len);
//    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);
//
//    payload=HAL_Malloc(1000);
//    if(payload==NULL){
//        EXAMPLE_TRACE("memory not enough for payload\n");
//        HAL_Free(payload);
//        return -1;
//    }
//    memset(payload,0,strlen(payload));
//
//    //HAL_Snprintf(payload,1000,"{\"aht10_temp\":%d,\"aht10_humi\":%d,\"brightness\":%d,\"BUZZ_Lock\":%d}",AHT10_Temp,12,ap3216c_ps,BUZZ_Lock);
//
//    HAL_Snprintf(payload,1000,"{\"aht10_temp\":%d,\"aht10_humi\":%d,\"ap3216c_ps\":%d,\"buzz_Lock\":%d,\"foodroom_led_state\":%d,\"livingroom_window_state\":%d,\"bedroom_window_state\":%d,\"fan_lock\":%d,\"livingroom_led_state\":%d,\"bedroom_led_state\":%d,\"wc_led_state\":%d,\"fan_state\":%d,\"livingroom_window_flag\":%d,\"bedroom_window_flag\":%d}",AHT10_Temp,AHT10_Humi,ap3216c_ps,BUZZ_Lock,FoodRoom_LED_State,LivingRoom_Window_State,BedRoom_Window_State,Fan_Lock,LivingRoom_LED_State,BedRoom_LED_State,WC_LED_State,Fan_State,LivingRoom_Window_Flag,BedRoom_Window_Flag);
//    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, strlen(payload));
//    if (res < 0) {
//        EXAMPLE_TRACE("publish failed, res = %d", res);
//        HAL_Free(topic);
//        HAL_Free(payload);
//        return -1;
//    }
//
//    HAL_Free(topic);
//    HAL_Free(payload);
//    rt_thread_mdelay(1000);
//    return 0;
//}
//
//
//static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
//{
//    EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
//}
//
///*
// *  NOTE: About demo topic of /${productKey}/${deviceName}/user/get
// *
// *  The demo device has been configured in IoT console (https://iot.console.aliyun.com)
// *  so that its /${productKey}/${deviceName}/user/get can both be subscribed and published
// *
// *  We design this to completely demonstrate publish & subscribe process, in this way
// *  MQTT client can receive original packet sent by itself
// *
// *  For new devices created by yourself, pub/sub privilege also requires being granted
// *  to its /${productKey}/${deviceName}/user/get for successfully running whole example
// */
//
//static int mqtt_example_main(int argc, char *argv[])
//{
//    void                   *pclient = NULL;
//    int                     res = 0;
//    int                     loop_cnt = 0;
//    iotx_mqtt_param_t       mqtt_params;
//
//    HAL_GetProductKey(DEMO_PRODUCT_KEY);
//    HAL_GetDeviceName(DEMO_DEVICE_NAME);
//    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);
//
//    EXAMPLE_TRACE("mqtt example");
//
//    /* Initialize MQTT parameter */
//    /*
//     * Note:
//     *
//     * If you did NOT set value for members of mqtt_params, SDK will use their default values
//     * If you wish to customize some parameter, just un-comment value assigning expressions below
//     *
//     **/
//    memset(&mqtt_params, 0x0, sizeof(mqtt_params));
//
//    /**
//     *
//     *  MQTT connect hostname string
//     *
//     *  MQTT server's hostname can be customized here
//     *
//     *  default value is ${productKey}.iot-as-mqtt.cn-shanghai.aliyuncs.com
//     */
//    /* mqtt_params.host = "something.iot-as-mqtt.cn-shanghai.aliyuncs.com"; */
//
//    /**
//     *
//     *  MQTT connect port number
//     *
//     *  TCP/TLS port which can be 443 or 1883 or 80 or etc, you can customize it here
//     *
//     *  default value is 1883 in TCP case, and 443 in TLS case
//     */
//    /* mqtt_params.port = 1883; */
//
//    /**
//     *
//     * MQTT request timeout interval
//     *
//     * MQTT message request timeout for waiting ACK in MQTT Protocol
//     *
//     * default value is 2000ms.
//     */
//    /* mqtt_params.request_timeout_ms = 2000; */
//
//    /**
//     *
//     * MQTT clean session flag
//     *
//     * If CleanSession is set to 0, the Server MUST resume communications with the Client based on state from
//     * the current Session (as identified by the Client identifier).
//     *
//     * If CleanSession is set to 1, the Client and Server MUST discard any previous Session and Start a new one.
//     *
//     * default value is 0.
//     */
//    /* mqtt_params.clean_session = 0; */
//
//    /**
//     *
//     * MQTT keepAlive interval
//     *
//     * KeepAlive is the maximum time interval that is permitted to elapse between the point at which
//     * the Client finishes transmitting one Control Packet and the point it starts sending the next.
//     *
//     * default value is 60000.
//     */
//    /* mqtt_params.keepalive_interval_ms = 60000; */
//
//    /**
//     *
//     * MQTT write buffer size
//     *
//     * Write buffer is allocated to place upstream MQTT messages, MQTT client will be limitted
//     * to send packet no longer than this to Cloud
//     *
//     * default value is 1024.
//     *
//     */
//    /* mqtt_params.write_buf_size = 1024; */
//
//    /**
//     *
//     * MQTT read buffer size
//     *
//     * Write buffer is allocated to place downstream MQTT messages, MQTT client will be limitted
//     * to recv packet no longer than this from Cloud
//     *
//     * default value is 1024.
//     *
//     */
//    /* mqtt_params.read_buf_size = 1024; */
//
//    /**
//     *
//     * MQTT event callback function
//     *
//     * Event callback function will be called by SDK when it want to notify user what is happening inside itself
//     *
//     * default value is NULL, which means PUB/SUB event won't be exposed.
//     *
//     */
//    mqtt_params.handle_event.h_fp = example_event_handle;
//
//    pclient = IOT_MQTT_Construct(&mqtt_params);
//    if (NULL == pclient) {
//        EXAMPLE_TRACE("MQTT construct failed");
//        return -1;
//    }
//
//    res = example_subscribe(pclient);
//    if (res < 0) {
//        IOT_MQTT_Destroy(&pclient);
//        return -1;
//    }
//
//    while (1) {
//        if (0 == loop_cnt % 20) {
//            example_publish(pclient);
//        }
//
//        IOT_MQTT_Yield(pclient, 200);
//
//        loop_cnt += 1;
//    }
//
//    return 0;
//
//}
//
//#ifdef FINSH_USING_MSH
//MSH_CMD_EXPORT_ALIAS(mqtt_example_main, ali_mqtt_sample, ali coap sample);
//#endif

//INIT_APP_EXPORT(mqtt_example_main);
