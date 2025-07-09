<template>
  <view class="selfinfo-container">
    <view class="info-card">
      <view class="bg-decoration"></view>
      
      <view class="card-header">
        <text class="title">个人资料</text>
      </view>
      
      <view class="avatar-section">
        <view class="avatar-wrapper">
<!--          <image 
            class="avatar" 
            :src="userInfo.avatar || '/static/default.png'" 
            mode="aspectFill"
          /> -->
		  <image
		    class="avatar" 
		    src="../../static/test0.jpg" 
		    mode="aspectFill"
		  />
          <view v-if="isEditing" class="avatar-overlay" @tap="chooseImage">
            <text class="iconfont icon-camera"></text>
          </view>
        </view>
      </view>

      <view class="form-container">
        <view class="form-item">
          <text class="label">用户名</text>
          <input 
            class="input" 
            v-model="userInfo.username" 
            :disabled="!isEditing" 
            placeholder="请输入用户名"
          />
        </view>
        <view class="form-item">
          <text class="label">昵称</text>
          <input 
            class="input" 
            v-model="userInfo.nickname" 
            :disabled="!isEditing"
            placeholder="请输入昵称"
          />
        </view>
        <view class="form-item">
          <text class="label">手机号</text>
          <input 
            class="input" 
            v-model="userInfo.phone" 
            :disabled="!isEditing"
            placeholder="请输入手机号"
          />
        </view>
        <view class="form-item">
          <text class="label">邮箱</text>
          <input 
            class="input" 
            v-model="userInfo.email" 
            :disabled="!isEditing"
            placeholder="请输入邮箱"
          />
        </view>
        <view class="form-item">
          <text class="label">个人简介</text>
          <text class="textarea">介绍一下自己吧...</text>
<!--          <text 
            class="textarea" 
            v-model="userInfo.bio" 
            :disabled="!isEditing"
            placeholder="介绍一下自己吧..."
          /> -->
        </view>
      </view>

      <view class="button-container">
        <button 
          class="action-btn" 
          :class="isEditing ? 'save-btn' : 'edit-btn'" 
          @tap="isEditing ? handleSave() : handleEdit()"
        >
          {{ isEditing ? '保存' : '编辑' }}
        </button>
      </view>
    </view>
  </view>
</template>

<script setup>
import { ref, onMounted } from 'vue'

const isEditing = ref(false)
const userInfo = ref({
  username: '',
  nickname: '',
  phone: '',
  email: '',
  bio: '',
  avatar: '../../static/default.png'
})

// 获取用户信息
const getUserInfo = async () => {
  try {
    const result = await uni.request({
      url: '/api/user/info',
      method: 'GET'
    })
    if (result.data) {
      userInfo.value = result.data
    }
  } catch (error) {
    uni.showToast({
      title: '获取用户信息失败',
      icon: 'none'
    })
  }
}

// 处理编辑按钮
const handleEdit = () => {
  isEditing.value = true
}

// 处理保存按钮
const handleSave = async () => {
  try {
    await uni.request({
      url: '/api/user/update',
      method: 'POST',
      data: userInfo.value
    })
    isEditing.value = false
    uni.showToast({
      title: '保存成功',
      icon: 'success'
    })
  } catch (error) {
    uni.showToast({
      title: '保存失败',
      icon: 'none'
    })
  }
}

// 选择并上传图片
const chooseImage = () => {
  uni.chooseImage({
    count: 1,
    success: async (chooseImageRes) => {
      const tempFilePaths = chooseImageRes.tempFilePaths
      
      uni.uploadFile({
        url: '/api/upload',
        filePath: tempFilePaths[0],
        name: 'file',
        success: (uploadFileRes) => {
          const data = JSON.parse(uploadFileRes.data)
          userInfo.value.avatar = data.url
          uni.showToast({
            title: '上传成功',
            icon: 'success'
          })
        },
        fail: () => {
          uni.showToast({
            title: '上传失败',
            icon: 'none'
          })
        }
      })
    }
  })
}

