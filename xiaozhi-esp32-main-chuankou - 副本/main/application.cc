#include "application.h"
#include "board.h"
#include "display.h"
#include "system_info.h"
#include "ml307_ssl_transport.h"
#include "audio_codec.h"
#include "mqtt_protocol.h"
#include "websocket_protocol.h"
#include "font_awesome_symbols.h"
#include "iot/thing_manager.h"
#include "assets/zh/binary.h"
#include "protocols/aliyun_mqtt.h"

#include <cstring>
#include <esp_log.h>
#include <cJSON.h>
#include <driver/gpio.h>
#include <arpa/inet.h>

#define TAG "Application"


static const char* const STATE_STRINGS[] = {
    "unknown",
    "starting",
    "configuring",
    "idle",
    "connecting",
    "listening",
    "speaking",
    "upgrading",
    "activating",
    "fatal_error",
    "invalid_state"
};

Application::Application() {

    event_group_ = xEventGroupCreate();
    background_task_ = new BackgroundTask(4096 * 8);
    
    // 初始化串口
    InitUart();
}

Application::~Application() {
    if (background_task_ != nullptr) {
        delete background_task_;
    }
    vEventGroupDelete(event_group_);
}

void Application::CheckNewVersion() {
    auto& board = Board::GetInstance();
    auto display = board.GetDisplay();
    // Check if there is a new firmware version available
    ota_.SetPostData(board.GetJson());

    while (true) {
        if (ota_.CheckVersion()) {
            if (ota_.HasNewVersion()) {
                Alert("OTA 升级", "正在升级系统", "happy", std::string(p3_upgrade_start, p3_upgrade_end - p3_upgrade_start));
                // Wait for the chat state to be idle
                do {
                    vTaskDelay(pdMS_TO_TICKS(3000));
                } while (GetDeviceState() != kDeviceStateIdle);

                // Use main task to do the upgrade, not cancelable
                Schedule([this, &board, display]() {
                    SetDeviceState(kDeviceStateUpgrading);
                    
                    display->SetIcon(FONT_AWESOME_DOWNLOAD);
                    display->SetChatMessage("system", "新版本 " + ota_.GetFirmwareVersion());

                    board.SetPowerSaveMode(false);
#if CONFIG_USE_AUDIO_PROCESSING
                    wake_word_detect_.StopDetection();
#endif
                    // 预先关闭音频输出，避免升级过程有音频操作
                    auto codec = board.GetAudioCodec();
                    codec->EnableInput(false);
                    codec->EnableOutput(false);
                    {
                        std::lock_guard<std::mutex> lock(mutex_);
                        audio_decode_queue_.clear();
                    }
                    background_task_->WaitForCompletion();
                    delete background_task_;
                    background_task_ = nullptr;
                    vTaskDelay(pdMS_TO_TICKS(1000));

                    ota_.StartUpgrade([display](int progress, size_t speed) {
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%d%% %zuKB/s", progress, speed / 1024);
                        display->SetChatMessage("system", buffer);
                    });

                    // If upgrade success, the device will reboot and never reach here
                    display->SetStatus("更新失败");
                    ESP_LOGI(TAG, "Firmware upgrade failed...");
                    vTaskDelay(pdMS_TO_TICKS(3000));
                    Reboot();
                });
            } else {
                ota_.MarkCurrentVersionValid();
                display->ShowNotification("版本 " + ota_.GetCurrentVersion());
            
                // Check if the activation code is valid
                if (ota_.HasActivationCode()) {
                    SetDeviceState(kDeviceStateActivating);
                    ShowActivationCode();
                } else {
                    SetDeviceState(kDeviceStateIdle);
                    display->SetChatMessage("system", "");
                    return;
                }
            }
        }

        // Check again in 60 seconds
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}

void Application::ShowActivationCode() {
    auto& message = ota_.GetActivationMessage();
    auto& code = ota_.GetActivationCode();

    struct digit_sound {
        char digit;
        const char* sound_data_start;
        const char* sound_data_end;
    };
    digit_sound digit_sounds[] = {
        {'0', p3_0_start, p3_0_end},
        {'1', p3_1_start, p3_1_end},
        {'2', p3_2_start, p3_2_end},
        {'3', p3_3_start, p3_3_end},
        {'4', p3_4_start, p3_4_end},
        {'5', p3_5_start, p3_5_end},
        {'6', p3_6_start, p3_6_end},
        {'7', p3_7_start, p3_7_end},
        {'8', p3_8_start, p3_8_end},
        {'9', p3_9_start, p3_9_end},
    };
    std::string sound = std::string(p3_activation_start, p3_activation_end - p3_activation_start);
    for (const auto& digit : code) {
        auto it = std::find_if(digit_sounds, digit_sounds + sizeof(digit_sounds) / sizeof(digit_sound),
            [digit](const digit_sound& ds) { return ds.digit == digit; });
        if (it != digit_sounds + sizeof(digit_sounds) / sizeof(digit_sound)) {
            sound += std::string(it->sound_data_start, it->sound_data_end - it->sound_data_start);
        }
    }
    Alert("激活设备", message, "happy", sound);
}

void Application::Alert(const std::string& status, const std::string& message, const std::string& emotion, const std::string& sound) {
    ESP_LOGW(TAG, "Alert %s: %s [%s]", status.c_str(), message.c_str(), emotion.c_str());
    auto display = Board::GetInstance().GetDisplay();
    display->SetStatus(status);
    display->SetEmotion(emotion);
    display->SetChatMessage("system", message);
    if (!sound.empty()) {
        PlayLocalFile(sound.data(), sound.size());
    }
}

void Application::PlayLocalFile(const char* data, size_t size) {
    ESP_LOGI(TAG, "PlayLocalFile: %zu bytes", size);
    auto codec = Board::GetInstance().GetAudioCodec();
    codec->EnableOutput(true);
    SetDecodeSampleRate(16000);
    for (const char* p = data; p < data + size; ) {
        auto p3 = (BinaryProtocol3*)p;
        p += sizeof(BinaryProtocol3);

        auto payload_size = ntohs(p3->payload_size);
        std::vector<uint8_t> opus;
        opus.resize(payload_size);
        memcpy(opus.data(), p3->payload, payload_size);
        p += payload_size;

        std::lock_guard<std::mutex> lock(mutex_);
        audio_decode_queue_.emplace_back(std::move(opus));
    }
}

void Application::ToggleChatState() {
    Schedule([this]() {
        if (device_state_ == kDeviceStateActivating) {
            Reboot();
            return;
        }

        if (!protocol_) {
            ESP_LOGE(TAG, "Protocol not initialized");
            return;
        }

        if (device_state_ == kDeviceStateIdle) {
            SetDeviceState(kDeviceStateConnecting);
            if (!protocol_->OpenAudioChannel()) {
                Alert("ERROR", "无法建立音频通道");
                SetDeviceState(kDeviceStateIdle);
                return;
            }

            keep_listening_ = true;
            protocol_->SendStartListening(kListeningModeAutoStop);
            SetDeviceState(kDeviceStateListening);
        } else if (device_state_ == kDeviceStateSpeaking) {
            AbortSpeaking(kAbortReasonNone);
        } else if (device_state_ == kDeviceStateListening) {
            protocol_->CloseAudioChannel();
        }
    });
}

void Application::StartListening() {
    Schedule([this]() {
        if (device_state_ == kDeviceStateActivating) {
            Reboot();
            return;
        }

        if (!protocol_) {
            ESP_LOGE(TAG, "Protocol not initialized");
            return;
        }
        
        keep_listening_ = false;
        if (device_state_ == kDeviceStateIdle) {
            if (!protocol_->IsAudioChannelOpened()) {
                SetDeviceState(kDeviceStateConnecting);
                if (!protocol_->OpenAudioChannel()) {
                    SetDeviceState(kDeviceStateIdle);
                    Alert("ERROR", "无法建立音频通道");
                    return;
                }
            }
            protocol_->SendStartListening(kListeningModeManualStop);
            SetDeviceState(kDeviceStateListening);
        } else if (device_state_ == kDeviceStateSpeaking) {
            AbortSpeaking(kAbortReasonNone);
            protocol_->SendStartListening(kListeningModeManualStop);
            // FIXME: Wait for the speaker to empty the buffer
            vTaskDelay(pdMS_TO_TICKS(120));
            SetDeviceState(kDeviceStateListening);
        }
    });
}

void Application::StopListening() {
    Schedule([this]() {
        if (device_state_ == kDeviceStateListening) {
            protocol_->SendStopListening();
            SetDeviceState(kDeviceStateIdle);
        }
    });
}

void Application::Start() {
    auto& board = Board::GetInstance();
    SetDeviceState(kDeviceStateStarting);

    /* Setup the display */
    auto display = board.GetDisplay();

    /* Setup the audio codec */
    auto codec = board.GetAudioCodec();
    opus_decode_sample_rate_ = codec->output_sample_rate();
    opus_decoder_ = std::make_unique<OpusDecoderWrapper>(opus_decode_sample_rate_, 1);
    opus_encoder_ = std::make_unique<OpusEncoderWrapper>(16000, 1, OPUS_FRAME_DURATION_MS);
    // For ML307 boards, we use complexity 5 to save bandwidth
    // For other boards, we use complexity 3 to save CPU
    if (board.GetBoardType() == "ml307") {
        ESP_LOGI(TAG, "ML307 board detected, setting opus encoder complexity to 5");
        opus_encoder_->SetComplexity(5);
    } else {
        ESP_LOGI(TAG, "WiFi board detected, setting opus encoder complexity to 3");
        opus_encoder_->SetComplexity(3);
    }

    if (codec->input_sample_rate() != 16000) {
        input_resampler_.Configure(codec->input_sample_rate(), 16000);
        reference_resampler_.Configure(codec->input_sample_rate(), 16000);
    }
    codec->OnInputReady([this, codec]() {
        BaseType_t higher_priority_task_woken = pdFALSE;
        xEventGroupSetBitsFromISR(event_group_, AUDIO_INPUT_READY_EVENT, &higher_priority_task_woken);
        return higher_priority_task_woken == pdTRUE;
    });
    codec->OnOutputReady([this]() {
        BaseType_t higher_priority_task_woken = pdFALSE;
        xEventGroupSetBitsFromISR(event_group_, AUDIO_OUTPUT_READY_EVENT, &higher_priority_task_woken);
        return higher_priority_task_woken == pdTRUE;
    });
    
    codec->Start();

    /* Start the main loop */
    xTaskCreate([](void* arg) {
        Application* app = (Application*)arg;
        app->MainLoop();
        vTaskDelete(NULL);
    }, "main_loop", 4096 * 2, this, 2, nullptr);

    /* Wait for the network to be ready */
    board.StartNetwork();

    // Initialize the protocol
    display->SetStatus("加载协议...");
    
#ifdef CONFIG_CONNECTION_TYPE_WEBSOCKET
    protocol_ = std::make_unique<WebsocketProtocol>();
#else
    protocol_ = std::make_unique<MqttProtocol>();
#endif
    protocol_->OnNetworkError([this](const std::string& message) {
        Alert("ERROR", message);
    });
    protocol_->OnIncomingAudio([this](std::vector<uint8_t>&& data) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (device_state_ == kDeviceStateSpeaking) {
            audio_decode_queue_.emplace_back(std::move(data));
        }
    });
    protocol_->OnAudioChannelOpened([this, codec, &board]() {
        board.SetPowerSaveMode(false);
        if (protocol_->server_sample_rate() != codec->output_sample_rate()) {
            ESP_LOGW(TAG, "服务器的音频采样率 %d 与设备输出的采样率 %d 不一致，重采样后可能会失真",
                protocol_->server_sample_rate(), codec->output_sample_rate());
        }
        SetDecodeSampleRate(protocol_->server_sample_rate());
        // 物联网设备描述符
        last_iot_states_.clear();
        auto& thing_manager = iot::ThingManager::GetInstance();
        protocol_->SendIotDescriptors(thing_manager.GetDescriptorsJson());
    });
    protocol_->OnAudioChannelClosed([this, &board]() {
        board.SetPowerSaveMode(true);
        Schedule([this]() {
            auto display = Board::GetInstance().GetDisplay();
            display->SetChatMessage("", "");
            SetDeviceState(kDeviceStateIdle);
        });
    });
    protocol_->OnIncomingJson([this, display](const cJSON* root) {
        // Parse JSON data
        auto type = cJSON_GetObjectItem(root, "type");
        if (strcmp(type->valuestring, "tts") == 0) {
            auto state = cJSON_GetObjectItem(root, "state");
            if (strcmp(state->valuestring, "start") == 0) {
                Schedule([this]() {
                    aborted_ = false;
                    if (device_state_ == kDeviceStateIdle || device_state_ == kDeviceStateListening) {
                        SetDeviceState(kDeviceStateSpeaking);
                    }
                });
            } else if (strcmp(state->valuestring, "stop") == 0) {
                Schedule([this]() {
                    if (device_state_ == kDeviceStateSpeaking) {
                        background_task_->WaitForCompletion();
                        if (keep_listening_) {
                            protocol_->SendStartListening(kListeningModeAutoStop);
                            SetDeviceState(kDeviceStateListening);
                        } else {
                            SetDeviceState(kDeviceStateIdle);
                        }
                    }
                });
            } else if (strcmp(state->valuestring, "sentence_start") == 0) {
                auto text = cJSON_GetObjectItem(root, "text");
                if (text != NULL) {
                    ESP_LOGI(TAG, "<< %s", text->valuestring);
                    Schedule([this, display, message = std::string(text->valuestring)]() {
                        display->SetChatMessage("assistant", message);
                    });
                }
            }
        } else if (strcmp(type->valuestring, "stt") == 0) {
            auto text = cJSON_GetObjectItem(root, "text");
            if (text != NULL) {
                ESP_LOGI(TAG, ">> %s", text->valuestring);

                if (!aliyun_mqtt_ || !aliyun_mqtt_->IsConnected()) {
                    aliyun_mqtt_ = std::make_unique<AliyunMqtt>();
                    mqtt_connected_ = aliyun_mqtt_->Connect(
                        "k19jp8qxcE8",     // 替换为实际的产品密钥
                        "esp32xiaobai",     // 替换为实际的设备名称
                        "bfba858f24e3a8a47bbcf95b635fb01d",   // 替换为实际的设备密钥
                        "cn-shanghai"            // 上海节点
                    );
                    
                    if (mqtt_connected_) {
                        std::string text_str(text->valuestring);
    std::string mqtt_payload = "";
    bool should_publish = false;
    
    // 客厅灯控制
    if (text_str.find("打开客厅灯") != std::string::npos || 
        text_str.find("开客厅灯") != std::string::npos ||
        text_str.find("客厅开灯") != std::string::npos) {
        mqtt_payload = "{\"livingroom_led_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开客厅灯指令");
    }
    else if (text_str.find("关闭客厅灯") != std::string::npos || 
             text_str.find("关客厅灯") != std::string::npos ||
             text_str.find("客厅关灯") != std::string::npos) {
        mqtt_payload = "{\"livingroom_led_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关客厅灯指令");
    }
    
    // 卧室灯控制
    else if (text_str.find("打开卧室灯") != std::string::npos || 
             text_str.find("开卧室灯") != std::string::npos ||
             text_str.find("卧室开灯") != std::string::npos) {
        mqtt_payload = "{\"bedroom_led_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开卧室灯指令");
    }
    else if (text_str.find("关闭卧室灯") != std::string::npos || 
             text_str.find("关卧室灯") != std::string::npos ||
             text_str.find("卧室关灯") != std::string::npos) {
        mqtt_payload = "{\"bedroom_led_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关卧室灯指令");
    }
    
    // 厨房灯控制
    else if (text_str.find("打开厨房灯") != std::string::npos || 
             text_str.find("开厨房灯") != std::string::npos ||
             text_str.find("厨房开灯") != std::string::npos) {
        mqtt_payload = "{\"foodroom_led_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开厨房灯指令");
    }
    else if (text_str.find("关闭厨房灯") != std::string::npos || 
             text_str.find("关厨房灯") != std::string::npos ||
             text_str.find("厨房关灯") != std::string::npos) {
        mqtt_payload = "{\"foodroom_led_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关厨房灯指令");
    }
    
    // 卫生间灯控制
    else if (text_str.find("打开卫生间灯") != std::string::npos || 
             text_str.find("开卫生间灯") != std::string::npos ||
             text_str.find("开厕所灯") != std::string::npos) {
        mqtt_payload = "{\"wc_led_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开卫生间灯指令");
    }
    else if (text_str.find("关闭卫生间灯") != std::string::npos || 
             text_str.find("关卫生间灯") != std::string::npos ||
             text_str.find("卫生间关灯") != std::string::npos) {
        mqtt_payload = "{\"wc_led_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关卫生间灯指令");
    }
    
    // 风扇控制
    else if (text_str.find("打开风扇") != std::string::npos || 
             text_str.find("开风扇") != std::string::npos) {
        mqtt_payload = "{\"fan_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开风扇指令");
    }
    else if (text_str.find("关闭风扇") != std::string::npos || 
             text_str.find("关风扇") != std::string::npos) {
        mqtt_payload = "{\"fan_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关风扇指令");
    }
    
    // 风扇锁定控制
    else if (text_str.find("锁定风扇") != std::string::npos || 
             text_str.find("风扇锁定") != std::string::npos) {
        mqtt_payload = "{\"fan_lock\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到锁定风扇指令");
    }
    else if (text_str.find("解锁风扇") != std::string::npos || 
             text_str.find("风扇解锁") != std::string::npos) {
        mqtt_payload = "{\"fan_lock\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到解锁风扇指令");
    }
    
    // 客厅窗户控制
    else if (text_str.find("打开客厅窗户") != std::string::npos || 
             text_str.find("开客厅窗户") != std::string::npos) {
        mqtt_payload = "{\"livingroom_window_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开客厅窗户指令");
    }
    else if (text_str.find("关闭客厅窗户") != std::string::npos || 
             text_str.find("关客厅窗户") != std::string::npos) {
        mqtt_payload = "{\"livingroom_window_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关客厅窗户指令");
    }
    
    // 卧室窗户控制
    else if (text_str.find("打开卧室窗户") != std::string::npos || 
             text_str.find("开卧室窗户") != std::string::npos) {
        mqtt_payload = "{\"bedroom_window_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到开卧室窗户指令");
    }
    else if (text_str.find("关闭卧室窗户") != std::string::npos || 
             text_str.find("关卧室窗户") != std::string::npos) {
        mqtt_payload = "{\"bedroom_window_state\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到关卧室窗户指令");
    }
    
    // 蜂鸣器锁定控制
    else if (text_str.find("锁定蜂鸣器") != std::string::npos || 
             text_str.find("蜂鸣器锁定") != std::string::npos) {
        mqtt_payload = "{\"buzz_Lock\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到锁定蜂鸣器指令");
    }
    else if (text_str.find("解锁蜂鸣器") != std::string::npos || 
             text_str.find("蜂鸣器解锁") != std::string::npos) {
        mqtt_payload = "{\"buzz_Lock\": 0}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到解锁蜂鸣器指令");
    }
        else if (text_str.find("打开睡眠模式") != std::string::npos) {
        mqtt_payload = "{\"sleeping_state\": 1}";
        should_publish = true;
        ESP_LOGI(TAG, "检测到打开睡眠模式指令");
    }
    
    // 运动控制命令 - 防重复发送机制
    static std::string last_movement_command = "";
    static uint32_t last_movement_time = 0;
    const uint32_t MOVEMENT_COOLDOWN_MS = 3000; // 3秒冷却时间
    
    std::string current_command = "";
    uint32_t current_time = esp_timer_get_time() / 1000; // 转换为毫秒
    
    if (text_str.find("向前走") != std::string::npos || 
        text_str.find("前进") != std::string::npos ||
        text_str.find("往前") != std::string::npos) {
        current_command = "forward";
        mqtt_payload = "{\"command\": \"forward\", \"speed\": 0.3}";
        ESP_LOGI(TAG, "检测到前进指令");
    }
    else if (text_str.find("向后退") != std::string::npos || 
             text_str.find("后退") != std::string::npos ||
             text_str.find("往后") != std::string::npos) {
        current_command = "backward";
        mqtt_payload = "{\"command\": \"backward\", \"speed\": 0.3}";
        ESP_LOGI(TAG, "检测到后退指令");
    }
    else if (text_str.find("向左转") != std::string::npos || 
             text_str.find("左转") != std::string::npos ||
             text_str.find("往左") != std::string::npos) {
        current_command = "left";
        mqtt_payload = "{\"command\": \"left\", \"speed\": 0.3}";
        ESP_LOGI(TAG, "检测到左转指令");
    }
    else if (text_str.find("向右转") != std::string::npos || 
             text_str.find("右转") != std::string::npos ||
             text_str.find("往右") != std::string::npos) {
        current_command = "right";
        mqtt_payload = "{\"command\": \"right\", \"speed\": 0.3}";
        ESP_LOGI(TAG, "检测到右转指令");
    }
    else if (text_str.find("停止") != std::string::npos || 
             text_str.find("停下") != std::string::npos ||
             text_str.find("不要动") != std::string::npos) {
        current_command = "stop";
        mqtt_payload = "{\"command\": \"stop\"}";
        ESP_LOGI(TAG, "检测到停止指令");
    }
    
    // 检查是否需要发送运动命令（防重复发送）
    if (!current_command.empty()) {
        if (last_movement_command != current_command || 
            (current_time - last_movement_time) > MOVEMENT_COOLDOWN_MS) {
            should_publish = true;
            last_movement_command = current_command;
            last_movement_time = current_time;
            ESP_LOGI(TAG, "发送运动命令: %s", current_command.c_str());
        } else {
            ESP_LOGI(TAG, "运动命令 %s 在冷却时间内，跳过发送", current_command.c_str());
        }
    }
    
    // 抽屉控制
    else if (text_str.find("打开抽屉") != std::string::npos) {
        SendUartCommand("x");
        ESP_LOGI(TAG, "检测到打开抽屉指令，发送串口命令: x");
    }
    else if (text_str.find("关上抽屉") != std::string::npos || 
             text_str.find("关闭抽屉") != std::string::npos) {
        SendUartCommand("y");
        ESP_LOGI(TAG, "检测到关闭抽屉指令，发送串口命令: y");
    }
    
    // 发布MQTT消息
    if (should_publish) {
        bool result = aliyun_mqtt_->PublishMessage("/k19jp8qxcE8/esp32xiaobai/user/pub", mqtt_payload);
        if (result) {
            ESP_LOGI(TAG, "MQTT消息发布成功: %s", mqtt_payload.c_str());
        } else {
            ESP_LOGE(TAG, "MQTT消息发布失败: %s", mqtt_payload.c_str());
        }
    }

                        // aliyun_mqtt_->PublishMessage("/k19jp8qxcE8/esp32xiaobai/user/pub", "{\"livingroom_led_state\": \"开灯\" }");
                        ESP_LOGI(TAG, "Successfully connected to Aliyun MQTT");
                    } else {
                        ESP_LOGE(TAG, "Failed to connect to Aliyun MQTT");
                    }
                }

