<template>
  <view class="help-container">
    <view class="info-card">
      <view class="bg-decoration"></view>
      
      <view class="card-header">
        <text class="title">帮助中心</text>
      </view>

      <!-- 搜索框 -->
      <view class="search-box">
        <text class="iconfont">🔍</text>
        <input 
          type="text" 
          v-model="searchText"
          placeholder="搜索问题"
          class="search-input"
        />
      </view>

      <!-- 常见问题分类 -->
      <view class="help-sections">
        <!-- 设备使用 -->
        <view class="help-section">
          <view class="section-header">
            <text class="iconfont">📱</text>
            <text class="section-title">设备使用</text>
          </view>
          <view class="qa-list">
            <view 
              class="qa-item"
              v-for="(item, index) in deviceQuestions"
              :key="index"
              @tap="showAnswer(item)"
            >
              <view class="question">
                <text class="q-text">{{ item.question }}</text>
                <text class="iconfont arrow" :class="{ 'arrow-down': item.isOpen }">></text>
              </view>
              <view class="answer" v-if="item.isOpen">
                {{ item.answer }}
              </view>
            </view>
          </view>
        </view>

        <!-- 账号安全 -->
        <view class="help-section">
          <view class="section-header">
            <text class="iconfont">🔒</text>
            <text class="section-title">账号安全</text>
          </view>
          <view class="qa-list">
            <view 
              class="qa-item"
              v-for="(item, index) in accountQuestions"
              :key="index"
              @tap="showAnswer(item)"
            >
              <view class="question">
                <text class="q-text">{{ item.question }}</text>
                <text class="iconfont arrow" :class="{ 'arrow-down': item.isOpen }">></text>
              </view>
              <view class="answer" v-if="item.isOpen">
                {{ item.answer }}
              </view>
            </view>
          </view>
        </view>

        <!-- 系统设置 -->
        <view class="help-section">
          <view class="section-header">
            <text class="iconfont">⚙️</text>
            <text class="section-title">系统设置</text>
          </view>
          <view class="qa-list">
            <view 
              class="qa-item"
              v-for="(item, index) in systemQuestions"
              :key="index"
              @tap="showAnswer(item)"
            >
              <view class="question">
                <text class="q-text">{{ item.question }}</text>
                <text class="iconfont arrow" :class="{ 'arrow-down': item.isOpen }">></text>
              </view>
              <view class="answer" v-if="item.isOpen">
                {{ item.answer }}
              </view>
            </view>
          </view>
        </view>
      </view>

      <!-- 联系客服 -->
      <view class="contact-support">
        <button class="support-btn" @tap="contactSupport">
          联系客服
        </button>
      </view>
    </view>
  </view>
</template>

<script setup>
import { ref } from 'vue'

const searchText = ref('')

// 设备使用问题
const deviceQuestions = ref([
  {
    question: '如何添加新设备？',
    answer: '1. 点击首页"+"按钮\n2. 选择设备类型\n3. 按照提示进行配网\n4. 等待设备连接成功',
    isOpen: false
  },
  {
    question: '设备离线怎么办？',
    answer: '1. 检查设备电源\n2. 确认WiFi网络正常\n3. 尝试重启设备\n4. 如果问题持续，请重新配网',
    isOpen: false
  },
  {
    question: '如何分享设备给家人？',
    answer: '1. 进入设备详情页\n2. 点击分享按钮\n3. 选择分享方式\n4. 输入对方账号完成分享',
    isOpen: false
  }
])

// 账号安全问题
const accountQuestions = ref([
  {
    question: '忘记密码怎么办？',
    answer: '可以通过手机号验证码或邮箱验证的方式重置密码。进入登录页面，点击"忘记密码"按照提示操作即可。',
    isOpen: false
  },
  {
    question: '如何修改手机号？',
    answer: '进入"个人资料"页面，点击手机号码，验证身份后即可修改绑定的手机号。',
    isOpen: false
  }
])

// 系统设置问题
const systemQuestions = ref([
  {
    question: '如何设置设备自动化？',
    answer: '1. 进入"自动化"页面\n2. 点击"添加自动化"\n3. 设置触发条件和执行动作\n4. 保存即可',
    isOpen: false
  },
  {
    question: '如何备份设备配置？',
    answer: '在"系统设置"中开启"自动备份"功能，系统会定期自动备份您的设备配置到云端。',
    isOpen: false
  }
])

// 展示答案
const showAnswer = (item) => {
  item.isOpen = !item.isOpen
}

// 联系客服
const contactSupport = () => {
  // uni.navigateTo({
  //   url: '/pages/contact/support'
  // })
}
</script>

<style lang="scss" scoped>
.help-container {
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

.search-box {
  position: relative;
  margin-bottom: 40rpx;
  z-index: 1;
}

.search-input {
  width: 100%;
  height: 80rpx;
  background: #f8fafc;
  border-radius: 40rpx;
  padding: 0 80rpx;
  font-size: 28rpx;
  border: 2rpx solid #e8e8e8;
}

.search-box .iconfont {
  position: absolute;
  left: 30rpx;
  top: 50%;
  transform: translateY(-50%);
  font-size: 32rpx;
  color: #999;
}

.help-sections {
  position: relative;
  z-index: 1;
}

.help-section {
  margin-bottom: 40rpx;
  animation: fadeInUp 0.5s ease forwards;
}

.section-header {
  display: flex;
  align-items: center;
  margin-bottom: 20rpx;
}

.section-header .iconfont {
  font-size: 36rpx;
  margin-right: 16rpx;
}

.section-title {
  font-size: 32rpx;
  color: #1e3c72;
  font-weight: 500;
}

.qa-list {
  background: rgba(248, 250, 252, 0.8);
  border-radius: 20rpx;
  overflow: hidden;
}

.qa-item {
  border-bottom: 2rpx solid rgba(0, 0, 0, 0.05);
}

.qa-item:last-child {
  border-bottom: none;
}

.question {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 30rpx;
  transition: all 0.3s ease;
}

.q-text {
  font-size: 28rpx;
  color: #333;
  flex: 1;
}

.arrow {
  font-size: 24rpx;
  color: #999;
  transition: all 0.3s ease;
}

.arrow-down {
  transform: rotate(90deg);
}

.answer {
  padding: 0 30rpx 30rpx;
  font-size: 26rpx;
  color: #666;
  line-height: 1.6;
  background: rgba(248, 250, 252, 0.9);
}

.contact-support {
  margin-top: 60rpx;
  text-align: center;
}

.support-btn {
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

.support-btn:active {
  transform: scale(0.95);
  opacity: 0.9;
}

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

.help-section:nth-child(1) { animation-delay: 0.1s; }
.help-section:nth-child(2) { animation-delay: 0.2s; }
.help-section:nth-child(3) { animation-delay: 0.3s; }
</style>
