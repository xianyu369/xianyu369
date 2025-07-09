<template>
  <view class="container">
    <view class="header-section">
      <text class="app-title">智能遥控中心</text>
      <text class="app-subtitle">Remote Control Center</text>
    </view>
    
    <view class="video-container">
      <div id="video-player" style="width: 100%; height: 100%; background-color: #000000;">
        <!-- 视频播放区域 -->
      </div>
      <view class="video-overlay">
        <view class="status-indicator">
          <view class="status-dot"></view>
          <text class="status-text">实时传输</text>
        </view>
      </view>
    </view>
    
    <view class="control-section">
      <view class="section-title">
        <view class="title-line"></view>
        <text class="title-text">方向控制</text>
        <view class="title-line"></view>
      </view>
      
      <view class="joystick-area">
        <zui-joystick
          enable-arrows
          :debug="debug"
          :size="100"
          :theme="theme1"
          :event-freq="30"
          :stick-range="0.25"
          @change="onJoy1Change"
        />
      </view>
      
      <view class="control-info">
        <view class="info-item">
          <text class="info-label">连接状态</text>
          <text class="info-value connected">已连接</text>
        </view>
        <view class="info-item">
          <text class="info-label">响应延迟</text>
          <text class="info-value">< 50ms</text>
        </view>
      </view>
    </view>
  </view>
</template>

<script>
// 保持原有的 script 部分不变
import arrowTopNormal from "@/static/images/arrow-up-normal.png";
import arrowTopActive from "@/static/images/arrow-up-active.png";
import arrowRightNormal from "@/static/images/arrow-right-normal.png";
import arrowRightActive from "@/static/images/arrow-right-active.png";
import arrowBottomNormal from "@/static/images/arrow-down-normal.png";
import arrowBottomActive from "@/static/images/arrow-down-active.png";
import arrowLeftNormal from "@/static/images/arrow-left-normal.png";
import arrowLeftActive from "@/static/images/arrow-left-active.png";
import joyBase from "@/static/images/background.png";
import joyHandle from "@/static/images/sticker-normal.png";

export default {
  data() {
    return {
      theme1: {
        base: joyBase,
        handle: joyHandle,
        arrowTop: arrowTopNormal,
        arrowRight: arrowRightNormal,
        arrowBottom: arrowBottomNormal,
        arrowLeft: arrowLeftNormal,
      },
      joyEvent: {},
      debug: false,
      canPostKeys: true,
    };
  },
  methods: {
    postkeys(json) {
      if (!this.canPostKeys) return;
      this.canPostKeys = false;
      uni.request({
        url: "https://brief-lioness-initially.ngrok-free.app/PostAllKeys",
        header: {
          'ngrok-skip-browser-warning': 'true'
        },
        method: 'POST',
        data: json,
        complete: () => {
          setTimeout(() => {
            this.canPostKeys = true;
          }, 500);
        }
      });
    },
    onJoy1Change(chg) {
      const dir = chg.direction;
      this.theme1.arrowTop = dir === "top" ? arrowTopActive : arrowTopNormal;
      this.theme1.arrowRight = dir === "right" ? arrowRightActive : arrowRightNormal;
      this.theme1.arrowBottom = dir === "bottom" ? arrowBottomActive : arrowBottomNormal;
      this.theme1.arrowLeft = dir === "left" ? arrowLeftActive : arrowLeftNormal;
      if (chg.direction !== "none") this.joyEvent = chg;
      let json = "{\"command\":\"" + this.joyEvent.direction + "\",\"speed\":0.3}";
      this.postkeys(json);
    },
    toggleDebugger(evt) {
      this.debug = evt.detail.value;
    },
    initVideoPlayer() {
      let pages = getCurrentPages();
      let page = pages[pages.length - 1];
      let currentWebview = page.$getAppWebview();
      let player = new plus.video.VideoPlayer('video-player', {
        src: 'rtmp://192.168.98.93:1935/live',
		top: '100px',
		left:'25px',
		    width: '90%', // 设置宽度为 100%
		    height: '200px', // 设置高度为 200px
		    position: 'static ', // 设置位置为 stati
      });
      currentWebview.append(player);
      player.play();
    }
  },
  mounted() {
    this.initVideoPlayer();
  }
};
</script>

