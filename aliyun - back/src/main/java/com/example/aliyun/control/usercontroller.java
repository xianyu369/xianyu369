package com.example.aliyun.control;


//import com.example.aliyun.dao.Dao1;
import com.example.aliyun.pojo.*;
import com.example.aliyun.service.IFansService;
import com.example.aliyun.tool.MqttSign;

import com.example.aliyun.tool.SseService;
import com.example.aliyun.tool.SseClientService;
import com.example.aliyun.tool.WebSocketService;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import jakarta.annotation.PostConstruct;
import okhttp3.*;
import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.servlet.mvc.method.annotation.SseEmitter;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


@RestController
@CrossOrigin(origins = "*")
//@RequestMapping("/user")
public class usercontroller {

    private static String productKey = "k19jp8qxcE8";
    private static String deviceName = "devicerx";
    private static String deviceSecret = "94306b01e098424faafd45615ac2d19f";
//    private String deviceSecret = "94306b01e098424faafd45615ac2d19f";


    final String host = "tcp://" + productKey + ".iot-as-mqtt.cn-shanghai.aliyuncs.com:1883";
    private static final String pub_topic = "/k19jp8qxcE8/devicerx/user/pub";
    private static final String sub_topic =  "/k19jp8qxcE8/devicerx/user/get";
    static MqttSign sign = new MqttSign();
    String port = "443";
String broker = "ssl://" + productKey + ".iot-as-mqtt.cn-shanghai.aliyuncs.com" + ":" + port;
    MemoryPersistence persistence = new MemoryPersistence();
    MqttClient sampleClient;
    MqttConnectOptions connOpts;
    private static final String API_KEY = "S5JbuFeonASMIJAM4";
    private static final String BASE_URL = "https://api.seniverse.com/v3/weather/daily.json";
    String getallkeys="{}";
    String Dkeys="{}";

    private static final String DONE = "[DONE]";
    private static final Integer timeout = 60;


    private static final String URL_OLLAMA = "http://localhost:11434/api/generate";

    private static final String MODEL_DEEPSEEK = "deepseek-r1:7b";
//    private static final String pub_topic = "/sys/ "+ productKey + "/" + deviceName + "/thing/event/property/post";

    private final ExecutorService executorService = Executors.newCachedThreadPool();
    private final ObjectMapper objectMapper = new ObjectMapper();
    @Autowired
    private SseService sseService;
    @Autowired
    private SseClientService sseClientService;
    @Autowired
    private IFansService fansService;
    @Autowired
    private WebSocketService webSocketService;



    @GetMapping("/sse")
    public String getSse(@RequestParam String userId) throws MqttException {
        System.out.println(userId+"SUERTYB");
        mqttinit();
        sampleClient.subscribe(sub_topic, new MqttPostPropertyMessageListener());
        return "200";
    }
    
    /**
     * 连接SSE服务器并接收消息
     * @param userId 用户ID
     * @param sseUrl SSE服务器地址2
     * @return 响应状态
     */
    @GetMapping("/connectSse")
    public String connectSse(@RequestParam String userId, @RequestParam String sseUrl) {
        try {
            System.out.println("用户 " + userId + " 连接SSE服务器: " + sseUrl);
            sseClientService.connectSse(sseUrl, userId);
            return "200";
        } catch (Exception e) {
            System.err.println("连接SSE服务器失败: " + e.getMessage());
            e.printStackTrace();
            return "500";
        }
    }
    
    /**
     * 关闭SSE连接
     * @return 响应状态
     */
    @GetMapping("/closeSse")
    public String closeSse() {
        try {
            sseClientService.closeSse();
            return "200";
        } catch (Exception e) {
            System.err.println("关闭SSE连接失败: " + e.getMessage());
            e.printStackTrace();
            return "500";
        }
    }
    
    /**
     * 检查SSE连接状态
     * @return 连接状态信息
     */
    @GetMapping("/sseStatus")
    public String getSseStatus() {
        try {
            boolean isConnected = sseClientService.isConnected();
            return isConnected ? "connected" : "disconnected";
        } catch (Exception e) {
            System.err.println("获取SSE状态失败: " + e.getMessage());
            e.printStackTrace();
            return "error";
        }
    }


