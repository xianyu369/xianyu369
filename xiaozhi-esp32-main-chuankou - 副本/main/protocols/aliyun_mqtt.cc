#include "aliyun_mqtt.h"
#include "board.h"
#include <esp_log.h>
#include <mbedtls/md.h>
#include <mbedtls/base64.h>
#include <cJSON.h>
#include <sstream>
#include <iomanip>

#define TAG "AliyunMqtt"

AliyunMqtt::AliyunMqtt() : mqtt_(nullptr), connected_(false) {
}

AliyunMqtt::~AliyunMqtt() {
    if (mqtt_ != nullptr) {
        delete mqtt_;
    }
}

bool AliyunMqtt::Connect(const std::string& product_key, 
                        const std::string& device_name, 
                        const std::string& device_secret,
                        const std::string& region_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (connected_) {
        ESP_LOGW(TAG, "Already connected to Aliyun MQTT");
        return true;
    }
    
    product_key_ = product_key;
    device_name_ = device_name;
    device_secret_ = device_secret;
    region_id_ = region_id;
    
    // 创建MQTT客户端
    if (mqtt_ != nullptr) {
        delete mqtt_;
    }
    mqtt_ = Board::GetInstance().CreateMqtt();
    
    // 生成连接参数
    std::string endpoint = GenerateEndpoint(product_key, region_id);
    std::string client_id = GenerateClientId(product_key, device_name);
    std::string username = GenerateUsername(product_key, device_name);
    std::string password = GeneratePassword(product_key, device_name, device_secret);
    
    ESP_LOGI(TAG, "Connecting to Aliyun MQTT: %s", endpoint.c_str());
    ESP_LOGI(TAG, "Client ID: %s", client_id.c_str());
    ESP_LOGI(TAG, "Username: %s", username.c_str());
    
    // 设置回调函数
    mqtt_->OnConnected([this]() {
        ESP_LOGI(TAG, "Connected to Aliyun MQTT");
        connected_ = true;
        if (on_connected_) {
            on_connected_();
        }
    });
    
    mqtt_->OnDisconnected([this]() {
        ESP_LOGI(TAG, "Disconnected from Aliyun MQTT");
        connected_ = false;
        if (on_disconnected_) {
            on_disconnected_();
        }
    });
    
    mqtt_->OnMessage([this](const std::string& topic, const std::string& payload) {
        ESP_LOGI(TAG, "Received message on topic: %s", topic.c_str());
        if (on_message_) {
            on_message_(topic, payload);
        }
    });
    
    // 连接到阿里云MQTT服务器
    mqtt_->SetKeepAlive(60);
    bool result = mqtt_->Connect(endpoint, 1883, client_id, username, password);
    
    if (!result) {
        ESP_LOGE(TAG, "Failed to connect to Aliyun MQTT");
        connected_ = false;
    }
    
    return result;
}

void AliyunMqtt::Disconnect() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (mqtt_ != nullptr) {
        mqtt_->Disconnect();
    }
    connected_ = false;
}

bool AliyunMqtt::PublishMessage(const std::string& topic, const std::string& payload, int qos) {
    std::lock_guard<std::mutex> lock(mutex_);

    
    ESP_LOGI(TAG, "Publishing to topic: %s", topic.c_str());
    ESP_LOGI(TAG, "Payload: %s", payload.c_str());
    
    return mqtt_->Publish(topic, payload, qos);
}

bool AliyunMqtt::PublishProperty(const std::string& properties_json) {
    std::string topic = GetPropertyPostTopic();
    
    // 构造物模型属性上报消息格式
    cJSON* root = cJSON_CreateObject();
    cJSON* id = cJSON_CreateString("123456"); // 消息ID，可以用时间戳
    cJSON* version = cJSON_CreateString("1.0");
    cJSON* params = cJSON_Parse(properties_json.c_str());
    
    if (params == nullptr) {
        ESP_LOGE(TAG, "Invalid properties JSON");
        cJSON_Delete(root);
        return false;
    }
    
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToObject(root, "version", version);
    cJSON_AddItemToObject(root, "params", params);
    
    char* json_string = cJSON_Print(root);
    std::string payload(json_string);
    
    bool result = PublishMessage(topic, payload);
    
    free(json_string);
    cJSON_Delete(root);
    
    return result;
}