// 智能家居设备控制逻辑        
                // 这里可以发送STT结果到阿里云
                // if (aliyun_mqtt_->IsConnected()) {
                    // if (text->valuestring.contain("开灯"))
                    // {
                        
                    // }
                    
                    // 发送消息到阿里云IoT平台
                    // aliyun_mqtt_->PublishMessage(topic, text->valuestring);
                // }
                Schedule([this, display, message = std::string(text->valuestring)]() {
                    display->SetChatMessage("user", message);
                });
            }
        } else if (strcmp(type->valuestring, "llm") == 0) {
            auto emotion = cJSON_GetObjectItem(root, "emotion");
            if (emotion != NULL) {
                Schedule([this, display, emotion_str = std::string(emotion->valuestring)]() {
                    display->SetEmotion(emotion_str);
                });
            }
        } else if (strcmp(type->valuestring, "iot") == 0) {
            auto commands = cJSON_GetObjectItem(root, "commands");
            if (commands != NULL) {
                auto& thing_manager = iot::ThingManager::GetInstance();
                for (int i = 0; i < cJSON_GetArraySize(commands); ++i) {
                    auto command = cJSON_GetArrayItem(commands, i);
                    thing_manager.Invoke(command);
                }
            }
        }
    });

    // Check for new firmware version or get the MQTT broker address
    ota_.SetCheckVersionUrl(CONFIG_OTA_VERSION_URL);
    ota_.SetHeader("Device-Id", SystemInfo::GetMacAddress().c_str());
    ota_.SetHeader("Client-Id", board.GetUuid());
    xTaskCreate([](void* arg) {
        Application* app = (Application*)arg;
        app->CheckNewVersion();
        vTaskDelete(NULL);
    }, "check_new_version", 4096 * 2, this, 1, nullptr);


