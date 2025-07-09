<template>
  <view class="message-container">
    <view class="info-card">
      <view class="bg-decoration"></view>
      
      <view class="card-header">
        <text class="title">消息中心</text>
      </view>

      <!-- 消息类型选择 -->
      <view class="message-tabs">
        <view 
          class="tab-item" 
          :class="{ active: activeTab === 'device' }"
          @tap="activeTab = 'device'"
        >
          设备通知
        </view>
        <view 
          class="tab-item" 
          :class="{ active: activeTab === 'system' }"
          @tap="activeTab = 'system'"
        >
          系统消息
        </view>
      </view>

      <!-- 消息列表 -->
      <scroll-view 
        class="message-list"
        scroll-y
        @scrolltolower="loadMore"
      >
        <view 
          class="message-item"
          v-for="(item, index) in filteredMessages"
          :key="index"
          @tap="handleMessageClick(item)"
        >
          <view class="message-icon" :class="item.type">
            <text class="iconfont">{{ getMessageIcon(item.type) }}</text>
          </view>
          <view class="message-content">
            <view class="message-header">
              <text class="message-title">{{ item.title }}</text>
              <text class="message-time">{{ item.time }}</text>
            </view>
            <text class="message-desc">{{ item.content }}</text>
          </view>
          <view class="unread-dot" v-if="!item.isRead"></view>
        </view>

        <!-- 加载更多 -->
        <view class="load-more" v-if="hasMore">
          <text>加载中...</text>
        </view>
      </scroll-view>

      <!-- 空状态 -->
      <view class="empty-state" v-if="filteredMessages.length === 0">
        <text class="empty-icon">📭</text>
        <text class="empty-text">暂无消息</text>
      </view>
    </view>
  </view>
</template>

<script setup>
import { ref, computed } from 'vue'

const activeTab = ref('device')
const hasMore = ref(true)

// 修改为智能家居相关的消息数据
const messages = ref([
  {
    type: 'warning',
    title: '空调温度异常',
    content: '客厅空调温度过高，建议及时检查',
    time: '10分钟前',
    isRead: false,
    category: 'device'
  },
  {
    type: 'info',
    title: '门锁电量提醒',
    content: '前门智能门锁电量低于20%，请及时更换电池',
    time: '30分钟前',
    isRead: true,
    category: 'device'
  },
  {
    type: 'success',
    title: '设备连接成功',
    content: '新增智能窗帘已成功接入系统',
    time: '2小时前',
    isRead: true,
    category: 'device'
  },
  {
    type: 'error',
    title: '安全警报',
    content: '检测到后门异常开启，请及时确认',
    time: '昨天',
    isRead: false,
    category: 'device'
  },
  {
    type: 'info',
    title: '系统更新提醒',
    content: '新版本V1.2.0已发布，新增多项功能',
    time: '2天前',
    isRead: true,
    category: 'system'
  },
  {
    type: 'success',
    title: '备份完成',
    content: '设备配置已自动备份至云端',
    time: '3天前',
    isRead: true,
    category: 'system'
  }
])

// 根据消息类型返回对应图标
const getMessageIcon = (type) => {
  const icons = {
    warning: '⚠️',
    info: '💡',
    success: '✅',
    error: '🚨'
  }
  return icons[type] || '💡'
}

// 根据当前选中的标签过滤消息
const filteredMessages = computed(() => {
  return messages.value.filter(msg => msg.category === activeTab.value)
})

// 处理消息点击
const handleMessageClick = (message) => {
  message.isRead = true
  // 根据消息类型跳转到相应页面
  if (message.category === 'device') {
    uni.navigateTo({
      url: `/pages/device/detail?id=${message.deviceId}`
    })
  } else {
    uni.showToast({
      title: message.title,
      icon: 'none'
    })
  }
}

// 加载更多消息
const loadMore = () => {
  if (!hasMore.value) return
  // 模拟加载更多数据
  setTimeout(() => {
    // 这里可以添加更多模拟数据
    hasMore.value = false
  }, 1000)
}
</script>

<style lang="scss" scoped>
.message-container {
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
  min-height: 90vh;
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

.message-tabs {
  display: flex;
  justify-content: space-around;
  margin-bottom: 30rpx;
  position: relative;
  z-index: 1;
}

.tab-item {
  padding: 20rpx 40rpx;
  font-size: 28rpx;
  color: #666;
  position: relative;
  transition: all 0.3s ease;
}

.tab-item.active {
  color: #1e3c72;
  font-weight: 500;
}

.tab-item.active::after {
  content: '';
  position: absolute;
  bottom: 0;
  left: 50%;
  transform: translateX(-50%);
  width: 60rpx;
  height: 4rpx;
  background: #1e3c72;
  border-radius: 2rpx;
}

.message-list {
  height: calc(100vh - 400rpx);
  position: relative;
  z-index: 1;
}

.message-item {
  display: flex;
  padding: 30rpx;
  background: rgba(248, 250, 252, 0.8);
  border-radius: 20rpx;
  margin-bottom: 20rpx;
  position: relative;
  transition: all 0.3s ease;
}

.message-item:active {
  transform: scale(0.98);
  background: rgba(248, 250, 252, 0.9);
}

.message-icon {
  width: 80rpx;
  height: 80rpx;
  border-radius: 40rpx;
  display: flex;
  align-items: center;
  justify-content: center;
  margin-right: 20rpx;
}

.message-icon.warning { 
  background: rgba(255, 152, 0, 0.1); 
  color: #ff9800;
}
.message-icon.info { 
  background: rgba(33, 150, 243, 0.1); 
  color: #2196f3;
}
.message-icon.success { 
  background: rgba(76, 175, 80, 0.1); 
  color: #4caf50;
}
.message-icon.error { 
  background: rgba(244, 67, 54, 0.1); 
  color: #f44336;
}

.message-content {
  flex: 1;
}

.message-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 10rpx;
}

.message-title {
  font-size: 30rpx;
  color: #333;
  font-weight: 500;
}

.message-time {
  font-size: 24rpx;
  color: #999;
}

.message-desc {
  font-size: 26rpx;
  color: #666;
  line-height: 1.5;
}

.unread-dot {
  position: absolute;
  top: 30rpx;
  right: 30rpx;
  width: 16rpx;
  height: 16rpx;
  background: #ff4d4f;
  border-radius: 50%;
}

.load-more {
  text-align: center;
  padding: 20rpx 0;
  color: #999;
  font-size: 26rpx;
}

.empty-state {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 100rpx 0;
}

.empty-icon {
  font-size: 80rpx;
  margin-bottom: 20rpx;
}

.empty-text {
  color: #999;
  font-size: 28rpx;
}

/* 添加消息出现动画 */
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

.message-item {
  animation: fadeInUp 0.5s ease forwards;
}

.message-item:nth-child(1) { animation-delay: 0.1s; }
.message-item:nth-child(2) { animation-delay: 0.2s; }
.message-item:nth-child(3) { animation-delay: 0.3s; }
.message-item:nth-child(4) { animation-delay: 0.4s; }
</style>