onMounted(() => {
  // getUserInfo()
})
</script>

<style lang="scss" scoped>
.selfinfo-container {
  padding: 30rpx;
  min-height: 100vh;
  background: linear-gradient(to bottom, #1e3c72 0%, #2a5298 100%);
}

.info-card {
  background-color: #ffffff;
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

.avatar-section {
  position: relative;
  text-align: center;
  margin-bottom: 60rpx;
  z-index: 1;
}

.avatar-wrapper {
  position: relative;
  display: inline-block;
}

.avatar {
  width: 180rpx;
  height: 180rpx;
  border-radius: 90rpx;
  border: 6rpx solid #ffffff;
  box-shadow: 0 8rpx 20rpx rgba(30, 60, 114, 0.2);
}

.avatar-overlay {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(30, 60, 114, 0.6);
  border-radius: 90rpx;
  display: flex;
  justify-content: center;
  align-items: center;
  opacity: 0;
  transition: opacity 0.3s ease;
}

.avatar-wrapper:active .avatar-overlay {
  opacity: 1;
}

.icon-camera {
  color: #ffffff;
  font-size: 48rpx;
}

.form-container {
  position: relative;
  z-index: 1;
  margin-bottom: 60rpx;
}

.form-item {
  margin-bottom: 40rpx;
}

.label {
  display: block;
  margin-bottom: 16rpx;
  font-size: 28rpx;
  color: #1e3c72;
  font-weight: 500;
}

.input {
  width: 100%;
  height: 80rpx;
  border: 2rpx solid #e8e8e8;
  border-radius: 16rpx;
  padding: 0 30rpx;
  font-size: 28rpx;
  background: #f8fafc;
  transition: all 0.3s ease;
}

.input:focus {
  border-color: #1e3c72;
  background: #ffffff;
  box-shadow: 0 0 10rpx rgba(30, 60, 114, 0.1);
}

.textarea {
  width: 100%;
  height: 200rpx;
  border: 2rpx solid #e8e8e8;
  border-radius: 16rpx;
  padding: 20rpx 30rpx;
  font-size: 28rpx;
  background: #f8fafc;
  transition: all 0.3s ease;
}

.textarea:focus {
  border-color: #1e3c72;
  background: #ffffff;
  box-shadow: 0 0 10rpx rgba(30, 60, 114, 0.1);
}

.input:disabled, .textarea:disabled {
  background-color: #f8fafc;
  color: #909399;
  cursor: not-allowed;
  opacity: 0.8;
}

.button-container {
  text-align: center;
  margin-top: 40rpx;
}

.action-btn {
  width: 80%;
  height: 90rpx;
  line-height: 90rpx;
  border: none;
  border-radius: 45rpx;
  font-size: 32rpx;
  font-weight: 500;
  transition: all 0.3s ease;
  color: #ffffff;
}

.edit-btn {
  background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);
  box-shadow: 0 8rpx 20rpx rgba(30, 60, 114, 0.3);
}

.save-btn {
  background: linear-gradient(135deg, #16a085 0%, #2ecc71 100%);
  box-shadow: 0 8rpx 20rpx rgba(46, 204, 113, 0.3);
}

/* 添加输入框激活状态的动画效果 */
@keyframes inputFocus {
  from {
    transform: scale(1);
  }
  to {
    transform: scale(1.02);
  }
}

.input:not(:disabled):focus,
.textarea:not(:disabled):focus {
  animation: inputFocus 0.3s ease;
}

/* 添加按钮点击效果 */
.action-btn:active {
  transform: scale(0.95);
  opacity: 0.9;
}

/* 添加深色主题相关样式 */
.info-card {
  background-color: rgba(255, 255, 255, 0.98);
}

.input, .textarea {
  background: rgba(248, 250, 252, 0.8);
}

.input:focus, .textarea:focus {
  background: rgba(255, 255, 255, 0.95);
}
</style>