#if CONFIG_USE_AUDIO_PROCESSING
    audio_processor_.Initialize(codec->input_channels(), codec->input_reference());
    audio_processor_.OnOutput([this](std::vector<int16_t>&& data) {
        background_task_->Schedule([this, data = std::move(data)]() mutable {
            opus_encoder_->Encode(std::move(data), [this](std::vector<uint8_t>&& opus) {
                Schedule([this, opus = std::move(opus)]() {
                    protocol_->SendAudio(opus);
                });
            });
        });
    });

    wake_word_detect_.Initialize(codec->input_channels(), codec->input_reference());
    wake_word_detect_.OnVadStateChange([this](bool speaking) {
        Schedule([this, speaking]() {
            if (device_state_ == kDeviceStateListening) {
                if (speaking) {
                    voice_detected_ = true;
                } else {
                    voice_detected_ = false;
                }
                auto led = Board::GetInstance().GetLed();
                led->OnStateChanged();
            }
        });
    });

    wake_word_detect_.OnWakeWordDetected([this](const std::string& wake_word) {
        Schedule([this, &wake_word]() {
            if (device_state_ == kDeviceStateIdle) {
                SetDeviceState(kDeviceStateConnecting);
                wake_word_detect_.EncodeWakeWordData();

                if (!protocol_->OpenAudioChannel()) {
                    ESP_LOGE(TAG, "Failed to open audio channel");
                    SetDeviceState(kDeviceStateIdle);
                    wake_word_detect_.StartDetection();
                    return;
                }
                
                std::vector<uint8_t> opus;
                // Encode and send the wake word data to the server
                while (wake_word_detect_.GetWakeWordOpus(opus)) {
                    protocol_->SendAudio(opus);
                }
                // Set the chat state to wake word detected
                protocol_->SendWakeWordDetected(wake_word);
                ESP_LOGI(TAG, "Wake word detected: %s", wake_word.c_str());
                keep_listening_ = true;
                SetDeviceState(kDeviceStateListening);
            } else if (device_state_ == kDeviceStateSpeaking) {
                AbortSpeaking(kAbortReasonWakeWordDetected);
            }

            // Resume detection
            wake_word_detect_.StartDetection();
        });
    });
    wake_word_detect_.StartDetection();
