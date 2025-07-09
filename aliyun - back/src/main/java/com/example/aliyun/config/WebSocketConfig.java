package com.example.aliyun.config;

import com.example.aliyun.tool.WebSocketEndpoint;
import com.example.aliyun.tool.WebSocketService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;

import jakarta.annotation.PostConstruct;

/**
 * WebSocket配置类
 * 用于初始化WebSocket相关组件
 * 注意：ServerEndpointExporter已在configration.java中定义，无需重复定义
 */
@Configuration
public class WebSocketConfig {
    
    @Autowired
    private WebSocketService webSocketService;
    
    /**
     * 在Spring容器初始化完成后，将WebSocketService实例设置到WebSocketEndpoint中
     */
    @PostConstruct
    public void initWebSocketEndpoint() {
        WebSocketEndpoint.setWebSocketService(webSocketService);
        System.out.println("WebSocketService已注入到WebSocketEndpoint");
    }
}