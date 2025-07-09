<template>
  <view class="contact-container">
    <view class="info-card">
      <view class="bg-decoration"></view>
      
      <view class="card-header">
        <text class="title">联系我们</text>
      </view>

      <view class="content-section">
        <!-- 联系电话 -->
        <view class="contact-item">
          <view class="item-header">
            <text class="iconfont">📞</text>
            <text class="item-title">客服电话</text>
          </view>
          <view class="item-content" @tap="makePhoneCall">
            <text class="value">400-123-4567</text>
            <text class="hint">点击拨打</text>
          </view>
        </view>

        <!-- 工作时间 -->
        <view class="contact-item">
          <view class="item-header">
            <text class="iconfont">⏰</text>
            <text class="item-title">工作时间</text>
          </view>
          <view class="item-content">
            <text class="value">周一至周五</text>
            <text class="sub-value">09:00 - 18:00</text>
          </view>
        </view>

        <!-- 电子邮箱 -->
        <view class="contact-item">
          <view class="item-header">
            <text class="iconfont">📧</text>
            <text class="item-title">电子邮箱</text>
          </view>
          <view class="item-content">
            <text class="value">support@example.com</text>
          </view>
        </view>

        <!-- 公司地址 -->
        <view class="contact-item">
          <view class="item-header">
            <text class="iconfont">📍</text>
            <text class="item-title">公司地址</text>
          </view>
          <view class="item-content">
            <text class="value">河北省石家庄市某某区</text>
            <text class="sub-value">某某街123号</text>
          </view>
        </view>
      </view>

      <!-- 快速联系按钮 -->
      <view class="quick-contact">
        <button class="contact-btn" @tap="makePhoneCall">
          立即联系
        </button>
      </view>
    </view>
  </view>
</template>

<script setup>
const makePhoneCall = () => {
  uni.makePhoneCall({
    phoneNumber: '4001234567',
    fail: () => {
      uni.showToast({
        title: '拨打失败',
        icon: 'none'
      })
    }
  })
}
</script>

<style lang="scss" scoped>
.contact-container {
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
  margin-bottom: 60rpx;
  z-index: 1;
}

.title {
  font-size: 40rpx;
  font-weight: 600;
  color: #1e3c72;
  letter-spacing: 4rpx;
}

.content-section {
  position: relative;
  z-index: 1;
}

.contact-item {
  background: rgba(248, 250, 252, 0.8);
  border-radius: 20rpx;
  padding: 30rpx;
  margin-bottom: 30rpx;
  transition: all 0.3s ease;
}

.contact-item:active {
  transform: scale(0.98);
  background: rgba(248, 250, 252, 0.9);
}

.item-header {
  display: flex;
  align-items: center;
  margin-bottom: 20rpx;
}

.iconfont {
  font-size: 40rpx;
  margin-right: 20rpx;
}

.item-title {
  font-size: 32rpx;
  color: #1e3c72;
  font-weight: 500;
}

.item-content {
  padding-left: 60rpx;
}

.value {
  font-size: 32rpx;
  color: #2c3e50;
  display: block;
}

.sub-value {
  font-size: 28rpx;
  color: #666;
  display: block;
  margin-top: 10rpx;
}

.hint {
  font-size: 24rpx;
  color: #1e3c72;
  margin-top: 10rpx;
  display: block;
}

.quick-contact {
  margin-top: 60rpx;
  text-align: center;
}

.contact-btn {
  width: 80%;
  height: 90rpx;
  line-height: 90rpx;
  border: none;
  border-radius: 45rpx;
  font-size: 32rpx;
  font-weight: 500;
  color: #ffffff;
  background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);
  box-shadow: 0 8rpx 20rpx rgba(30, 60, 114, 0.3);
  transition: all 0.3s ease;
}

.contact-btn:active {
  transform: scale(0.95);
  opacity: 0.9;
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

.contact-item {
  animation: fadeInUp 0.5s ease forwards;
}

.contact-item:nth-child(1) { animation-delay: 0.1s; }
.contact-item:nth-child(2) { animation-delay: 0.2s; }
.contact-item:nth-child(3) { animation-delay: 0.3s; }
.contact-item:nth-child(4) { animation-delay: 0.4s; }
</style>