#endif

    SetDeviceState(kDeviceStateIdle);
}

void Application::Schedule(std::function<void()> callback) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        main_tasks_.push_back(std::move(callback));
    }
    xEventGroupSetBits(event_group_, SCHEDULE_EVENT);
}

// The Main Loop controls the chat state and websocket connection
// If other tasks need to access the websocket or chat state,
// they should use Schedule to call this function
void Application::MainLoop() {
    while (true) {
        auto bits = xEventGroupWaitBits(event_group_,
            SCHEDULE_EVENT | AUDIO_INPUT_READY_EVENT | AUDIO_OUTPUT_READY_EVENT,
            pdTRUE, pdFALSE, portMAX_DELAY);

        if (bits & AUDIO_INPUT_READY_EVENT) {
            InputAudio();
        }
        if (bits & AUDIO_OUTPUT_READY_EVENT) {
            OutputAudio();
        }
        if (bits & SCHEDULE_EVENT) {
            std::unique_lock<std::mutex> lock(mutex_);
            std::list<std::function<void()>> tasks = std::move(main_tasks_);
            lock.unlock();
            for (auto& task : tasks) {
                task();
            }
        }
    }
}

void Application::ResetDecoder() {
    std::lock_guard<std::mutex> lock(mutex_);
    opus_decoder_->ResetState();
    audio_decode_queue_.clear();
    last_output_time_ = std::chrono::steady_clock::now();
}

