# 智能家居控制系统设计文档

## 项目概述

本设计基于 **STM32L475VET6** 开发板、**ESP32S3** 开发板和 **Jetson Nano** 硬件平台，结合 **RT-Thread 实时操作系统** 与 **阿里云物联网平台**，构建了一套高效、智能的家居控制系统。

团队创新性地开发了**智能服务机器人**，其主控采用 Jetson Nano，具备监控、运输、定点移动、语音聊天等功能。系统通过多传感器融合、语音交互及自主导航技术，实现环境监测、智能设备联动与主动安防预警，显著提升家居自动化水平与人机交互体验。

## 系统架构

### 硬件架构

- **STM32L475VET6**：作为低功耗主控，负责传感器数据采集、设备驱动及通信协议处理。
- **Jetson Nano**：外接 USB 摄像头等设备，通过 RTMP 推流将视频流实时传输到团队自主搭建的服务器，实现行为识别检测，从而控制家居设备做出相应的反应。
- **机器人导航**：通过激光雷达 SLAM 建图构建室内地图，结合改进的路径规划算法，实现全屋自主巡逻。
- **运动控制**：通过 MQTT 协议实现手机 APP 对智能服务机器人运动状态的控制。
- **定点移动**：使用 `waterplus_map_tools` 包，编写节点发送导航指令，实现机器人在家中的定点移动，方便用户远程查看家中状况。
- **环境监测模块**：集成温湿度、光照、PM2.5 等传感器，数据经 MQTT 协议上传至阿里云，支持可视化展示与智能策略下发。

### 软件架构

- **RT-Thread 实时操作系统**：
  - 采用多任务调度机制，优化任务优先级分配。
  - 安防事件（如入侵检测、陌生人告警）设为最高优先级。
  - 环境调控任务次之。
  - 通信任务采用事件驱动模式。

- **Jetson Nano 行为识别流水线**：
  - 融合 **YOLOv5** 目标检测。
  - **SlowFast** 时序动作分析。
  - **DeepSort** 目标跟踪技术。
  - 可识别跌倒等异常行为。

### 手机软件架构

- **技术栈**：
  - **UniApp**：跨平台移动应用开发框架。
  - **WebSocket**：实现实时通信。
  - **虚拟摇杆**：控制机器人运动。
  - **RTMP 推流播放**：实时视频流播放。
  - **内网穿透工具 ngrok**：实现公网访问。
  - **服务器代理技术**：Nginx + RTMP 模块转发。
  - **后端框架**：Spring Boot。
  - **通信协议**：阿里云 MQTT 协议。

- **功能实现**：
  - 实现手机端与物联网设备端的高效连接和交互。

## 项目创新点

1. **多模态交互与云同步融合**：
   - 实现多端数据同步和高效控制。

2. **应用推理大模型**：
   - 提升语音助手的自然交互能力和服务丰富度。

3. **内网穿透技术**：
   - 实现手机 APP 直连远程控制，保障数据安全。

4. **Transformer 架构优化行为识别算法**：
   - 提高复杂行为模式的识别和预测准确性。

## 项目意义

本方案为智能家居领域提供了“端-边-云”协同的可行范例，多模态感知、分级安防及自适应控制策略对嵌入式 AI 应用具有广泛参考价值。
