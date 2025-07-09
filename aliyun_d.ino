#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <ArduinoJson.h>

#define LED_PIN     22         // 连接到WS2812灯带的控制引脚
#define NUM_LEDS    7         // 灯带上的LED数量
#define BRIGHTNESS  10        // 最大LED亮度，范围从0到255
#define LED_TYPE    WS2812     // LED类型，可以是WS2812、WS2812B等
#define COLOR_ORDER GRB        // 颜色顺序，可以是RGB或GRB

CRGB leds[NUM_LEDS];           // 定义一个LED数组
char led_flag = 0;
int rain;
int last_led_flag = -1;  // 用于记录上次的模式

const char* ssid = "hotspot"; // wifi名
const char* password = "12345678"; // wifi密码
const char* mqttHostUrl = "k19jp8qxcE8.iot-as-mqtt.cn-shanghai.aliyuncs.com"; // 阿里云服务器域名
const int mqttPort = 1883; // 端口号
const char* mqttUser = "esp32_ws&k19jp8qxcE8"; // username
const char* mqttPassword = "6d47d535d94acf1bb4759280e74266d74b2705fdc7b33e6fe39ce8e7629e25cb"; // 密钥
const char* clientId = "k19jp8qxcE8.esp32_ws|securemode=2,signmethod=hmacsha256,timestamp=1740230593456|"; // 连接id

#define PRODUCT_KEY       "k19jp8qxcE8"
#define DEVICE_NAME       "esp32_ws"
#define ALINK_BODY_FORMAT         "{\"id\":\"level\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":%s}"

#define ALINK_TOPIC_PROP_POST     "/k19jp8qxcE8/esp32_ws/user/pub"

unsigned long lastMs = 0;
unsigned long lastSerialProcess = 0;
WiFiClient espClient;
PubSubClient client(espClient);

String serialString;  // 用于累积串口数据
bool newDataReceived = false;

char serialBuffer[128];  // 串口缓冲区
uint8_t bufferIndex = 0;

// 连接wifi
void wifiInit() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

// void processSerialData() {
//   if (serialString.length() == 0) return;

//   Serial.print("收到原始数据: ");
//   Serial.println(serialString);

//   // 处理 "aht10_temp:23" 格式
//   if (serialString.indexOf("aht10_temp:") != -1) {
//     int start = serialString.indexOf("aht10_temp:") + 11; // 跳过 "aht10_temp:"
//     float temp = serialString.substring(start).toFloat();
    
//     char payload[128];
//     sprintf(payload, "{\"aht10_temp\":%.1f}", temp);
//     client.publish(ALINK_TOPIC_PROP_POST, payload);
//     Serial.print("发送温度数据: ");
//     Serial.println(payload);
//   }
// }

// 发送当前灯带模式到阿里云
void sendLedModeToCloud(int mode) {
  char payload[128];
  sprintf(payload, "{\"type\":%d}", mode);
  client.publish(ALINK_TOPIC_PROP_POST, payload);
  Serial.print("发送灯带模式数据: ");
  Serial.println(payload);
}

// MQTT消息回调函数
void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  
  // 检查各种消息类型并发送对应数字
  if (strstr(message, "{\"livingroom_led_state\":1}") != NULL) {
    Serial.print('7');
    led_flag = 0;
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"livingroom_led_state\":0}") != NULL) {
    Serial.println('6');
    led_flag = 0;
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"bedroom_led_state\":0}") != NULL) {
    Serial.println('8');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"bedroom_led_state\":1}") != NULL) {
    Serial.println('9');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"foodroom_led_state\":0}") != NULL) {
    Serial.println('a');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"foodroom_led_state\":1}") != NULL) {
    Serial.println('b');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"wc_led_state\":0}") != NULL) {
    Serial.println("c");
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"wc_led_state\":1}") != NULL) {
    Serial.println('d');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"fan_state\":0}") != NULL) {
    Serial.println("e");
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"fan_state\":1}") != NULL) {
    Serial.println('f');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"buzz_Lock\":0}") != NULL) {
    Serial.println('g');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"buzz_Lock\":1}") != NULL) {
    Serial.println('h');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"livingroom_window_state\":0}") != NULL) {
    Serial.println('i');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"livingroom_window_state\":1}") != NULL) {
    Serial.println('j');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"bedroom_window_state\":0}") != NULL) {
    Serial.println('k');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"bedroom_window_state\":1}") != NULL) {
    Serial.println('l');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"bedroom_window_state\":0}") != NULL) {
    Serial.println('m');
    FastLED.clear();
    FastLED.show();
    return;
  }
  else if (strstr(message, "{\"sleeping_state\":1}") != NULL) {
    Serial.println('6');
    Serial.println('8');
    Serial.println('10');
    Serial.println('12');
    FastLED.clear();
    FastLED.show();
    return;
  }
  
  // 根据接收到的消息设置led_flag
  if ((char)payload[7] == '1') {
    led_flag = 1;
  } else if ((char)payload[7] == '2') {
    led_flag = 2;
  } else if ((char)payload[7] == '3') {
    led_flag = 3;
  } else if ((char)payload[7] == '4') {
    led_flag = 4;
  } else if ((char)payload[7] == '5') {
    led_flag = 5;
  } else if ((char)payload[7] == '0') {
    led_flag = 0;
    FastLED.clear();
    FastLED.show();
  }
}