void Application::OutputAudio() {
    auto now = std::chrono::steady_clock::now();
    auto codec = Board::GetInstance().GetAudioCodec();
    const int max_silence_seconds = 10;

    std::unique_lock<std::mutex> lock(mutex_);
    if (audio_decode_queue_.empty()) {
        // Disable the output if there is no audio data for a long time
        if (device_state_ == kDeviceStateIdle) {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_output_time_).count();
            if (duration > max_silence_seconds) {
                codec->EnableOutput(false);
            }
        }
        return;
    }

    if (device_state_ == kDeviceStateListening) {
        audio_decode_queue_.clear();
        return;
    }

    last_output_time_ = now;
    auto opus = std::move(audio_decode_queue_.front());
    audio_decode_queue_.pop_front();
    lock.unlock();

    background_task_->Schedule([this, codec, opus = std::move(opus)]() mutable {
        if (aborted_) {
            return;
        }

        std::vector<int16_t> pcm;
        if (!opus_decoder_->Decode(std::move(opus), pcm)) {
            return;
        }

        // Resample if the sample rate is different
        if (opus_decode_sample_rate_ != codec->output_sample_rate()) {
            int target_size = output_resampler_.GetOutputSamples(pcm.size());
            std::vector<int16_t> resampled(target_size);
            output_resampler_.Process(pcm.data(), pcm.size(), resampled.data());
            pcm = std::move(resampled);
        }
        
        codec->OutputData(pcm);
    });
}

