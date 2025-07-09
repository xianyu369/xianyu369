<template>
  <view class="container">
    <!-- 背景渐变 -->
    <view class="bg-gradient"></view>
    
    <!-- 顶部导航栏 -->
    <view class="header">
      <text class="header-title">音乐播放器</text>
    </view>
    
    <!-- 专辑封面区域 -->
    <view class="album-container">
      <view class="album-wrapper" :class="{ 'album-rotating': isPlaying }">
        <image class="album-cover" :src="currentSong.cover || '../../static/duan.jpg'" mode="aspectFill"></image>
      </view>
    </view>
    
    <!-- 歌曲信息 -->
    <view class="song-info">
      <text class="song-title">{{ currentSong.title }}</text>
      <text class="song-artist">{{ currentSong.artist }}</text>
    </view>
    
    <!-- 进度条 -->
    <view class="progress-container">
      <text class="time-text">{{ formatTime(currentTime) }}</text>
      <view class="progress-bar-container">
        <view class="progress-bar-bg">
          <view class="progress-bar" :style="{ width: progress + '%' }"></view>
        </view>
      </view>
      <text class="time-text">{{ formatTime(duration) }}</text>
    </view>
    
    <!-- 控制按钮 -->
    <view class="controls">
      <view class="control-btn" @tap="prevSong">
        <image class="action-icon1" src="../../static/pre.png" mode="aspectFit"></image>
      </view>
      <view class="play-btn" @tap="togglePlay">
        <image v-if="!isPlaying" class="action-icon1" src="../../static/play.png" mode="aspectFit"></image>
        <image v-else class="action-icon1" src="../../static/stop.png" mode="aspectFit"></image>
      </view>
      <view class="control-btn" @tap="nextSong">
        <image class="action-icon1" src="../../static/next.png" mode="aspectFit"></image>
      </view>
    </view>
    
    <!-- 播放列表 -->
    <view class="playlist-container">
      <view class="playlist-tags">
        <text 
          class="playlist-tag" 
          :class="{ 'active-tag': currentPlaylistType === 'chinese' }" 
          @click="switchPlaylist('chinese')"
        >流行</text>
        <text 
          class="playlist-tag" 
          :class="{ 'active-tag': currentPlaylistType === 'english' }" 
          @click="switchPlaylist('english')"
        >英语</text>
      </view>
      <view class="playlist-header">
        <text class="playlist-title">播放列表</text>
        <text class="playlist-count">{{ playlist.length }}首歌曲</text>
      </view>
      
      <scroll-view class="playlist" scroll-y="true">
        <view 
          v-for="(song, index) in playlist" 
          :key="index" 
          class="playlist-item" 
          :class="{ active: currentSongIndex === index }"
          @tap="playSong(index)"
        >
          <view class="playlist-item-left">
            <text class="playlist-item-index">{{ index + 1 }}</text>
            <image class="playlist-item-cover" :src="song.cover" mode="aspectFill"></image>
            <view class="playlist-item-info">
              <text class="playlist-item-title">{{ song.title }}</text>
              <text class="playlist-item-artist">{{ song.artist }}</text>
            </view>
          </view>
          <text class="playlist-item-duration">{{ formatTime(song.duration) }}</text>
        </view>
      </scroll-view>
    </view>
    <uni-fab ref="fab" :pattern="pattern" :content="content" :horizontal="horizontal" :vertical="vertical"
            :direction="direction" @trigger="trigger" @fabClick="fabClick" />
  </view>
</template>

<script>
// #ifdef H5
import { ref, onMounted, onUnmounted } from 'vue';
// #endif

