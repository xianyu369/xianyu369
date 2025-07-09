package com.example.aliyun.tool;

import com.fasterxml.jackson.databind.ObjectMapper;
import jakarta.websocket.Session;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * WebSocket消息推送服务类
 * 用于管理WebSocket连接和推送消息
 */
@Service
public class WebSocketService {
    
    // 存储所有在线用户的WebSocket会话
    private static final Map<String, CopyOnWriteArrayList<Session>> onlineUsers = new ConcurrentHashMap<>();
    
    private final ObjectMapper objectMapper = new ObjectMapper();
    
    /**
     * 添加用户会话
     * @param userId 用户ID
     * @param session WebSocket会话
     */
    public void addUserSession(String userId, Session session) {
        onlineUsers.putIfAbsent(userId, new CopyOnWriteArrayList<>());
        onlineUsers.get(userId).add(session);
        
        // 注意：不在这里设置MessageHandler，因为WebSocketEndpoint已经通过@OnMessage注解处理消息
        // 避免"A text message handler has already been configured"错误
    }
    
    /**
     * 移除用户会话
     * @param userId 用户ID
     * @param session WebSocket会话
     */
    public void removeUserSession(String userId, Session session) {
        if (onlineUsers.containsKey(userId)) {
            onlineUsers.get(userId).remove(session);
            if (onlineUsers.get(userId).isEmpty()) {
                onlineUsers.remove(userId);
            }
        }
    }
    
    /**
     * 向指定用户推送消息
     * @param userId 用户ID
     * @param message 消息内容
     */
    public void sendMessageToUser(String userId, String message) {
        if (onlineUsers.containsKey(userId)) {
            CopyOnWriteArrayList<Session> sessions = onlineUsers.get(userId);
            for (Session session : sessions) {
                if (session.isOpen()) {
                    try {
                        session.getBasicRemote().sendText(message);
                    } catch (IOException e) {
                        // 发送失败，移除该会话
                        sessions.remove(session);
                        e.printStackTrace();
                    }
                } else {
                    // 会话已关闭，移除
                    sessions.remove(session);
                }
            }
        }
    }
    
    /**
     * 向指定用户推送JSON对象
     * @param userId 用户ID
     * @param jsonObject JSON对象
     */
    public void sendJsonToUser(String userId, Object jsonObject) {
        try {
            String jsonString = objectMapper.writeValueAsString(jsonObject);
            sendMessageToUser(userId, jsonString);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 广播消息给所有在线用户
     * @param message 消息内容
     */
    public void broadcastMessage(String message) {
        for (Map.Entry<String, CopyOnWriteArrayList<Session>> entry : onlineUsers.entrySet()) {
            CopyOnWriteArrayList<Session> sessions = entry.getValue();
            for (Session session : sessions) {
                if (session.isOpen()) {
                    try {
                        session.getBasicRemote().sendText(message);
                    } catch (IOException e) {
                        sessions.remove(session);
                        e.printStackTrace();
                    }
                } else {
                    sessions.remove(session);
                }
            }
        }
    }
    
    /**
     * 广播JSON对象给所有在线用户
     * @param jsonObject JSON对象
     */
    public void broadcastJson(Object jsonObject) {
        try {
            String jsonString = objectMapper.writeValueAsString(jsonObject);
            broadcastMessage(jsonString);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 获取在线用户数量
     * @return 在线用户数量
     */
    public int getOnlineUserCount() {
        return onlineUsers.size();
    }
    
    /**
     * 检查用户是否在线
     * @param userId 用户ID
     * @return 是否在线
     */
    public boolean isUserOnline(String userId) {
        return onlineUsers.containsKey(userId) && !onlineUsers.get(userId).isEmpty();
    }
    
    /**
     * 处理JSON字符串并推送给指定用户
     * 这个方法专门为usercontroller中的processJsonAndSendSse方法提供支持
     * @param userId 用户ID
     * @param jsonString JSON字符串
     */
    public void processJsonAndSendToUser(String userId, String jsonString) {
        try {
            // 验证JSON格式
            objectMapper.readTree(jsonString);
            // 推送消息
            sendMessageToUser(userId, jsonString);
        } catch (Exception e) {
            e.printStackTrace();
            // 如果JSON格式错误，发送错误消息
            sendMessageToUser(userId, "{\"error\":\"Invalid JSON format\"}");
        }
    }
}