<template>
	<view class="container">
		<!-- 顶部装饰波浪 -->
		<view class="wave-box">
			<view class="wave"></view>
		</view>

		<!-- 标题区域 -->
		<view class="header">
			<image class="logo" src="/static/Edge Devices.png" mode="aspectFit"></image>
			<text class="title">智能家居管理</text>
		</view>

		<!-- 表单区域 -->
		<view class="form-box">
			<!-- 切换标签 -->
			<view class="tab-box">
				<text class="tab-item" :class="{ active: activeTab === 'login' }" @click="activeTab = 'login'">登录</text>
				<text class="tab-item" :class="{ active: activeTab === 'register' }"
					@click="activeTab = 'register'">注册</text>
			</view>

			<!-- 登录表单 -->
			<view class="form" v-show="activeTab === 'login'">
				<view class="input-group">
					<uni-icons type="person" size="20" color="#666"></uni-icons>
					<input class="input" type="text" v-model="loginForm.username" placeholder="请输入账号" />
				</view>
				<view class="input-group">
					<uni-icons type="locked" size="20" color="#666"></uni-icons>
					<input class="input" type="password" v-model="loginForm.password" placeholder="请输入密码" />
				</view>
				<!-- 登录选项 -->
				<view class="login-options">
					<view class="checkbox-group">
						<checkbox :checked="loginForm.rememberPwd" @tap="loginForm.rememberPwd = !loginForm.rememberPwd"
							color="#1e4c88" style="transform: scale(0.7);" />
						<text>记住密码</text>
					</view>
					<view class="checkbox-group">
						<checkbox :checked="loginForm.autoLogin" @tap="loginForm.autoLogin = !loginForm.autoLogin"
							color="#1e4c88" style="transform: scale(0.7);" />
						<text>自动登录</text>
					</view>
					<text class="forget-pwd" @click="handleForgetPwd">忘记密码？</text>
				</view>
				<button class="submit-btn" @click="handleLogin">登录</button>
			</view>

			<!-- 注册表单 -->
			<view class="form" v-show="activeTab === 'register'">
				<view class="input-group">
					<uni-icons type="person" size="20" color="#666"></uni-icons>
					<input class="input" type="text" v-model="registerForm.username" placeholder="请输入账号" />
				</view>
				<view class="input-group">
					<uni-icons type="locked" size="20" color="#666"></uni-icons>
					<input class="input" type="password" v-model="registerForm.password" placeholder="请输入密码" />
				</view>
				<view class="input-group">
					<uni-icons type="locked" size="20" color="#666"></uni-icons>
					<input class="input" type="password" v-model="registerForm.confirmPassword" placeholder="请确认密码" />
				</view>
				<button class="submit-btn" @click="handleRegister">注册</button>
			</view>
		</view>
	</view>
	<navigator url="/pages/self/iphone">跳转</navigator>
</template>

<script setup>
	import {
		ref,
		reactive,
		onMounted,
	} from 'vue'
onMounted(()=>{
	if(uni.getStorageSync('autoLogin')){
		uni.switchTab({
			url: '/pages/weather/weather'
		});
	}
	if(uni.getStorageSync('rememberPwd')){
		loginForm.value.username=uni.getStorageSync('usrname');
		loginForm.value.password=uni.getStorageSync('password');
	}

})
	// 当前激活的标签
	const activeTab = ref('login')

	// 登录表单数据
	const loginForm = ref({
		username: '',
		password: '',
		rememberPwd: false,
		autoLogin: false
	})

	// 注册表单数据
	const registerForm = reactive({
		username: '',
		password: '',
		confirmPassword: ''
	})

	// 登录处理
	const handleLogin = () => {
		console.log('登录表单：', loginForm);

			console.log(uni.getStorageSync('usrname'));
			console.log(uni.getStorageSync('autoLogin'));
			console.log(uni.getStorageSync('rememberPwd'));
			console.log(uni.getStorageSync('password'));
			uni.switchTab({
				url:'/pages/weather/weather'
			});
		// 这里添加登录逻辑
		// uni.request({
		// 	url: "http://192.168.40.93:8080/login",
		// 	method: 'POST',
		// 	data: loginForm.value,
		// 	success: (res) => {
		// 		// console.log(res.data);
		// 		if(res.data=== 200){
		// 			uni.showToast({
		// 			title: '登陆成功',
		// 			icon: 'success'
		// 		}),
		// 		uni.switchTab({
		// 			url:'/pages/weather/weather'
		// 		});
		// 		uni.setStorageSync('rememberPwd',loginForm.value.rememberPwd);
		// 		if (loginForm.rememberPwd) {
		// 			// 登录成功后保存token
		// 			uni.setStorageSync('username', loginForm.value.username);
		// 			uni.setStorageSync('password', loginForm.value.password);
		// 		}
		// 			uni.setStorageSync('autoLogin',loginForm.value.autoLogin);
		// 			console.log(loginForm.value);

		// 		}

		// 	}
		// })
	}

	// 注册处理
	const handleRegister = () => {
		console.log('注册表单：', registerForm)
		// 这里添加注册逻辑
		uni.request({
			url: "https://brief-lioness-initially.ngrok-free.app/register",
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method: 'POST',
			data: registerForm,
			success: (res) => {
				uni.showToast({
					title: '注册成功',
					icon: 'success'
				})
			}
		})
	}

	// 忘记密码处理
	const handleForgetPwd = () => {
		// 这里添加忘记密码逻辑
	}