export default {
  data() {
    return {
      horizontal: "left",
      vertical: 'bottom',
      direction: 'horizontal',
      pattern: {
        color: '#7A7E83',
        backgroundColor: '#fff',
        selectedColor: '#007AFF',
        buttonColor: '#007AFF',
        iconColor: '#fff'
      },
      is_color_type: false,
      content: [{
        iconPath: '../../static/sleep.png',
        text: '控制',
        active: false
      }],
      isPlaying: false,
      currentSongIndex: 0,
      currentPlaylistType: 'chinese', // 当前播放列表类型
      progress: 0,
      currentTime: 0,
      duration: 0,
      chinesePlaylist: [
        {
          title: "七里香",
          artist: "周杰伦",
          cover: "../../static/zhoujielun.jpg",
          duration: 269,
          src: "../../static/1.ogg" // 添加实际音频文件路径
        },
        {
          title: "红尘客栈",
          artist: "周杰伦",
          cover: "../../static/zhoujielun.jpg",
          duration: 234,
          src: "../../static/2.ogg"
        },
        {
          title: "东北民谣",
          artist: "毛不易",
          cover: "../../static/maobuyi.jpg",
          duration: 256,
          src: "../../static/3.ogg"
        },
        {
          title: "遥远的她",
          artist: "毛不易",
          cover: "../../static/maobuyi.jpg",
          duration: 241,
          src: "../../static/4.ogg"
        },
        {
          title: "借",
          artist: "毛不易",
          cover: "../../static/maobuyi.jpg",
          duration: 217,
          src: "../../static/5.ogg"
        },
        {
          title: "知否知否",
          artist: "胡夏-郁可唯",
          cover: "../../static/huxia.jpeg",
          duration: 217,
          src: "../../static/6.ogg"
        },
        {
          title: "我用什么把你留住",
          artist: "福禄寿",
          cover: "../../static/fulushou.jpeg",
          duration: 217,
          src: "../../static/7.ogg"
        },
        {
          title: "大风吹",
          artist: "刘惜君-王赫野",
          cover: "../../static/liuxijun.jpeg",
          duration: 217,
          src: "../../static/8.ogg"
        },
        {
          title: "你礼貌吗",
          artist: "四川芬达",
          cover: "../../static/sichuanfenda.jpeg",
          duration: 217,
          src: "../../static/9.ogg"
        },
      ],
      englishPlaylist: [
        {
          title: "Arise",
          artist: "MAINLANDER",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/11.ogg"
        },
        {
          title: "Lazy Weekend",
          artist: "f0rest",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/12.ogg"
        },
        {
          title: "Again",
          artist: "Shiloh Dynasty",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/13.ogg"
        },
        {
          title: "That's US",
          artist: "Shiloh Dynasty",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/14.ogg"
        },
        {
          title: "Chicago Freestyle",
          artist: "Drake / GIVĒON",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/15.ogg"
        },
        {
          title: "Running Up That Hill",
          artist: "Meg Myers",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/16.ogg"
        },
        {
          title: "Sunset Sunrise",
          artist: "SNØW",
          cover: "../../static/duan.jpg",
          duration: 217,
          src: "../../static/17.ogg"
        },
      ],
      playlist: [], // 将在初始化时设置
      audioContext: null,
    }
  },
  computed: {
    currentSong() {
      return this.playlist[this.currentSongIndex] || { title: '', artist: '', src: '' };
    },
  },
  onLoad(options) {
    console.log("Options type:", options.type);

    // 创建音频上下文
    this.audioContext = uni.createInnerAudioContext();
    
    // 设置音频事件监听
    this.audioContext.onTimeUpdate(() => {
      this.currentTime = this.audioContext.currentTime;
      this.duration = this.audioContext.duration || 0;
      this.progress = this.duration > 0 ? (this.currentTime / this.duration) * 100 : 0;
    });
    
    this.audioContext.onEnded(() => {
      this.nextSong();
    });
    
    this.audioContext.onError((res) => {
      console.error('Audio error:', res);
      uni.showToast({
        title: '音频加载失败',
        icon: 'none'
      });
    });
    
    // 根据传入的type参数初始化播放列表
    if (options && options.type === "1") {
      this.switchPlaylist('chinese');
    } else if (options && options.type === "2") {
      this.switchPlaylist('english');
    } else {
      // 默认加载中文歌曲
      this.switchPlaylist('chinese');
    }
  },
  onUnload() {
    // 页面卸载时释放资源
    if (this.audioContext) {
      this.audioContext.destroy();
    }
  },
  methods: {
    // 添加trigger方法处理uni-fab组件的trigger事件
    trigger(e) {
      console.log('触发fab按钮事件:', e);
      // 根据e.index处理不同按钮的点击事件
    },
    
    // 切换播放列表
    switchPlaylist(type) {
      // 保存当前播放状态
      const wasPlaying = this.isPlaying;
      
      // 暂停当前播放
      if (this.isPlaying) {
        this.audioContext.pause();
        this.isPlaying = false;
      }
      
      // 更新播放列表类型
      this.currentPlaylistType = type;
      
      // 根据类型设置播放列表
      if (type === 'english') {
        this.playlist = [...this.englishPlaylist];
      } else {
        this.playlist = [...this.chinesePlaylist];
      }
      
      // 重置当前歌曲索引
      this.currentSongIndex = 0;
      
      // 加载新的歌曲
      this.loadCurrentSong();
      
      // 如果之前在播放，则继续播放
      if (wasPlaying) {
        setTimeout(() => {
          this.audioContext.play();
          this.isPlaying = true;
        }, 100); // 短暂延迟确保音频已加载
      }
    },
    
    loadCurrentSong() {
      if (!this.currentSong || !this.currentSong.src) {
        console.error('No valid song source found');
        return;
      }
      
      // 使用歌曲对象中的src属性作为音频源
      this.audioContext.src = this.currentSong.src;
      
      // 预加载音频
      this.audioContext.autoplay = false;
      
      console.log('Loading song:', this.currentSong.title, 'Source:', this.currentSong.src);
      
      // 重置进度
      this.currentTime = 0;
      this.progress = 0;
    },
    
    fabClick() {
      uni.switchTab({
        url: '/pages/controller/controller'
      });
    },
    
    togglePlay() {
      if (!this.currentSong || !this.currentSong.src) {
        uni.showToast({
          title: '没有可播放的歌曲',
          icon: 'none'
        });
        return;
      }
      
      this.isPlaying = !this.isPlaying;
      
      if (this.isPlaying) {
        console.log('Playing song:', this.currentSong.title);
        this.audioContext.play();
      } else {
        console.log('Pausing song:', this.currentSong.title);
        this.audioContext.pause();
      }
    },
    
    prevSong() {
      this.currentSongIndex = (this.currentSongIndex - 1 + this.playlist.length) % this.playlist.length;
      this.loadCurrentSong();
      
      if (this.isPlaying) {
        setTimeout(() => {
          this.audioContext.play();
        }, 100);
      }
    },
    
    nextSong() {
      this.currentSongIndex = (this.currentSongIndex + 1) % this.playlist.length;
      this.loadCurrentSong();
      
      if (this.isPlaying) {
        setTimeout(() => {
          this.audioContext.play();
        }, 100);
      }
    },
    
    playSong(index) {
      if (this.currentSongIndex !== index) {
        this.currentSongIndex = index;
        this.loadCurrentSong();
      }
      
      setTimeout(() => {
        this.isPlaying = true;
        this.audioContext.play();
      }, 100);
    },
    
    formatTime(seconds) {
      seconds = Math.floor(seconds || 0);
      const mins = Math.floor(seconds / 60);
      const secs = seconds % 60;
      return `${mins}:${secs < 10 ? "0" + secs : secs}`;
    },
  },
}
</script>

