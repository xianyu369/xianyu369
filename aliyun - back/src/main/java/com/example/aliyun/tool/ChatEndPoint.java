package com.example.aliyun.tool;

import com.example.aliyun.config.GetHttpSessionConfig;
import com.example.aliyun.pojo.deng;
import com.google.gson.Gson;
import jakarta.servlet.http.HttpSession;
import jakarta.websocket.*;
import jakarta.websocket.server.ServerEndpoint;
import okhttp3.*;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

@ServerEndpoint(value = "/Stream")
@Component
public class ChatEndPoint {
    private static final Map<String,Session> onlineUsers = new ConcurrentHashMap<>();

    private Session httpSession;
    /**
     *
     * @param session
     */
    @OnOpen
    public void onOpen(Session session  ,EndpointConfig config) throws IOException {
//        this.httpSession = (HttpSession) config.getUserProperties().get(HttpSession.class.getName());
        String user = "1";
        System.out.println(session);
        httpSession=session;
//        onlineUsers.put(user,session);
        //2，广播消息。需要将登陆的所有的用户推送给所有的用户
        deng deng=new deng();
        deng.setType3(true);
        deng.setType2(true);
        deng.setType1(true);
        deng.setClose(true);
        Gson gson=new Gson();
        String string= gson.toJson(deng);
//        System.out.println(string);
//        session.getBasicRemote().sendText(string);

//        broadcastAllUsers(string);
    }
    @OnMessage
    public void onMessage(Session session,String  message) throws IOException {
        new Thread(() -> {
            try {
                System.out.println(message);
                OkHttpClient client = new OkHttpClient();
                Gson gson = new Gson();
                String requestBody = gson.toJson(Map.of(
                        "model", "qwen3:4b",
                        "prompt", message,
                        "stream", true
                ));

                Request request = new Request.Builder()
                        .url("http://localhost:11434/api/generate")
                        .post(RequestBody.create(requestBody, MediaType.parse("application/json")))
                        .build();

                Response response = client.newCall(request).execute();
                if (!response.isSuccessful()) {
                    session.getBasicRemote().sendText("Error: " + response.code());
                    return;
                }

                // 逐行读取流式响应并发送给客户端
                try (ResponseBody body = response.body()) {
                    if (body != null) {
                        String line;
                        while ((line = body.source().readUtf8Line()) != null) {
                            System.out.println(line);
                            session.getBasicRemote().sendText(line);
                        }
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
        session.getBasicRemote().sendText("{a:"+message+"}");
        System.out.println(message);
    }
    @OnClose
    public void onClose(Session session){

    }
//    @OnError
//    public void onError(Session session){
//
//    }
    public void broadcastAllUsers(String message) {
        try {
            //遍历map集合
            Set<Map.Entry<String, Session>> entries = onlineUsers.entrySet();
            for (Map.Entry<String, Session> entry : entries) {
                //获取到所有用户对应的session对象
                Session session = entry.getValue();
                //发送消息
                session.getBasicRemote().sendText(message);
            }
        } catch (Exception e) {
            //记录日志
        }
    }
}
