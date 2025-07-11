#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <driver/uart.h>
#include <driver/gpio.h>

#include <string>
#include <mutex>
#include <list>

#include <opus_encoder.h>
#include <opus_decoder.h>
#include <opus_resampler.h>

#include "protocol.h"
#include "ota.h"
#include "background_task.h"

// 添加前向声明
class AliyunMqtt;

#if CONFIG_USE_AUDIO_PROCESSING
#include "wake_word_detect.h"
#include "audio_processor.h"
#endif

#define SCHEDULE_EVENT (1 << 0)
#define AUDIO_INPUT_READY_EVENT (1 << 1)
#define AUDIO_OUTPUT_READY_EVENT (1 << 2)

enum DeviceState {
    kDeviceStateUnknown,
    kDeviceStateStarting,
    kDeviceStateWifiConfiguring,
    kDeviceStateIdle,
    kDeviceStateConnecting,
    kDeviceStateListening,
    kDeviceStateSpeaking,
    kDeviceStateUpgrading,
    kDeviceStateActivating,
    kDeviceStateFatalError
};

#define OPUS_FRAME_DURATION_MS 60

class Application {
public:
    static Application& GetInstance() {
        static Application instance;
        return instance;
    }
    // 删除拷贝构造函数和赋值运算符
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Start();
    DeviceState GetDeviceState() const { return device_state_; }
    bool IsVoiceDetected() const { return voice_detected_; }
    void Schedule(std::function<void()> callback);
    void SetDeviceState(DeviceState state);
    void Alert(const std::string& status, const std::string& message, const std::string& emotion = "", const std::string& sound = "");
    void AbortSpeaking(AbortReason reason);
    void ToggleChatState();
    void StartListening();
    void StopListening();
    void UpdateIotStates();
    void Reboot();

private:
    Application();
    ~Application();

#if CONFIG_USE_AUDIO_PROCESSING
    WakeWordDetect wake_word_detect_;
    AudioProcessor audio_processor_;
#endif
    Ota ota_;
    std::mutex mutex_;
    std::list<std::function<void()>> main_tasks_;
    std::unique_ptr<Protocol> protocol_;
    EventGroupHandle_t event_group_;
    volatile DeviceState device_state_ = kDeviceStateUnknown;
    bool keep_listening_ = false;
    bool aborted_ = false;
    bool voice_detected_ = false;
    std::string last_iot_states_;

    // Audio encode / decode
    BackgroundTask* background_task_ = nullptr;
    std::chrono::steady_clock::time_point last_output_time_;
    std::list<std::vector<uint8_t>> audio_decode_queue_;

    std::unique_ptr<OpusEncoderWrapper> opus_encoder_;
    std::unique_ptr<OpusDecoderWrapper> opus_decoder_;
    std::unique_ptr<AliyunMqtt> aliyun_mqtt_;
    bool mqtt_connected_ = false;
    int opus_decode_sample_rate_ = -1;
    OpusResampler input_resampler_;
    OpusResampler reference_resampler_;
    OpusResampler output_resampler_;

    // 串口相关
    void InitUart();
    void SendUartCommand(const std::string& command);

    void MainLoop();
    void InputAudio();
    void OutputAudio();
    void ResetDecoder();
    void SetDecodeSampleRate(int sample_rate);
    void CheckNewVersion();
    void ShowActivationCode();

    void PlayLocalFile(const char* data, size_t size);
};

#endif // _APPLICATION_H_