<style lang="scss" scoped>
.container {
  background: linear-gradient(135deg, #0f0c29 0%, #302b63 50%, #24243e 100%);
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 20px 15px 30px;
  box-sizing: border-box;
  position: relative;
  
  // 适配安全区域
  padding-top: calc(20px + env(safe-area-inset-top));
  padding-bottom: calc(30px + env(safe-area-inset-bottom));
  
  &::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: 
      radial-gradient(circle at 20% 30%, rgba(120, 119, 198, 0.2) 0%, transparent 40%),
      radial-gradient(circle at 80% 70%, rgba(255, 255, 255, 0.08) 0%, transparent 40%),
      radial-gradient(circle at 40% 80%, rgba(120, 119, 198, 0.15) 0%, transparent 40%);
    pointer-events: none;
  }
}

.header-section {
  text-align: center;
  margin-bottom: 25px;
  z-index: 2;
  
  .app-title {
    display: block;
    font-size: 24px;
    font-weight: 700;
    color: #ffffff;
    margin-bottom: 5px;
    letter-spacing: 1px;
    text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
  }
  
  .app-subtitle {
    display: block;
    font-size: 12px;
    font-weight: 300;
    color: rgba(255, 255, 255, 0.6);
    letter-spacing: 2px;
    text-transform: uppercase;
  }
}

.video-container {
  width: 100%;
  height: 200px;
  background: linear-gradient(145deg, #000000, #000000);
  margin-bottom: 30px;
  overflow: hidden;
  box-shadow: 
    0 15px 30px rgba(0, 0, 0, 0.3),
    0 8px 15px rgba(0, 0, 0, 0.2),
    inset 0 1px 0 rgba(255, 255, 255, 0.1);
  position: relative;
  border: 1px solid rgba(255, 255, 255, 0.1);
  
  &::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: linear-gradient(135deg, transparent 0%, rgba(255, 255, 255, 0.03) 50%, transparent 100%);
    pointer-events: none;
    z-index: 1;
  }
  
  .video-overlay {
    position: absolute;
    top: 10px;
    right: 10px;
    z-index: 2;
    
    .status-indicator {
      display: flex;
      align-items: center;
      background: rgba(0, 0, 0, 0.8);
      backdrop-filter: blur(8px);
      padding: 6px 12px;
      border-radius: 15px;
      border: 1px solid rgba(255, 255, 255, 0.15);
      
      .status-dot {
        width: 6px;
        height: 6px;
        background: #00ff88;
        border-radius: 50%;
        margin-right: 6px;
        animation: pulse 2s infinite;
      }
      
      .status-text {
        color: #ffffff;
        font-size: 10px;
        font-weight: 500;
      }
    }
  }
}

.control-section {
  width: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  
  .section-title {
    display: flex;
    align-items: center;
    margin-bottom: 25px;
    width: 100%;
    justify-content: center;
    
    .title-text {
      color: #ffffff;
      font-size: 18px;
      font-weight: 600;
      margin: 0 15px;
      letter-spacing: 0.5px;
      white-space: nowrap;
    }
    
    .title-line {
      flex: 1;
      height: 1px;
      background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.3), transparent);
      max-width: 60px;
    }
  }
}

.joystick-area {
  width: 150px;
  height: 150px;
  display: flex;
  justify-content: center;
  align-items: center;
  background: 
    radial-gradient(circle at center, rgba(255, 255, 255, 0.08) 0%, rgba(255, 255, 255, 0.04) 40%, transparent 70%),
    linear-gradient(145deg, rgba(255, 255, 255, 0.08), rgba(255, 255, 255, 0.04));
  border-radius: 50%;
  box-shadow: 
    inset 0 6px 12px rgba(0, 0, 0, 0.2),
    inset 0 3px 6px rgba(0, 0, 0, 0.1),
    0 6px 20px rgba(0, 0, 0, 0.25),
    0 0 0 1px rgba(255, 255, 255, 0.08);
  position: relative;
  margin-bottom: 25px;
  
  // 增加触摸区域
  &::after {
    content: '';
    position: absolute;
    top: -10px;
    left: -10px;
    right: -10px;
    bottom: -10px;
    border-radius: 50%;
    pointer-events: none;
  }
  
  &::before {
    content: '';
    position: absolute;
    top: 8px;
    left: 8px;
    right: 8px;
    bottom: 8px;
    border-radius: 50%;
    background: linear-gradient(145deg, rgba(255, 255, 255, 0.06), transparent);
    pointer-events: none;
  }
  
  > * {
    position: relative;
    z-index: 1;
  }
}