void Application::InputAudio() {
    auto codec = Board::GetInstance().GetAudioCodec();
    std::vector<int16_t> data;
    if (!codec->InputData(data)) {
        return;
    }

    if (codec->input_sample_rate() != 16000) {
        if (codec->input_channels() == 2) {
            auto mic_channel = std::vector<int16_t>(data.size() / 2);
            auto reference_channel = std::vector<int16_t>(data.size() / 2);
            for (size_t i = 0, j = 0; i < mic_channel.size(); ++i, j += 2) {
                mic_channel[i] = data[j];
                reference_channel[i] = data[j + 1];
            }
            auto resampled_mic = std::vector<int16_t>(input_resampler_.GetOutputSamples(mic_channel.size()));
            auto resampled_reference = std::vector<int16_t>(reference_resampler_.GetOutputSamples(reference_channel.size()));
            input_resampler_.Process(mic_channel.data(), mic_channel.size(), resampled_mic.data());
            reference_resampler_.Process(reference_channel.data(), reference_channel.size(), resampled_reference.data());
            data.resize(resampled_mic.size() + resampled_reference.size());
            for (size_t i = 0, j = 0; i < resampled_mic.size(); ++i, j += 2) {
                data[j] = resampled_mic[i];
                data[j + 1] = resampled_reference[i];
            }
        } else {
            auto resampled = std::vector<int16_t>(input_resampler_.GetOutputSamples(data.size()));
            input_resampler_.Process(data.data(), data.size(), resampled.data());
            data = std::move(resampled);
        }
    }
    
#if CONFIG_USE_AUDIO_PROCESSING
    if (audio_processor_.IsRunning()) {
        audio_processor_.Input(data);
    }
    if (wake_word_detect_.IsDetectionRunning()) {
        wake_word_detect_.Feed(data);
    }
#else
    if (device_state_ == kDeviceStateListening) {
        background_task_->Schedule([this, data = std::move(data)]() mutable {
            opus_encoder_->Encode(std::move(data), [this](std::vector<uint8_t>&& opus) {
                Schedule([this, opus = std::move(opus)]() {
                    protocol_->SendAudio(opus);
                });
            });
        });
    }
#endif
}

