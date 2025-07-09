package com.example.aliyun.tool;

import okhttp3.*;
import okhttp3.sse.EventSource;
import okhttp3.sse.EventSourceListener;
import okhttp3.sse.EventSources;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.concurrent.TimeUnit;

/**
 * SSE客户端服务类
 * 使用OkHttp接收SSE消息
 */
@Service
public class SseClientService {
    
    @Autowired
    private WebSocketService webSocketService;
    
    private final OkHttpClient client;
    private EventSource eventSource;
    
    public SseClientService() {
        this.client = new OkHttpClient.Builder()
                .connectTimeout(30, TimeUnit.SECONDS)
                .readTimeout(0, TimeUnit.SECONDS) // SSE需要长连接
                .writeTimeout(30, TimeUnit.SECONDS)
                .build();
    }
    
    /**
     * 连接SSE服务器并接收消息
     * @param url SSE服务器地址
     * @param userId 用户ID，用于转发消息
     */
    public void connectSse(String url, String userId) {
        Request request = new Request.Builder()
                .url(url)
                .addHeader("Accept", "text/event-stream")
                .addHeader("Cache-Control", "no-cache")
                .build();
        
        EventSourceListener listener = new EventSourceListener() {
            @Override
            public void onOpen(EventSource eventSource, Response response) {
                System.out.println("SSE连接已建立: " + url);
            }
            
            @Override
            public void onEvent(EventSource eventSource, String id, String type, String data) {
                System.out.println("收到SSE消息 - ID: " + id + ", Type: " + type + ", Data: " + data);
                
                // 将接收到的SSE消息通过WebSocket转发给指定用户
                if (webSocketService != null && data != null && !data.trim().isEmpty()) {
                    webSocketService.sendMessageToUser("1", data);
                }
            }
            
            @Override
            public void onClosed(EventSource eventSource) {
                System.out.println("SSE连接已关闭");
            }
            
            @Override
            public void onFailure(EventSource eventSource, Throwable t, Response response) {
                System.err.println("SSE连接失败: " + t.getMessage());
                if (response != null) {
                    System.err.println("响应码: " + response.code());
                }
            }
        };
        
        // 创建EventSource
        this.eventSource = EventSources.createFactory(client).newEventSource(request, listener);
    }
    
    /**
     * 连接SSE服务器并接收消息（带请求头）
     * @param url SSE服务器地址
     * @param userId 用户ID
     * @param headers 自定义请求头
     */
    public void connectSseWithHeaders(String url, String userId, Headers headers) {
        Request.Builder requestBuilder = new Request.Builder()
                .url(url)
                .addHeader("Accept", "text/event-stream")
                .addHeader("Cache-Control", "no-cache");
        
        // 添加自定义请求头
        if (headers != null) {
            requestBuilder.headers(headers);
        }
        
        Request request = requestBuilder.build();
        
        EventSourceListener listener = new EventSourceListener() {
            @Override
            public void onOpen(EventSource eventSource, Response response) {
                System.out.println("SSE连接已建立: " + url);
            }
            
            @Override
            public void onEvent(EventSource eventSource, String id, String type, String data) {
                System.out.println("收到SSE消息 - ID: " + id + ", Type: " + type + ", Data: " + data);
                
                // 将接收到的SSE消息通过WebSocket转发给指定用户
                if (webSocketService != null && data != null && !data.trim().isEmpty()) {
                    webSocketService.sendMessageToUser(userId, data);
                }
            }
            
            @Override
            public void onClosed(EventSource eventSource) {
                System.out.println("SSE连接已关闭");
            }
            
            @Override
            public void onFailure(EventSource eventSource, Throwable t, Response response) {
                System.err.println("SSE连接失败: " + t.getMessage());
                if (response != null) {
                    System.err.println("响应码: " + response.code());
                }
            }
        };
        
        // 创建EventSource
        this.eventSource = EventSources.createFactory(client).newEventSource(request, listener);
    }
    
    /**
     * 关闭SSE连接
     */
    public void closeSse() {
        if (eventSource != null) {
            eventSource.cancel();
            System.out.println("SSE连接已主动关闭");
        }
    }
    
    /**
     * 检查SSE连接状态
     * @return 连接是否活跃
     */
    public boolean isConnected() {
        return eventSource != null;
    }
}