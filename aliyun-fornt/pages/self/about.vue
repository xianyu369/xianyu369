<template>
  <view class="about-container">
    <!-- 顶部标题栏 -->

    <!-- 系统信息 -->
    <view class="content">
      <view class="logo-box">
        <image class="logo" src="/static/logo.png" mode="aspectFit"></image>
        <text class="app-name">智能家居管理</text>
        <text class="version">版本号：1.0.0</text>
      </view>

      <!-- 系统介绍 -->
      <view class="info-section">
        <view class="section-title">
          <uni-icons type="info" size="20" color="#1e4c88"></uni-icons>
          <text>系统简介</text>
        </view>
        <view class="section-content">
          <text class="desc">本系统是专门为智能家居开发的智能管理平台，集成了设备监控、数据分析等功能，旨在提高管理水平。</text>
        </view>
      </view>

      <!-- 主要功能 -->
      <view class="info-section">
        <view class="section-title">
          <uni-icons type="star" size="20" color="#1e4c88"></uni-icons>
          <text>主要功能</text>
        </view>
        <view class="feature-list">
          <view class="feature-item">
            <text>天气监测</text>
          </view>
          <view class="feature-item">
            <text>设备控制</text>
          </view>
          <view class="feature-item">
            <text>数据统计</text>
          </view>
        </view>
      </view>

      <!-- 联系方式 -->
      <view class="info-section">
        <view class="section-title">
          <uni-icons type="phone" size="20" color="#1e4c88"></uni-icons>
          <text>联系我们</text>
        </view>
        <view class="contact-list">
          <view class="contact-item">
            <text class="label">电话：</text>
            <text class="value">400-123-4567</text>
          </view>
          <view class="contact-item">
            <text class="label">邮箱：</text>
            <text class="value">support@example.com</text>
          </view>
          <view class="contact-item">
            <text class="label">地址：</text>
            <text class="value">河北省石家庄市某某区某某街123号</text>
          </view>
        </view>
      </view>
    </view>
  </view>
</template>

<script setup>
const goBack = () => {
  uni.navigateBack()
}
</script>

<style lang="scss" scoped>
.about-container {
  min-height: 100vh;
  background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
}

.header {
  display: flex;
  align-items: center;
  padding: 40rpx 30rpx;
  background: rgba(255,255,255,0.1);
  backdrop-filter: blur(10px);
}

.back-btn {
  padding: 10rpx;
}

.title {
  flex: 1;
  text-align: center;
  color: #fff;
  font-size: 36rpx;
  font-weight: 600;
  margin-right: 40rpx;
}

.content {
  padding: 30rpx;
}

.logo-box {
  display: flex;
  flex-direction: column;
  align-items: center;
  padding: 40rpx 0;
}

.logo {
  width: 160rpx;
  height: 160rpx;
  margin-bottom: 20rpx;
}

.app-name {
  color: #fff;
  font-size: 36rpx;
  font-weight: 600;
  margin-bottom: 10rpx;
}

.version {
  color: rgba(255,255,255,0.8);
  font-size: 26rpx;
}

.info-section {
  background: rgba(255,255,255,0.95);
  border-radius: 16rpx;
  padding: 30rpx;
  margin-bottom: 30rpx;
}

.section-title {
  display: flex;
  align-items: center;
  gap: 16rpx;
  font-size: 32rpx;
  font-weight: 600;
  color: #1e4c88;
  margin-bottom: 20rpx;
}

.desc {
  font-size: 28rpx;
  color: #333;
  line-height: 1.6;
}

.feature-list {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 20rpx;
}

.feature-item {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10rpx;
  padding: 20rpx;
  background: #f8f9fa;
  border-radius: 12rpx;
}

.feature-item text {
  font-size: 26rpx;
  color: #333;
}

.contact-list {
  display: flex;
  flex-direction: column;
  gap: 16rpx;
}

.contact-item {
  display: flex;
  font-size: 28rpx;
}

.contact-item .label {
  color: #666;
  width: 100rpx;
}

.contact-item .value {
  color: #333;
  flex: 1;
}
</style>
