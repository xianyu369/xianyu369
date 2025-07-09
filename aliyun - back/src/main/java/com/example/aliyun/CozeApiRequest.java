package com.example.aliyun;

import okhttp3.*;

import java.io.IOException;

public class CozeApiRequest {
    public static void main(String[] args) {
        // 定义URL
        String url = "https://api.coze.cn/v3/chat";
        
        // 创建一个OkHttpClient实例
        OkHttpClient client = new OkHttpClient();
        
        //一个 创建RequestBody，包含JSON数据
        String jsonBody = "{"
                + "\"bot_id\": \"7508357957743443968\","
                + "\"user_id\": \"123\","
                + "\"stream\": true,"
                + "\"auto_save_history\":true,"
                + "\"additional_messages\":["
                + "    {"
                + "        \"role\":\"user\","
                + "        \"content\":\"鱼缸的摆放位置\","
                + "        \"content_type\":\"text\""
                + "    }"
                + "]"
                + "}";
        
        RequestBody requestBody = RequestBody.create(MediaType.parse("application/json"), jsonBody);
        
        // 创建一个Request对象
        Request request = new Request.Builder()
                .url(url)
                .header("Authorization", "Bearer pat_f6t1GGcgbSam9RXIjIdrIz81oAGNlGAFPaSqxaUu8GUlcVMt64VYboYpZIsrRUYe") // 替换为你的实际Token
                .header("Content-Type", "application/json")
                .post(requestBody)
                .build();
        
        // 异步执行请求
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                e.printStackTrace();
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful()) {
                    // 请求成功，处理响应
                    System.out.println("999"+response.body().string());
                } else {
                    // 请求失败，处理错误
                    System.out.println("Response Code: " + response.code());
                }
            }
        });
    }
}