    @GetMapping("test")
    public SseEmitter  test(String  jsonString) throws IOException {
        String json = "{\"dynamicKey\":\"1\"}";
        SseEmitter emitter = new SseEmitter();

        executorService.execute(() -> {
            try {
                // 解析JSON字符串
                JsonNode jsonNode = objectMapper.readTree(json);

                // 获取唯一的键和值
                String key = jsonNode.fieldNames().next();
                String value = jsonNode.get(key).asText();

                // 根据值转换为布尔字符串
                Boolean finalValue = value.equals("1");

                // 构建最终的JSON结果
                JsonNode resultNode = objectMapper.createObjectNode().put(key, finalValue);

                // 发送SSE响应
                emitter.send(resultNode.toString(), MediaType.APPLICATION_JSON);
                emitter.complete();
            } catch (IOException e) {
                emitter.completeWithError(e);
            }
        });
        return emitter;
    }
    /**
     * 处理JSON字符串并通过WebSocket推送消息
     * @param jsonString JSON格式的消息字符串
     */
    public void processJsonAndSendSse(String jsonString){
        // 默认推送给用户ID为"1"的用户，可以根据实际需求修改
        String userId = "1";
        webSocketService.processJsonAndSendToUser(userId, jsonString);
    }


    @PostMapping("login")
    public String login(@RequestBody loginform login){
        String temp="500";
        System.out.println(login.autoLogin);
        System.out.println(login.toString() );
//        String password=dao.login(login.getUsername());
//        if (password.equals(login.getPassword())){
//            temp="200";
//        }
//        if (login.username.isEmpty()||login.password.isEmpty()){
//            temp="400";
//        }
        return temp;
    }
    @PostMapping("register")
    public String register(@RequestBody register_form register){
//        dao.register(register.username,register.password);
        return "200";
    }
    @GetMapping("post_fans")
    public void insert_fans(String fan){
        System.out.println(fan);
//        dao.inser_fans(fan);
    }
    @GetMapping("get_fans")
    public List<String> getfans(){
        System.out.println("1111111");
        List<String> list=new ArrayList<>();
        list=fansService.getfan();
        return list;
    }
    @GetMapping("getweather")
    public String getweaather(@RequestParam("location")String location){
        System.out.println(location);
        if (location.isEmpty()){
             location="石家庄";
        }
        System.out.println("ciwndcinwe");
        int temp0=0;
        OkHttpClient client = new OkHttpClient();
        String url = BASE_URL + "?key=" + API_KEY + "&location=" + location + "&language=zh-Hans&unit=c&start=0&days=4";

        Request request = new Request.Builder()
                .url(url)
                .build();

        try (Response response = client.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                throw new IOException("Unexpected code " + response);
            }
            String temp=response.body().string();
            System.out.println(temp);
            return temp;
    } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
    @GetMapping("getdeng")
    public String getdeng() throws MqttException {
        mqttinit();
        sampleClient.subscribe(sub_topic, new MqttPostPropertyMessageListener());
//        System.out.println(getallkeys+"6666");
//        System.out.println(trandeng(getallkeys));
//        return trandeng(getallkeys);
        return "";
    }
    @GetMapping("postdeng")
    public String postdeng(@RequestParam("id")String id) throws MqttException {
//        mqttinit();
        String content="{\"id\":\'"+id+"\'}";
        System.out.println("content"+content);
        MqttMessage message = new MqttMessage(content.getBytes());
        message.setQos(0);
        sampleClient.publish(pub_topic, message);
        return "200";
    }

