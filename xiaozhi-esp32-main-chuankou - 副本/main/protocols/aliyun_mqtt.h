#ifndef ALIYUN_MQTT_H
#define ALIYUN_MQTT_H

#include <mqtt.h>
#include <string>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <mutex>

class AliyunMqtt {
public:
    AliyunMqtt();
    ~AliyunMqtt();

    // 连接到阿里云MQTT服务器
    bool Connect(const std::string& product_key, 
                 const std::string& device_name, 
                 const std::string& device_secret,
                 const std::string& region_id = "cn-shanghai");
    
    // 断开连接
    void Disconnect();
    
    // 发布消息到指定主题
    bool PublishMessage(const std::string& topic, const std::string& payload, int qos = 0);
    
    // 发布属性上报消息
    bool PublishProperty(const std::string& properties_json);
    
    // 发布事件消息
    bool PublishEvent(const std::string& event_identifier, const std::string& event_data);
    
    // 订阅主题
    bool Subscribe(const std::string& topic, int qos = 0);
    
    // 设置消息接收回调
    void OnMessage(std::function<void(const std::string& topic, const std::string& payload)> callback);
    
    // 设置连接状态回调
    void OnConnected(std::function<void()> callback);
    void OnDisconnected(std::function<void()> callback);
    
    // 检查连接状态
    bool IsConnected() const;
    
private:
    // 生成阿里云MQTT连接参数
    std::string GenerateClientId(const std::string& product_key, const std::string& device_name);
    std::string GenerateUsername(const std::string& product_key, const std::string& device_name);
    std::string GeneratePassword(const std::string& product_key, const std::string& device_name, const std::string& device_secret);
    std::string GenerateEndpoint(const std::string& product_key, const std::string& region_id);
    
    // 生成主题路径
    std::string GetPropertyPostTopic() const;
    std::string GetEventPostTopic(const std::string& event_identifier) const;
    
    Mqtt* mqtt_;
    std::string product_key_;
    std::string device_name_;
    std::string device_secret_;
    std::string region_id_;
    
    bool connected_;
    std::mutex mutex_;
    
    std::function<void(const std::string&, const std::string&)> on_message_;
    std::function<void()> on_connected_;
    std::function<void()> on_disconnected_;
};

#endif // ALIYUN_MQTT_H