<style scoped>
.custom-button {
  position: fixed;
  bottom: 15vh;
  right: 20px;
  width: 50px;
  height: 50px;
  background-color: #ff00ff;
  color: #fff;
  font-size: 12px;
  text-align: center;
  line-height: 50px;
  border-radius: 50%;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
}

/* 标签容器样式 */
.playlist-tags {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20rpx;
}

/* 单个标签样式 */
.playlist-tag {
  font-size: 32rpx;
  color: #ffffff;
  font-weight: bold;
  padding: 10rpx 20rpx;
  background-color: rgba(255, 255, 255, 0.2);
  border-radius: 10rpx;
  text-align: center;
  flex: 1;
  margin: 0 10rpx;
  transition: all 0.3s ease;
}

/* 活跃标签样式 */
.active-tag {
  background-color: rgba(255, 255, 255, 0.4);
  transform: scale(1.05);
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
}

/* 全局样式 */
.container {
  position: relative;
  width: 100%;
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 0 30rpx;
  box-sizing: border-box;
  overflow: hidden;
}

.action-icon1 {
  height: 120rpx;
  width: 120rpx;
  color: #666;
}

/* 背景渐变 */
.bg-gradient {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-image: linear-gradient(to bottom, #5ca5ff, #9ce4fe);
  opacity: 0.9;
  z-index: -1;
}

/* 顶部导航栏 */
.header {
  width: 100%;
  height: 90rpx;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: 40rpx;
}

.header-title {
  font-size: 36rpx;
  color: #ffffff;
  font-weight: bold;
}

/* 专辑封面区域 */
.album-container {
  width: 100%;
  display: flex;
  justify-content: center;
  margin-top: 60rpx;
}

.album-wrapper {
  width: 500rpx;
  height: 500rpx;
  border-radius: 50%;
  overflow: hidden;
  box-shadow: 0 10rpx 30rpx rgba(0, 0, 0, 0.3);
  border: 10rpx solid rgba(255, 255, 255, 0.2);
}

.album-cover {
  width: 100%;
  height: 100%;
}

/* 专辑旋转动画 */
@keyframes rotate {
  from {
    transform: rotate(0deg);
  }
  to {
    transform: rotate(360deg);
  }
}

.album-rotating {
  animation: rotate 20s linear infinite;
}

/* 歌曲信息 */
.song-info {
  width: 100%;
  display: flex;
  flex-direction: column;
  align-items: center;
  margin-top: 60rpx;
}

.song-title {
  font-size: 40rpx;
  color: #ffffff;
  font-weight: bold;
  margin-bottom: 10rpx;
}

.song-artist {
  font-size: 28rpx;
  color: rgba(255, 255, 255, 0.8);
}

/* 进度条 */
.progress-container {
  width: 100%;
  display: flex;
  align-items: center;
  margin-top: 60rpx;
}

.time-text {
  font-size: 24rpx;
  color: rgba(255, 255, 255, 0.8);
  width: 80rpx;
  text-align: center;
}

.progress-bar-container {
  flex: 1;
  margin: 0 20rpx;
}

.progress-bar-bg {
  width: 100%;
  height: 6rpx;
  background-color: rgba(255, 255, 255, 0.3);
  border-radius: 3rpx;
  overflow: hidden;
}

.progress-bar {
  height: 100%;
  background-color: #ffffff;
  border-radius: 3rpx;
}

/* 控制按钮 */
.controls {
  width: 100%;
  display: flex;
  justify-content: center;
  align-items: center;
  margin-top: 60rpx;
}

.control-btn {
  width: 80rpx;
  height: 80rpx;
  display: flex;
  justify-content: center;
  align-items: center;
}

.play-btn {
  width: 120rpx;
  height: 120rpx;
  border-radius: 50%;
  background-color: rgba(255, 255, 255, 0.2);
  display: flex;
  justify-content: center;
  align-items: center;
}

/* 播放列表 */
.playlist-container {
  width: 100%;
  margin-top: 60rpx;
  background-color: rgba(255, 255, 255, 0.1);
  border-radius: 20rpx;
  padding: 30rpx;
  box-sizing: border-box;
  margin-bottom: 30rpx;
}

.playlist-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20rpx;
}