.control-info {
  display: flex;
  justify-content: space-between;
  width: 100%;
  gap: 10px;
  
  .info-item {
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 15px 12px;
    background: rgba(255, 255, 255, 0.08);
    backdrop-filter: blur(8px);
    border-radius: 12px;
    border: 1px solid rgba(255, 255, 255, 0.15);
    flex: 1;
    min-height: 60px;
    justify-content: center;
    
    // 增加触摸反馈
    &:active {
      transform: scale(0.98);
      background: rgba(255, 255, 255, 0.12);
    }
    
    .info-label {
      color: rgba(255, 255, 255, 0.6);
      font-size: 10px;
      margin-bottom: 4px;
      text-transform: uppercase;
      letter-spacing: 0.5px;
      text-align: center;
    }
    
    .info-value {
      color: #ffffff;
      font-size: 14px;
      font-weight: 600;
      text-align: center;
      
      &.connected {
        color: #00ff88;
      }
    }
  }
}

/* 动画效果 */
@keyframes pulse {
  0%, 100% {
    opacity: 1;
    transform: scale(1);
  }
  50% {
    opacity: 0.6;
    transform: scale(1.3);
  }
}

.video-container,
.joystick-area,
.control-info .info-item {
  transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

// 移动端优化的悬停效果（使用触摸反馈）
.video-container:active {
  transform: translateY(-2px);
  box-shadow: 
    0 20px 40px rgba(0, 0, 0, 0.4),
    0 10px 20px rgba(0, 0, 0, 0.3),
    inset 0 1px 0 rgba(255, 255, 255, 0.12);
}

.joystick-area:active {
  transform: translateY(-1px) scale(1.01);
  box-shadow: 
    inset 0 6px 12px rgba(0, 0, 0, 0.25),
    inset 0 3px 6px rgba(0, 0, 0, 0.15),
    0 8px 25px rgba(0, 0, 0, 0.3),
    0 0 0 1px rgba(255, 255, 255, 0.12);
}

/* 针对不同屏幕尺寸的优化 */
@media (max-width: 375px) {
  .container {
    padding: 15px 12px 25px;
  }
  
  .header-section {
    margin-bottom: 20px;
    
    .app-title {
      font-size: 22px;
    }
    
    .app-subtitle {
      font-size: 11px;
    }
  }
  
  .video-container {
    height: 180px;
    margin-bottom: 25px;
  }
  
  .joystick-area {
    width: 130px;
    height: 130px;
    margin-bottom: 20px;
  }
  
  .control-info .info-item {
    padding: 12px 8px;
    min-height: 55px;
    
    .info-label {
      font-size: 9px;
    }
    
    .info-value {
      font-size: 13px;
    }
  }
}

@media (max-width: 320px) {
  .header-section .app-title {
    font-size: 20px;
  }
  
  .video-container {
    height: 160px;
  }
  
  .joystick-area {
    width: 120px;
    height: 120px;
  }
  
  .control-section .section-title .title-text {
    font-size: 16px;
    margin: 0 10px;
  }
}

/* 横屏适配 */
@media (orientation: landscape) and (max-height: 500px) {
  .container {
    flex-direction: row;
    align-items: flex-start;
    padding: 15px;
  }
  
  .header-section {
    position: absolute;
    top: 15px;
    left: 50%;
    transform: translateX(-50%);
    margin-bottom: 0;
    z-index: 10;
    
    .app-title {
      font-size: 18px;
    }
    
    .app-subtitle {
      font-size: 10px;
    }
  }
  
  .video-container {
    width: 60%;
    height: calc(100vh - 80px);
    margin-bottom: 0;
    margin-right: 15px;
    margin-top: 50px;
  }
  
  .control-section {
    width: 40%;
    margin-top: 50px;
    
    .section-title {
      margin-bottom: 15px;
      
      .title-text {
        font-size: 16px;
      }
    }
  }
  
  .joystick-area {
    width: 120px;
    height: 120px;
    margin-bottom: 15px;
  }
  
  .control-info {
    flex-direction: column;
    gap: 8px;
  }
}

/* 防止用户选择文本 */
.container * {
  user-select: none;
  -webkit-user-select: none;
  -webkit-touch-callout: none;
}

/* 优化滚动性能 */
.container {
  -webkit-overflow-scrolling: touch;
  overflow-scrolling: touch;
}
</style>