// MQTT连接
void mqttCheckConnect() {
  client.setServer(mqttHostUrl, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(clientId, mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe("/k19jp8qxcE8/esp32_ws/user/get");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

// 组数据帧并发送
void mqttIntervalPost() {
  char param[32];
  char jsonBuf[128];
  
  sprintf(param, "{\"rain_num\":%d,\"type\":%d}", rain, led_flag);
  sprintf(jsonBuf, ALINK_BODY_FORMAT, param);

  client.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
  delay(800);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  wifiInit();
  mqttCheckConnect();

  pinMode(23, INPUT);  // A0口接收模拟输入信号
  pinMode(16, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  static char command = 0;
  client.loop();
  
  analogWrite(2, (map(analogRead(A0), 0, 1023, 235, 0)));
  rain = map(analogRead(A0), 0, 1023, 235, 0);
  
  if (millis() - lastMs >= 4000) {
    lastMs = millis();
    mqttCheckConnect();
    mqttIntervalPost();
    // processSerialData();
  }

  // 持续接收串口数据
  while (Serial.available() > 0) {
    char c = Serial.read();
    serialString += c;
    if (c == '\n' || c == '\r') {
      newDataReceived = true;
    }
  }

  // 检查模式是否改变，如果改变则发送到阿里云
  if (led_flag != last_led_flag) {
    if (led_flag == 1 || led_flag == 3 || led_flag == 5) {
      sendLedModeToCloud(led_flag);
    }
    last_led_flag = led_flag;
  }

  // 根据当前模式显示不同效果
  switch (led_flag) {
    case 0:
      FastLED.clear();
      FastLED.show();
      break;
    case 1:
      breathingLight(0, 0, 255);
      break;
    case 2:
      gradientEffect(20);
      break;
    case 3:
      runningWaterEffect(50);
      break;
    case 4:
      runningChaseEffect(50);
      break;
    case 5:
      runningChaseEffect2(50);
      delay(100);
      runningChaseEffect(50);
      delay(100);
      break;
  }

  // 串口读取模式指令
  if (Serial.available() > 0) {
    command = Serial.read();
    switch (command) {
      case '1':
        led_flag = 1;
        break;
      case '2':
        led_flag = 2;
        break;
      case '3':
        led_flag = 3;
        break;
      case '4':
        led_flag = 4;
        break;
      case '5':
        led_flag = 5;
        break;
      case '0':
        led_flag = 0;
        FastLED.clear();
        FastLED.show();
        break;
    }
  }
}

// 以下是LED效果函数，保持不变
void breathingLight(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(r, g, b);
  }
  for (int j = 1; j < BRIGHTNESS; j += 4) {
    FastLED.setBrightness(j);
    FastLED.show();
    delay(80);
  }
}

void gradientEffect(int time) {
  static uint8_t hue = 0;
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 255));
  FastLED.show();
  hue += 1;
  delay(time);
}

void runningWaterEffect(int time) {
  static uint8_t currentLED = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(40);
  }
  leds[currentLED] = CRGB::Blue;
  FastLED.show();
  currentLED = (currentLED + 1) % NUM_LEDS;
  delay(time);
}

void runningChaseEffect(int time) {
  static uint8_t currentLED = 0;
  FastLED.clear();
  leds[currentLED] = CRGB::Red;
  FastLED.show();
  currentLED = (currentLED + 1) % NUM_LEDS;
  delay(time);
}

void runningChaseEffect2(int time) {
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 3 == 0) leds[i].setRGB(255, 0, 0);
    if (i % 3 == 1) leds[i].setRGB(0, 255, 0);
    if (i % 3 == 2) leds[i].setRGB(0, 0, 255);
    FastLED.show();
    delay(time);
  }
}