.playlist-title {
  font-size: 32rpx;
  color: #ffffff;
  font-weight: bold;
}

.playlist-count {
  font-size: 24rpx;
  color: rgba(255, 255, 255, 0.7);
}

.playlist {
  max-height: 600rpx;
}

.playlist-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20rpx 0;
  border-bottom: 1rpx solid rgba(255, 255, 255, 0.1);
}

.playlist-item.active {
  background-color: rgba(255, 255, 255, 0.2);
  border-radius: 10rpx;
  padding: 20rpx 10rpx;
}

.playlist-item-left {
  display: flex;
  align-items: center;
}

.playlist-item-index {
  width: 40rpx;
  font-size: 24rpx;
  color: rgba(255, 255, 255, 0.7);
  text-align: center;
}

.playlist-item-cover {
  width: 80rpx;
  height: 80rpx;
  border-radius: 10rpx;
  margin: 0 20rpx;
}

.playlist-item-info {
  display: flex;
  flex-direction: column;
}

.playlist-item-title {
  font-size: 28rpx;
  color: #ffffff;
  margin-bottom: 6rpx;
}

.playlist-item-artist {
  font-size: 24rpx;
  color: rgba(255, 255, 255, 0.7);
}

.playlist-item-duration {
  font-size: 24rpx;
  color: rgba(255, 255, 255, 0.7);
}
</style>