    @GetMapping("GetAllKeys")
    public String getallkeys() throws MqttException {
        try{
            sampleClient.subscribe(sub_topic, new MqttPostPropertyMessageListener());
        }catch (Exception e){
            return "";
        }
        if (getallkeys.equals("{}")){
            return "400";
        }
//        return tran(getallkeys);
        return "";
    }
    @PostMapping("PostAllKeys")
    public String postallkeys(@RequestBody String data) throws MqttException, JsonProcessingException {
        if(data.contains("command")){
            System.out.println(data+"kdfvns");
            JsonNode jsonNode = objectMapper.readTree(data);

            String command = jsonNode.path("command").asText();
            String content="";
            content=data;
            if(command.equals("top")||command.equals("top-left")||command.equals("top-right")){
                content="{\"command\":\"forward\",\"speed\":0.3}";
            }
            if(command.equals("bottom")||command.equals("bottom-left")||command.equals("bottom-right")){
                content="{\"command\":\"backward\",\"speed\":0.3}";
            }
            mqttinit();
            MqttMessage message = new MqttMessage(content.getBytes());
            message.setQos(0);
            sampleClient.publish(pub_topic, message);
        }else{
        executorService.execute(() -> {
                    try {
                        // 解析JSON字符串
                        JsonNode jsonNode = objectMapper.readTree(data);

                        // 获取唯一的键和值
                        String key = jsonNode.fieldNames().next();
                        Boolean value = jsonNode.get(key).asBoolean();
                        int finalValue = 0;
                        if (value) {
                            finalValue = 1;
                        } else {
                            finalValue = 0;
                        }
                        JsonNode resultNode = objectMapper.createObjectNode().put(key, finalValue);
                        mqttinit();
                        MqttMessage message = new MqttMessage(resultNode.toString().getBytes());
                        System.out.println(resultNode.toString() + "较场坝三大法宝");
                        message.setQos(0);
                        sampleClient.publish(pub_topic, message);
                    } catch (IOException | MqttException e) {
                        e.printStackTrace();
                    }
                });}
//        MqttMessage message = new MqttMessage(content.getBytes());
//        try{
//
//            message.setQos(0);
//            sampleClient.publish(pub_topic, message);
//        }catch (Exception e){
//            System.out.println(e);
//            return "";
//        }`

        return "200";
    }
    public  class MqttPostPropertyMessageListener implements IMqttMessageListener {
        @Override
        public void messageArrived(String var1, MqttMessage var2) throws Exception {
//            System.out.println("reply topic  : " + var2);
            String payload = new String(var2.getPayload(),   StandardCharsets.UTF_8);
//            System.out.println("payload"+payload);
            if (!payload.equals("{}")){
            executorService.execute(() -> {
                try {
                    // 解析JSON字符串
                    JsonNode jsonNode = objectMapper.readTree(payload);

                    // 获取唯一的键和值
                    String key = jsonNode.fieldNames().next();
                    String value = jsonNode.get(key).asText();
                    JsonNode resultNode;
                    if(!key.equals("type")&& !key.equals("music_type")){
                        Boolean finalValue = value.equals("1");
//                        System.out.println("finalValue"+finalValue);
                        resultNode = objectMapper.createObjectNode().put(key, finalValue);
                        System.out.println("finalValue"+resultNode.toString());

                    }else{
                        resultNode=objectMapper.createObjectNode().put(key,value);
                        System.out.println("Value"+resultNode.toString());

                    }
                    webSocketService.broadcastMessage(resultNode.toString());
                    // 构建最终的JSON结果
//                    sseService.sendToUser("1", resultNode.toString());

                } catch (IOException e) {
                    e.printStackTrace();
                }

            });
            }
        }
    }
    public void mqttinit() throws MqttException {
        sign.calculate(productKey, deviceName, deviceSecret);


// Paho MQTT客户端。
        sampleClient = new MqttClient(broker, sign.getClientid(), persistence);

// Paho MQTT连接参数。
        connOpts = new MqttConnectOptions();
        connOpts.setCleanSession(true);
        connOpts.setKeepAliveInterval(180);
        connOpts.setUserName(sign.getUsername());

        connOpts.setPassword(sign.getPassword().toCharArray());
        sampleClient.connect(connOpts);

        connOpts.setConnectionTimeout(30); // 设置连接超时时间为 30 秒
        connOpts.setAutomaticReconnect(true);
    }




    @PostConstruct
     public void initMqttClient() {
        try {
            sign.calculate(productKey, deviceName, deviceSecret);
            sampleClient = new MqttClient(broker, sign.getClientid(),persistence);
            connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            connOpts.setKeepAliveInterval(180);
            connOpts.setUserName(sign.getUsername());

            connOpts.setPassword(sign.getPassword().toCharArray());
            sampleClient.connect(connOpts);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