void Application::AbortSpeaking(AbortReason reason) {
    ESP_LOGI(TAG, "Abort speaking");
    aborted_ = true;
    protocol_->SendAbortSpeaking(reason);
}

void Application::SetDeviceState(DeviceState state) {
    if (device_state_ == state) {
        return;
    }
    
    device_state_ = state;
    ESP_LOGI(TAG, "STATE: %s", STATE_STRINGS[device_state_]);
    // The state is changed, wait for all background tasks to finish
    background_task_->WaitForCompletion();

    auto& board = Board::GetInstance();
    auto codec = board.GetAudioCodec();
    auto display = board.GetDisplay();
    auto led = board.GetLed();
    led->OnStateChanged();
    switch (state) {
        case kDeviceStateUnknown:
        case kDeviceStateIdle:
            display->SetStatus("待命");
            display->SetEmotion("neutral");
#ifdef CONFIG_USE_AUDIO_PROCESSING
            audio_processor_.Stop();
#endif
            break;
        case kDeviceStateConnecting:
            display->SetStatus("连接中...");
            break;
        case kDeviceStateListening:
            display->SetStatus("聆听中...");
            display->SetEmotion("neutral");
            ResetDecoder();
            opus_encoder_->ResetState();
#if CONFIG_USE_AUDIO_PROCESSING
            audio_processor_.Start();
#endif
            UpdateIotStates();
            break;
        case kDeviceStateSpeaking:
            display->SetStatus("说话中...");
            ResetDecoder();
            codec->EnableOutput(true);
#if CONFIG_USE_AUDIO_PROCESSING
            audio_processor_.Stop();
#endif
            break;
        default:
            // Do nothing
            break;
    }
}

