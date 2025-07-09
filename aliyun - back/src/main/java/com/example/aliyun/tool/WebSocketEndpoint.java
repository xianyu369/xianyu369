package com.example.aliyun.tool;

import jakarta.websocket.OnClose;
import jakarta.websocket.OnMessage;
import jakarta.websocket.OnOpen;
import jakarta.websocket.Session;
import jakarta.websocket.server.PathParam;
import jakarta.websocket.server.ServerEndpoint;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;

@ServerEndpoint("/websocket/{userId}")
@Component
public class WebSocketEndpoint {
    
    private static WebSocketService webSocketService;
    
    // 静态方法用于设置WebSocketService实例
    public static void setWebSocketService(WebSocketService service) {
        webSocketService = service;
    }
    
    @OnOpen
    public void onOpen(Session session, @PathParam("userId") String userId) {
        if (webSocketService != null) {
            webSocketService.addUserSession(userId, session);
            System.out.println("用户 " + userId + " 连接成功");

        } else {
            System.err.println("WebSocketService未初始化");
        }
    }
    
    @OnClose
    public void onClose(Session session, @PathParam("userId") String userId) {
        if (webSocketService != null) {
            webSocketService.removeUserSession(userId, session);
            System.out.println("用户 " + userId + " 断开连接");
        }
    }
    
    @OnMessage
    public void onMessage(String message, Session session, @PathParam("userId") String userId) {
        // 处理接收到的消息
        System.out.println("收到用户 " + userId + " 的消息: " + message);
        
        // 可以通过WebSocketService广播消息
        if (webSocketService != null) {
            webSocketService.broadcastMessage("用户 " + userId + " 说: " + message);
        }
    }
}