bool AliyunMqtt::PublishEvent(const std::string& event_identifier, const std::string& event_data) {
    std::string topic = GetEventPostTopic(event_identifier);
    
    // 构造物模型事件上报消息格式
    cJSON* root = cJSON_CreateObject();
    cJSON* id = cJSON_CreateString("123456"); // 消息ID
    cJSON* version = cJSON_CreateString("1.0");
    cJSON* params = cJSON_Parse(event_data.c_str());
    
    if (params == nullptr) {
        ESP_LOGE(TAG, "Invalid event data JSON");
        cJSON_Delete(root);
        return false;
    }
    
    cJSON_AddItemToObject(root, "id", id);
    cJSON_AddItemToObject(root, "version", version);
    cJSON_AddItemToObject(root, "params", params);
    
    char* json_string = cJSON_Print(root);
    std::string payload(json_string);
    
    bool result = PublishMessage(topic, payload);
    
    free(json_string);
    cJSON_Delete(root);
    
    return result;
}

bool AliyunMqtt::Subscribe(const std::string& topic, int qos) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!connected_ || mqtt_ == nullptr) {
        ESP_LOGE(TAG, "Not connected to Aliyun MQTT");
        return false;
    }
    
    ESP_LOGI(TAG, "Subscribing to topic: %s", topic.c_str());
    return mqtt_->Subscribe(topic, qos);
}

void AliyunMqtt::OnMessage(std::function<void(const std::string&, const std::string&)> callback) {
    on_message_ = callback;
}

void AliyunMqtt::OnConnected(std::function<void()> callback) {
    on_connected_ = callback;
}

void AliyunMqtt::OnDisconnected(std::function<void()> callback) {
    on_disconnected_ = callback;
}

bool AliyunMqtt::IsConnected() const {
    return connected_;
}

// 私有方法实现
std::string AliyunMqtt::GenerateClientId(const std::string& product_key, const std::string& device_name) {
    return product_key + "." + device_name + "|securemode=3,signmethod=hmacsha1|"; 
}

std::string AliyunMqtt::GenerateUsername(const std::string& product_key, const std::string& device_name) {
    return device_name + "&" + product_key;
}

std::string AliyunMqtt::GeneratePassword(const std::string& product_key, 
                                        const std::string& device_name, 
                                        const std::string& device_secret) {
    // 构造签名内容
    std::string content = "clientId" + product_key + "." + device_name + 
                         "deviceName" + device_name + 
                         "productKey" + product_key;
    
    // 使用HMAC-SHA1计算签名
    unsigned char hmac_result[20];
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);
    
    const mbedtls_md_info_t* md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA1);
    mbedtls_md_setup(&ctx, md_info, 1);
    mbedtls_md_hmac_starts(&ctx, (const unsigned char*)device_secret.c_str(), device_secret.length());
    mbedtls_md_hmac_update(&ctx, (const unsigned char*)content.c_str(), content.length());
    mbedtls_md_hmac_finish(&ctx, hmac_result);
    mbedtls_md_free(&ctx);
    
    // 转换为十六进制字符串
    std::stringstream ss;
    for (int i = 0; i < 20; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hmac_result[i];
    }
    
    return ss.str();
}

std::string AliyunMqtt::GenerateEndpoint(const std::string& product_key, const std::string& region_id) {
    return product_key + ".iot-as-mqtt." + region_id + ".aliyuncs.com";
}

std::string AliyunMqtt::GetPropertyPostTopic() const {
    return "/sys/" + product_key_ + "/" + device_name_ + "/thing/event/property/post";
}

std::string AliyunMqtt::GetEventPostTopic(const std::string& event_identifier) const {
    return "/sys/" + product_key_ + "/" + device_name_ + "/thing/event/" + event_identifier + "/post";
}