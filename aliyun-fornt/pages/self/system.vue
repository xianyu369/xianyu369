<template>
  <view class="system-container">
    <view class="info-card">
      <view class="bg-decoration"></view>
      
      <view class="card-header">
        <text class="title">系统设置</text>
      </view>

      <view class="settings-list">
        <!-- 通知设置 -->
        <view class="settings-group">
          <view class="group-title">
            <text class="iconfont">🔔</text>
            <text>通知设置</text>
          </view>
          <view class="setting-item">
            <text>设备异常提醒</text>
            <switch :checked="settings.deviceAlert" @change="handleSwitchChange('deviceAlert', $event)" color="#1e3c72" />
          </view>
          <view class="setting-item">
            <text>系统消息通知</text>
            <switch :checked="settings.systemNotice" @change="handleSwitchChange('systemNotice', $event)" color="#1e3c72" />
          </view>
        </view>

        <!-- 安全设置 -->
        <view class="settings-group">
          <view class="group-title">
            <text class="iconfont">🔒</text>
            <text>安全设置</text>
          </view>
          <view class="setting-item" @tap="navigateTo('password')">
            <text>修改密码</text>
            <text class="iconfont arrow">></text>
          </view>
          <view class="setting-item">
            <text>指纹解锁</text>
            <switch :checked="settings.fingerprint" @change="handleSwitchChange('fingerprint', $event)" color="#1e3c72" />
          </view>
          <view class="setting-item">
            <text>人脸识别</text>
            <switch :checked="settings.faceId" @change="handleSwitchChange('faceId', $event)" color="#1e3c72" />
          </view>
        </view>

        <!-- 主题设置 -->


        <!-- 其他设置 -->
        <view class="settings-group">
          <view class="group-title">
            <text class="iconfont">⚙️</text>
            <text>其他设置</text>
          </view>
          <view class="setting-item">
            <text>自动备份</text>
            <switch :checked="settings.autoBackup" @change="handleSwitchChange('autoBackup', $event)" color="#1e3c72" />
          </view>
          <view class="setting-item" @tap="clearCache">
            <text>清除缓存</text>
            <text class="cache-size">{{ cacheSize }}</text>
          </view>
          <view class="setting-item" @tap="checkUpdate">
            <text>检查更新</text>
            <text class="version">当前版本 1.0.0</text>
          </view>
        </view>
      </view>
    </view>
  </view>
</template>

<script setup>
import { ref } from 'vue'

const settings = ref({
  deviceAlert: true,
  systemNotice: true,
  fingerprint: false,
  faceId: false,
  darkMode: false,
  themeColor: '#1e3c72',
  autoBackup: true
})

const cacheSize = ref('23.5MB')

const themeColors = [
  '#1e3c72',
  '#2ecc71',
  '#e74c3c',
  '#f39c12',
  '#9b59b6'
]

// 处理开关变化
const handleSwitchChange = (key, event) => {
  settings.value[key] = event.detail.value
  // 保存设置
  saveSettings()
}

// 处理主题色变化
const handleThemeChange = (color) => {
  settings.value.themeColor = color
  // 保存设置
  saveSettings()
}

// 保存设置
const saveSettings = () => {
  uni.showToast({
    title: '设置已保存',
    icon: 'success'
  })
}

// 清除缓存
const clearCache = () => {
  uni.showModal({
    title: '提示',
    content: '确定要清除缓存吗？',
    success: (res) => {
      if (res.confirm) {
        uni.showLoading({ title: '清理中' })
        setTimeout(() => {
          uni.hideLoading()
          cacheSize.value = '0KB'
          uni.showToast({
            title: '清除成功',
            icon: 'success'
          })
        }, 1500)
      }
    }
  })
}

// 检查更新
const checkUpdate = () => {
  uni.showLoading({ title: '检查更新中' })
  setTimeout(() => {
    uni.hideLoading()
    uni.showToast({
      title: '已是最新版本',
      icon: 'none'
    })
  }, 1500)
}

// 页面跳转
const navigateTo = (page) => {
  uni.navigateTo({
    url: `/pages/settings/${page}`
  })
}
</script>

<style lang="scss" scoped>
.system-container {
  padding: 30rpx;
  min-height: 100vh;
  background: linear-gradient(to bottom, #1e3c72 0%, #2a5298 100%);
}

.info-card {
  background-color: rgba(255, 255, 255, 0.98);
  border-radius: 30rpx;
  padding: 40rpx 30rpx;
  box-shadow: 0 10rpx 30rpx rgba(0, 0, 0, 0.2);
  position: relative;
  overflow: hidden;
}

.bg-decoration {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  height: 200rpx;
  background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);
  opacity: 0.1;
  z-index: 0;
}

.card-header {
  position: relative;
  text-align: center;
  margin-bottom: 40rpx;
  z-index: 1;
}

.title {
  font-size: 40rpx;
  font-weight: 600;
  color: #1e3c72;
  letter-spacing: 4rpx;
}

.settings-list {
  position: relative;
  z-index: 1;
}

.settings-group {
  margin-bottom: 40rpx;
  background: rgba(248, 250, 252, 0.8);
  border-radius: 20rpx;
  padding: 30rpx;
}

.group-title {
  display: flex;
  align-items: center;
  margin-bottom: 20rpx;
  font-size: 32rpx;
  color: #1e3c72;
  font-weight: 500;
}

.group-title .iconfont {
  margin-right: 16rpx;
  font-size: 36rpx;
}

.setting-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20rpx 0;
  font-size: 28rpx;
  color: #333;
  border-bottom: 2rpx solid rgba(0, 0, 0, 0.05);
}

.setting-item:last-child {
  border-bottom: none;
}

.arrow {
  color: #999;
  font-size: 24rpx;
}

.cache-size, .version {
  font-size: 24rpx;
  color: #999;
}

.theme-colors {
  display: flex;
  justify-content: space-around;
  padding: 20rpx 0;
}

.color-item {
  width: 60rpx;
  height: 60rpx;
  border-radius: 30rpx;
  border: 4rpx solid transparent;
  transition: all 0.3s ease;
}

.color-item.active {
  border-color: #fff;
  box-shadow: 0 0 0 4rpx currentColor;
  transform: scale(1.1);
}

/* 添加动画效果 */
@keyframes fadeInUp {
  from {
    opacity: 0;
    transform: translateY(20rpx);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}

.settings-group {
  animation: fadeInUp 0.5s ease forwards;
}

.settings-group:nth-child(1) { animation-delay: 0.1s; }
.settings-group:nth-child(2) { animation-delay: 0.2s; }
.settings-group:nth-child(3) { animation-delay: 0.3s; }
.settings-group:nth-child(4) { animation-delay: 0.4s; }
</style>