void Application::SetDecodeSampleRate(int sample_rate) {
    if (opus_decode_sample_rate_ == sample_rate) {
        return;
    }

    opus_decode_sample_rate_ = sample_rate;
    opus_decoder_.reset();
    opus_decoder_ = std::make_unique<OpusDecoderWrapper>(opus_decode_sample_rate_, 1);

    auto codec = Board::GetInstance().GetAudioCodec();
    if (opus_decode_sample_rate_ != codec->output_sample_rate()) {
        ESP_LOGI(TAG, "Resampling audio from %d to %d", opus_decode_sample_rate_, codec->output_sample_rate());
        output_resampler_.Configure(opus_decode_sample_rate_, codec->output_sample_rate());
    }
}

void Application::UpdateIotStates() {
    auto& thing_manager = iot::ThingManager::GetInstance();
    auto states = thing_manager.GetStatesJson();
    if (states != last_iot_states_) {
        last_iot_states_ = states;
        protocol_->SendIotStates(states);
    }
}

void Application::Reboot() {
    ESP_LOGI(TAG, "Rebooting...");
    esp_restart();
}

void Application::InitUart() {
    // 配置UART参数
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    
    // 配置UART1驱动
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    
    // 设置UART1引脚 (TXD1: GPIO38, RXD1: GPIO48)
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, 38, 48, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // 安装UART1驱动
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, 1024, 1024, 0, NULL, 0));
    
    ESP_LOGI(TAG, "UART1初始化完成，TXD1: GPIO38, RXD1: GPIO48");
}

void Application::SendUartCommand(const std::string& command) {
    if (command.empty()) {
        ESP_LOGW(TAG, "串口命令为空");
        return;
    }
    
    // 在命令后添加换行符作为结束符
    std::string command_with_terminator = command + "\n";
    
    // 发送命令到UART1
    int len = uart_write_bytes(UART_NUM_1, command_with_terminator.c_str(), command_with_terminator.length());
    if (len > 0) {
        ESP_LOGI(TAG, "UART1发送成功: %s (长度: %d)", command_with_terminator.c_str(), len);
    } else {
        ESP_LOGE(TAG, "UART1发送失败: %s", command_with_terminator.c_str());
    }
}