</script>

<style>
	.container {
		min-height: 100vh;
		/* background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%); */
		background-image: linear-gradient(to bottom, #2695de, #9ce4fe);
		position: relative;
		display: flex;
		flex-direction: column;
		align-items: center;
		padding-top: 15vh;
	}

	.wave-box {
		height: 30vh;
		overflow: hidden;
		position: relative;
		position: absolute;
		top: 0;
		left: 0;
		right: 0;
		z-index: 1;
	}

	.wave {
		position: absolute;
		width: 200%;
		height: 200%;
		background: rgba(255, 255, 255, 0.1);
		border-radius: 40%;
		transform: translate(-25%, -75%);
		animation: wave 15s infinite linear;
	}

	@keyframes wave {
		from {
			transform: translate(-25%, -75%) rotate(0deg);
		}

		to {
			transform: translate(-25%, -75%) rotate(360deg);
		}
	}

	.header {
		left: 0;
		right: 0;
		display: flex;
		flex-direction: column;
		align-items: center;
		color: #fff;
		margin-bottom: 60rpx;
		position: relative;
		z-index: 2;
	}

	.logo {
		width: 120rpx;
		height: 120rpx;
		margin-bottom: 20rpx;
	}

	.title {
		font-size: 36rpx;
		font-weight: 600;
		letter-spacing: 2rpx;
	}

	.form-box {
		width: 85%;
		max-width: 600rpx;
		background: rgba(255, 255, 255, 0.98);
		border-radius: 20rpx;
		padding: 40rpx;
		/* box-shadow: 0 8rpx 24rpx rgba(0,0,0,0.15); */
		backdrop-filter: blur(10px);
		position: relative;
		z-index: 2;
	}

	.tab-box {
		display: flex;
		justify-content: center;
		margin-bottom: 40rpx;
		gap: 100rpx;
	}

	.tab-item {
		font-size: 32rpx;
		color: #000;
		position: relative;
		padding: 0 20rpx;
		transition: all 0.3s ease;
		font-weight: 500;
	}

	.tab-item.active {
		color: #1e4c88;
		font-weight: 600;
		transform: scale(1.1);
	}

	.input-group {
		display: flex;
		align-items: center;
		background: #f8f9fa;
		border-radius: 12rpx;
		padding: 20rpx;
		margin-bottom: 30rpx;
		transition: all 0.3s ease;
		border: 1px solid transparent;
	}

	.input-group:focus-within {
		background: #fff;
		border-color: #1e4c88;
		/* box-shadow: 0 2rpx 8rpx rgba(30,76,136,0.1); */
	}

	.input {
		flex: 1;
		margin-left: 20rpx;
		font-size: 28rpx;
		height: 44rpx;
		line-height: 44rpx;
		color: #000;
		font-weight: 500;
	}

	.input::placeholder {
		color: #666;
		font-weight: normal;
	}

	.login-options {
		display: flex;
		align-items: center;
		justify-content: space-between;
		margin-top: 20rpx;
		padding: 10rpx;
		background: #f8f9fa;
		border-radius: 8rpx;
	}

	.checkbox-group {
		display: flex;
		align-items: center;
		font-size: 26rpx;
		color: #000;
		padding: 4rpx 0;
		font-weight: 500;
	}

	.checkbox-group text {
		margin-left: -4rpx;
	}

	.submit-btn {
		/* background: #1e4c88; */
		background-image: linear-gradient(to bottom, #2695de, #61a8fe);
		color: #fff;
		border: none;
		border-radius: 12rpx;
		height: 88rpx;
		line-height: 88rpx;
		font-size: 32rpx;
		margin-top: 50rpx;
		transition: all 0.3s ease;
		/* box-shadow: 0 6rpx 16rpx rgba(30,76,136,0.3); */
		letter-spacing: 4rpx;
		font-weight: 600;
	}

	.submit-btn:active {
		transform: scale(0.98);
		/* box-shadow: 0 2rpx 8rpx rgba(30,76,136,0.2); */
	}

	.forget-pwd {
		color: #000;
		font-size: 26rpx;
		opacity: 0.9;
		transition: opacity 0.3s ease;
		font-weight: 500;
	}

	.forget-pwd:active {
		opacity: 0.6;
	}

	/* 添加表单切换动画 */
	.form {
		transition: all 0.3s ease;
	}

	.form-enter-active,
	.form-leave-active {
		transition: opacity 0.3s ease;
	}

	.form-enter-from,
	.form-leave-to {
		opacity: 0;
	}

	/* 修改图标颜色 */
	:deep(.uni-icons) {
		color: #000 !important;
	}
</style>