package com.example.aliyun.tool;

import org.springframework.http.MediaType;
import org.springframework.stereotype.Service;
import org.springframework.web.servlet.mvc.method.annotation.SseEmitter;

import java.io.IOException;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;
@Service
public class SseService {

    // 用于存储所有客户端的SseEmitter
    private final Map<String, CopyOnWriteArrayList<SseEmitter>> emitters = Collections.synchronizedMap(new HashMap<>());

    public SseEmitter subscribe(String userId) {
        emitters.putIfAbsent(userId, new CopyOnWriteArrayList<>());
        SseEmitter emitter = new SseEmitter(Long.MAX_VALUE);
        emitters.get(userId).add(emitter);

        emitter.onCompletion(() -> emitters.get(userId).remove(emitter));
        emitter.onTimeout(() -> emitters.get(userId).remove(emitter));
        emitter.onError(e -> emitters.get(userId).remove(emitter));

        return emitter;
    }

    public void sendToUser(String userId, String data) {
        if (emitters.containsKey(userId)) {
            for (SseEmitter emitter : emitters.get(userId)) {
                try {
                    emitter.send(data, MediaType.APPLICATION_JSON);
                } catch (IOException e) {
                    emitters.get(userId).remove(emitter);
                }
            }
        }
    }
}