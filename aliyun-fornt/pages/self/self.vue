<template>
  <view class="self-container">
    <!-- 顶部个人信息卡片 -->
    <view class="profile-card">
      <view class="profile-header">
        <view class="avatar"></view>
        <view class="basic-info">
          <text class="nickname">123234</text>
          <text class="status">在线</text>
        </view>
      </view>
    </view>

    <!-- 功能列表 -->
    <view class="menu-list">
      <view class="menu-group">
		  <navigator url="">
        <view class="menu-item" @click="selfinfo">
          <view class="item-left">
            <uni-icons type="person" size="20" color="#666"></uni-icons>
            <text>个人资料</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
		</navigator>
        <view class="menu-item" @click="iphone">
          <view class="item-left">
            <uni-icons type="phone" size="20" color="#666"></uni-icons>
            <text>联系方式</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
      </view>

      <view class="menu-group">
        <view class="menu-item" @click="xiaoxi">
          <view class="item-left">
            <uni-icons type="notification" size="20" color="#666"></uni-icons>
            <text>消息通知</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
        <view class="menu-item" @click="system">
          <view class="item-left">
            <uni-icons type="gear" size="20" color="#666"></uni-icons>
            <text>系统设置</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
      </view>

      <view class="menu-group">

        <view class="menu-item" @click="help">
          <view class="item-left">
            <uni-icons type="help" size="20" color="#666"></uni-icons>
            <text>帮助中心</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
        <view class="menu-item" @click="about">
          <view class="item-left">
            <uni-icons type="info" size="20" color="#666"></uni-icons>
            <text>关于我们</text>
          </view>
          <uni-icons type="right" size="16" color="#999"></uni-icons>
        </view>
      </view>
    </view>

    <!-- 退出按钮 -->
    <view class="logout-section">
      <button class="logout-btn" @click="outlogin()">退出登录</button>
    </view>
	</view>
</template>

<style lang="scss" scoped>

// .self-container {
//   min-height: 100vh;
//   background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
//   padding-bottom: 120rpx;
// }
.self-container {
 min-height: 100vh;
 background-image: linear-gradient(to bottom, #2695de, #9ce4fe);
 // background-color: #fff;
 // background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
 padding: 30rpx;
 padding-bottom: 120rpx;
}

.profile-card {
  background: rgba(255, 255, 255, 0.1);
  backdrop-filter: blur(10px);
  padding: 60rpx 40rpx;
  color: #fff;
}

.profile-header {
  display: flex;
  align-items: center;
  gap: 30rpx;
}

.avatar {
  width: 120rpx;
  height: 120rpx;
  border-radius: 50%;
  background: #ffd700;
  border: 4rpx solid rgba(255,255,255,0.4);
  box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.2);
}

.basic-info {
  display: flex;
  flex-direction: column;
  gap: 8rpx;
}

.nickname {
  font-size: 36rpx;
  font-weight: 500;
}

.status {
  font-size: 24rpx;
  opacity: 0.8;
  background: rgba(255,255,255,0.2);
  padding: 4rpx 16rpx;
  border-radius: 20rpx;
  width: fit-content;
}

.menu-list {
  padding: 20rpx;
}

.menu-group {
  background: rgba(255, 255, 255, 0.95);
  border-radius: 16rpx;
  margin-bottom: 20rpx;
  overflow: hidden;
  box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.1);
}

.menu-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 30rpx;
  border-bottom: 1px solid rgba(0,0,0,0.05);
}

.menu-item:last-child {
  border-bottom: none;
}

.item-left {
  display: flex;
  align-items: center;
  gap: 20rpx;
  color: #333;
  font-size: 28rpx;
}

.logout-section {
  padding: 40rpx;
}

.logout-btn {
  background: rgba(255, 255, 255, 0.95);
  color: #ff4d4f;
  border: none;
  border-radius: 12rpx;
  height: 88rpx;
  line-height: 88rpx;
  font-size: 28rpx;
  box-shadow: 0 4rpx 12rpx rgba(0,0,0,0.1);
}

.tab-bar {
  position: fixed;
  bottom: 0;
  left: 0;
  right: 0;
  background: rgba(255,255,255,0.95);
  backdrop-filter: blur(10px);
  display: flex;
  padding: 16rpx 0;
  border-top: 1rpx solid rgba(0,0,0,0.1);
}

.tab-item {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  font-size: 24rpx;
  color: #666;
  padding: 10rpx 0;
}

.tab-item.active {
  color: #1e4c88;
  position: relative;
}

.tab-item.active::after {
  content: '';
  position: absolute;
  bottom: -16rpx;
  left: 50%;
  transform: translateX(-50%);
  width: 40rpx;
  height: 4rpx;
  background: #1e4c88;
  border-radius: 2rpx;
}
</style>

<script setup>
import { ref } from 'vue'
function outlogin(){
	uni.setStorageSync('autoLogin',false);
	
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../index/index'
	});

}
function about(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/about'
	});

}
function help(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/help'
	});

}
function iphone(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/iphone'
	});

}
function selfinfo(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/selfinfo'
	});

}
function system(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/system'
	});

}
function xiaoxi(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../self/xiaoxi'
	});

}
function dvmskvm(){
	//在起始页面跳转到test.vue页面并传递参数
	uni.navigateTo({
		url: '../controller/dvmskvm/dvmskvm'
	});

}

</script>
