<template>
	<view class="control-container">
		<!-- 顶部标题 -->
		<view class="header">
			<text class="title">控制中心</text>
		</view>

		<!-- 开关控制区 -->
		<view class="switch-panel">
			<view class="switch-grid">
				<view class="switch-item">
					<view class="test">
						<view class="item-left">
							
							<text>客厅灯</text>
							<image v-if="!keys.livingroom_led_state" class="action-icon1" src="../../static/deng.png"></image>
							<image v-else class="action-icon1" src="../../static/deng1.png"></image>
						</view>
						<switch class="img" color="#ffab0f"  @change="IsSwitch('livingroom')" :checked="keys.livingroom_led_state" />
					</view>
<!-- 					<view class="test">
						<view class="item-left">
							
							<text>卧室灯</text>
							<image v-if="!keys.livingroom_led_state" class="action-icon1" src="../../static/窗户-关 (1).png"></image>
							<image v-else class="action-icon1" src="../../static/窗户-开.png"></image>
							
						</view>
						<switch  @change="IsSwitch('bedroom')" :checked="keys.bedroom_led_state" />
					</view> -->
					
				</view>
				<view class="switch-item">
					<view class="item-left">
						
						<text>卧室灯</text>
						<image v-if="!keys.bedroom_led_state" class="action-icon1" src="../../static/deng.png"></image>
						<image v-else class="action-icon1" src="../../static/deng1.png"></image>
						
					</view>
					<switch class="img" color="#ffab0f"  @change="IsSwitch('bedroom')" :checked="keys.bedroom_led_state" />
				</view>
				<view class="switch-item">
					<view class="item-left">
						
						<text>厨房灯</text>
						<image v-if="!keys.foodroom_led_state" class="action-icon1" src="../../static/deng.png"></image>
						<image v-else class="action-icon1" src="../../static/deng1.png"></image>
					</view>
					
					<!-- <view style="height: 5px;width: 5px;"> -->
						<switch class="img" color="#ffab0f"  @change="IsSwitch('foodroom')" :checked="keys.foodroom_led_state" />
						
					<!-- </view> -->
						</view>
				<view class="switch-item">
					<view class="item-left">
						
						<text>厕所灯</text>
						<image v-if="!keys.wc_led_state" class="action-icon1" src="../../static/deng.png"></image>
						<image v-else class="action-icon1" src="../../static/deng1.png"></image>
					</view>
					<switch class="img" color="#ffab0f" @change="IsSwitch('WC')" :checked="keys.wc_led_state" />
				</view>
						<view class="switch-item">
							<view class="item-left">
								
						<text>客厅窗户</text>
						<image v-if="!keys.livingroom_window_state" class="action-icon1" src="../../static/windows1.png"></image>
						<image v-else class="action-icon1" src="../../static/windows.png"></image>
					</view>
					<switch class="img" color="#ffab0f"  @change="IsSwitch('livingroom_windows')"
						:checked="keys.livingroom_window_state" />
				</view>
				<view class="switch-item">
					<view class="item-left">
						
						<text>卧室窗户</text>
						<image v-if="!keys.bedroom_window_state" class="action-icon1" src="../../static/windows1.png"></image>
						<image v-else class="action-icon1" src="../../static/windows.png"></image>
					</view>
					<switch class="img" color="#ffab0f"  @change="IsSwitch('bedroom_windows')" :checked="keys.bedroom_window_state" />
				</view>
				<view class="switch-item">
					<view class="item-left">
						
						
						<text>风扇开关</text>
						<image v-if="!keys.fan_state" class="action-icon1" src="../../static/fan1.png"></image>
						<image v-else class="action-icon1" src="../../static/fan.png"></image>
					</view>
					<switch class="img" color="#ffab0f" @change="IsSwitch('fan')" :checked="keys.fan_state" />
				</view>
				<view class="switch-item">
					<view class="item-left">
						
						<text>报警器</text>
						<image v-if="!keys.buzz_Lock" class="action-icon1" src="../../static/buzz-copy.png"></image>
						<image v-else class="action-icon1" src="../../static/buzz-copy (1).png"></image>
					</view>
					<switch class="img" color="#ffab0f"  @change="IsSwitch('ling')" :checked="keys.buzz_Lock" />
				</view>
				

			</view>

		</view>
		<view class="fan-control">
		  <text class="control-title">彩灯开关:</text>
		  <view class="fan-buttons">
		    <button class="fan-btn"  :class="{'active' : deng.type1}" @click="postdeng('2')">彩虹灯</button>
		    <button class="fan-btn"  :class="{'active' : deng.type2}" @click="postdeng('3')">流水灯</button>
		    <button class="fan-btn"  :class="{'active' : deng.type3}" @click="postdeng('5')">炫彩灯</button>
		    <button class="fan-btn"  :class="{'active' : deng.close}" @click="postdeng('0')">关灯</button>
		  </view>
		</view>
		
		<view class="fan-control">
		<text class="control-title">监控开关:</text>
		<img v-if="see.on" style="width: 90vw;height: 300rpx;" :src="videoUrl" />
		<view v-else="see.off" style="width: 90vw;height: 300rpx;">未开启</view>
		  <view class="fan-buttons">
		    <button class="fan-btn"  :class="{'active' : see.on}" @click="onsee('1')">开启</button>
		    <button class="fan-btn"  :class="{'active' : see.off}" @click="onsee('2')">关闭</button>
		  </view>
		</view>
		<uni-popup ref="popupRef" type="center" background-color="transparent" @change="popupChange">
		  <view class="popup-content">
		    <view class="menu-item">
		      {{ currentMenuItem }}
		    </view>
		    <view class="divider"></view>
		    <view class="control-buttons">
		      <button class="control-button" @click="toggleAutoScroll">
		        {{ autoScroll ? '停止' : '开始' }}
		      </button>
		      <button class="control-button cancel-button" @click="closePopup">关闭</button>
		    </view>
		  </view>
		</uni-popup>
<uni-fab ref="fab" :pattern="pattern" :content="content" :horizontal="horizontal" :vertical="vertical"
			:direction="direction" @trigger="trigger" />
	</view>
</template>

<script setup>
	import {
		onMounted,
		onUnmounted,
		computed,
		ref
	} from 'vue'
	const fans=ref([]);
	let timer;
	const fan=ref();
	const popupRef = ref(null);
	const menuItems = ref(['菜单1', '菜单2', '菜单3', '菜单4']);
	const autoScroll = ref(false);
	const currentIndex = ref(0);
	let scrollInterval = null;
	
	// Computed property for current menu item
	const currentMenuItem = computed(() => fans.value[currentIndex.value]);
	
	// Methods
	const openPopup = () => {
	  popupRef.value.open();
	  startAutoScroll();
	};
	
	const closePopup = () => {
	  popupRef.value.close();
	  stopAutoScroll();
	};
	
	const startAutoScroll = () => {
	  if (!scrollInterval) {
	    autoScroll.value = true;
	    scrollInterval = setInterval(() => {
	      currentIndex.value = (currentIndex.value + 1) % fans.value.length;
	    }, 100);
	  }
	};
	
	const stopAutoScroll = () => {
	  if (scrollInterval) {
	    clearInterval(scrollInterval);
	    scrollInterval = null;
	    autoScroll.value = false;
	  }
	};
	
	const toggleAutoScroll = () => {
	  if (autoScroll.value) {
	    stopAutoScroll();
	  } else {
	    startAutoScroll();
	  }
	};
	
	const popupChange = (e) => {
	  // If popup is closed, stop auto-scrolling
	  if (!e.show) {
	    stopAutoScroll();
	  }
	};
	
	// Clean up interval when component is unmounted
	onUnmounted(() => {
	  stopAutoScroll();
	});
	const see=ref({
		on:false,
		off:true
	})
	const fabText=ref('娱乐');
	const title=ref('uni-fab');
	const directionStr=ref('垂直');
	const horizontal=ref('left');
	const vertical=ref('bottom');
	const direction=ref('horizontal');
	const pattern=ref({
						color: '#7A7E83',
						backgroundColor: '#fff',
						selectedColor: '#007AFF',
						buttonColor: '#007AFF',
						iconColor: '#fff'
					});
	const is_color_type=ref(false);
	const content=ref([{
							iconPath: '../../static/sleep.png',
							text: '睡眠',
							active: false
						},
						{
							iconPath: '../../static/wan.png',
							text: '娱乐',
							active: false
						},{
							iconPath: '../../static/rice.png',
							text: '吃什么',
							active: false
						},
						{
							iconPath: '../../static/jia.png',
							text: '添加菜名',
							active: false
						}
					]);

	// function fabClick(){
	// 	uni.navigateTo({
	// 			url:"/pages/music/music"
	// 	})
	// }
	function onsee(type){
		switch(type){
			case '1':
			see.value.on=true;
			see.value.off=false;
			break;
			case '2':
			see.value.on=false;
			see.value.off=true;
			break;
		}
	}
const videoUrl = "http://192.168.40.13:81/stream";
// const videoUrl = "rtmp://https://brief-lioness-initially.ngrok-free.app:1935/live";
	const keys = ref({
		buzz_Lock: false,
		fan_state: false,
		// fan_lock: true,
		livingroom_led_state: false,
		bedroom_led_state: false,
		wc_led_state: false,
		foodroom_led_state: false,
		livingroom_window_state: false,
		bedroom_window_state: false,
		livingroom_window_flag: false,
		bedroom_window_flag: false,
		sleeping_state:false,
		etertain_state:false,
		music_type:"0"
	})

	function trigger(e) {
				
				if(e.item.text=='睡眠'){
					uni.showToast({
						icon:'success',
						title:'您已进入睡眠模式'
					});
					keys.value.bedroom_led_state=false;
					keys.value.wc_led_state=false;
					keys.value.livingroom_led_state=false;
					keys.value.foodroom_led_state=false;
				}
				else if(e.item.text=='吃什么'){
						openPopup();
					
				}
				else if(e.item.text=='添加菜名'){
					uni.showModal({
						title: "添加",
						content: "",
						editable:true,
						success: function (res) {
							if (res.confirm) {
								console.log('用户点击确定');
								uni.request({
									url: "https://brief-lioness-initially.ngrok-free.app/post_fans?fan="+res.content,
									header: {
									        'ngrok-skip-browser-warning': 'true' // 添加此请求头
									    },
									method : 'GET',
									success() {
										uni.showToast({
											icon:'success',
											title:'添加成功'
										})
									}
								})
							} else if (res.cancel) {
								console.log('用户点击取消');
							}
						}
					});
				}
				else{
					deng.value.type1=true;
					deng.value.type2=false;
					deng.value.type3=false;
					deng.value.close=false;
						uni.navigateTo({
								url:"/pages/music/music?type=1"
						})
				}

			}
	function IsSwitch(type) {
		switch (type) {
			case 'livingroom':
				keys.value.livingroom_led_state = !keys.value.livingroom_led_state;
				break;
			case "bedroom":
				keys.value.bedroom_led_state = !keys.value.bedroom_led_state;
				break;
			case 'foodroom':
				keys.value.foodroom_led_state = !keys.value.foodroom_led_state;
				break;
			case 'livingroom_windows':
				keys.value.livingroom_window_flag = !keys.value.livingroom_window_flag;
				keys.value.livingroom_window_state = !keys.value.livingroom_window_state;
				break;
			case 'bedroom_windows':
				keys.value.bedroom_window_flag = !keys.value.bedroom_window_flag;
				keys.value.bedroom_window_state = !keys.value.bedroom_window_state;
				break;
			case 'fan':
				keys.value.fan_state = !keys.value.fan_state;
				keys.value.fan_lock = !keys.value.fan_lock;
				break;
			case 'ling':
				keys.value.buzz_Lock = !keys.value.buzz_Lock;
				break;
			case 'WC':
				keys.value.wc_led_state = !keys.value.wc_led_state;
				break;
		}
		postkeys()
	}

	function postkeys() {

		uni.request({
			url: "https://brief-lioness-initially.ngrok-free.app/PostAllKeys",
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method : 'POST',
			data:keys.value,
			success: (res) => {
				uni.showToast({
					title: "状态改变成功",
					icon: 'success'
				})
			}
		})
	}
	const deng=ref({
		type1:false,
		type2:false,
		type3:false,
		close:true
	})
	function postdeng(e){
		uni.request({
			url:"https://brief-lioness-initially.ngrok-free.app/postdeng?id="+e,
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method: "GET",
			success: (res) => {
				uni.showToast({
					title:'已开启',
					icon:"success"
				})
			}
		})
		switch(e){
			case '2':
			deng.value.type1=true;
			deng.value.type2=false;
			deng.value.type3=false;
			deng.value.close=false;
			break;
			case '3':
			deng.value.type1=false;
			deng.value.type2=true;
			deng.value.type3=false;
			deng.value.close=false;
			break;
			case '5':
			deng.value.type1=false;
			deng.value.type2=false;
			deng.value.type3=true;
			deng.value.close=false;
			break;
			case '0':
			deng.value.type1=false;
			deng.value.type2=false;
			deng.value.type3=false;
			deng.value.close=true;
			break;
			default:
			deng.value.type1=false;
			deng.value.type2=false;
			deng.value.type3=false;
			deng.value.close=true;
			break;
		}
	}
	function initdeng(){
		uni.request({
			url:"https://brief-lioness-initially.ngrok-free.app/getdeng",
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method:"GET",
			success: (res) => {
				if(res.data){
					deng.value=res.data;
					
				}
			}
		})
	}
	const temp=ref()

	function test() {
		uni.request({
			url:"https://brief-lioness-initially.ngrok-free.app/GetAllKeys",
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method: "GET",
			success: (res) => {
				if(res.data=="400"){
					console.log(res.data)
				}
				else{
					keys.value=res.data;
					console.log(keys.value.sleeping_state);
					console.log("000000"+keys.value.etertain_state);
					if(keys.value.sleeping_state){
						console.log(keys.value.sleeping_state)
						keys.value.bedroom_led_state=false;
						keys.value.wc_led_state=false;
						keys.value.livingroom_led_state=false;
						keys.value.foodroom_led_state=false;
						keys.value.sleeping_state=false;
						postkeys();
					}
					if(keys.value.etertain_state){
						keys.value.entertain_state=false;
						postkeys();
						console.log("77777")
						uni.navigateTo({
								url:"/pages/music/music"
						})
					}
					if(keys.value.music_type=="1"){
						console.log("888888")
						// keys.value.entertain_state=false;
						// postkeys();
						keys.value.music_type='0';
						postkeys();
						uni.navigateTo({
								url:"/pages/music/music?type="+keys.value.music_type
						})
					}
					if(keys.value.music_type=="2"){
						console.log("66666")
						// keys.value.entertain_state=false;
						postkeys();
						uni.navigateTo({
								url:"/pages/music/music?type="+keys.value.music_type
						})
					}
					
				}
	

			}
		})
	}
	function call(){
	    // 导入Activity、Intent类  
	    var Intent = plus.android.importClass("android.content.Intent");  
	    var Uri = plus.android.importClass("android.net.Uri");  
	    // 获取主Activity对象的实例  
	    var main = plus.android.runtimeMainActivity();  
	    // 创建Intent  
	    var uri = Uri.parse("tel:17331596859"); // 这里可修改电话号码  
	    var call = new Intent("android.intent.action.CALL",uri);  
	var dualSimTypes = [
	        "com.android.phone.extra.slot",
	        "subscription",
	        "simSlot",
	        "simslot",
	        "sim_slot",
	        "slot",
	        "simId",
	        "simnum",
	        "phone_type",
	        "slotId",
	        "slotIdx",
	        "extra_asus_dial_use_dualsim"
	    ];
	
	    for (var i = 0; i < dualSimTypes.length; i++) {
	        call.putExtra(dualSimTypes[i], 0); // 0 表示 SIM 卡 1
	    }
	    // 调用startActivity方法拨打电话  
	    main.startActivity( call );  
	    // ...  
		plus.webview.currentWebview().addEventListener('pause', () => {
		        keys.value.buzz_Lock=false;
		      });
	}
	function getfans(){
		uni.request({
			url:"https://brief-lioness-initially.ngrok-free.app/get_fans",
			header: {
			        'ngrok-skip-browser-warning': 'true' // 添加此请求头
			    },
			method: "GET",
			success: (res) => {
				fans.value=res.data;
				console.log(res.data);
			}
		})
	}

	onMounted(() => {
		getfans();
	// 	initdeng();
		
	// 	const intervalId1 = setInterval(() => {
	// 	        if (keys.value.buzz_Lock) {
	// 				call();
	// 	        } else {
	// 	        }
	// 	      }, 3000);
	// 	const intervalId = setInterval(() => {
		
	// 	  test();
	// 	}, 2000); // 5000毫秒（5秒）


	});
	

</script>

<style lang="scss" scoped>
	.custom-button {
	  position: fixed;
	  bottom: 15vh;  /* 距离页面顶部15%的高度 */
	  right: 20px;
	  width: 50px;
	  height: 50px;
	  background-color: #ff00ff;
	  color: #fff;
	  font-size: 12px;
	  text-align: center;
	  line-height: 50px; /* 使文字垂直居中 */
	  border-radius: 50%; /* 圆形按钮 */
	  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
	}
	.img{
		transform: scale(0.9, 0.8);
		margin-left: 20px;
		margin-top: 10px;
	}
	.test{
		display: flex;
		flex-direction: row;
	}
	.action-icon1 {
		height: 45rpx;
		width: 50rpx;
		color: #666;
	}
	.control-container {
		min-height: 100vh;
		background-image: linear-gradient(to bottom, #2695de, #9ce4fe);
		// background-color: #fff;
		// background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
		padding: 30rpx;
		padding-bottom: 120rpx;
	}

	.header {
		padding: 20rpx 0;
		text-align: center;
		background: rgba(255, 255, 255, 0.1);
		backdrop-filter: blur(10px);
		border-radius: 16rpx;
		margin-bottom: 30rpx;
	}

	.title {
		color: #f7f7f7;
		font-size: 36rpx;
		font-weight: bold;
		font-family: "SimSun", "楷体", sans-serif;
	}

	.switch-panel {
		// background: rgba(255, 255, 255, 0.95);
		border-radius: 20rpx;
		padding: 30rpx;
		box-shadow: 0 4rpx 12rpx rgba(0, 0, 0, 0.1);
	}

	.switch-grid {
		display: grid;
		grid-template-columns: repeat(2, 1fr);
		gap: 30rpx;
	}

	.switch-item {
		
		display: flex;
		
		justify-content: space-between;
		align-items: center;
		background: rgba(248, 249, 250);
		padding: 30rpx;
		border-radius: 16rpx;
		box-shadow: 0 2rpx 8rpx rgba(0, 0, 0, 0.1);
		transition: all 0.3s ease;
	}

	.switch-item:active {
		transform: scale(0.98);
	}

	.item-left {
		flex-direction: column;
		display: flex;
		align-items: center;
		gap: 16rpx;
	}

	.item-left text {
		font-size: 30rpx;
		font-weight: bold;
		color: #84807e;
		
		// font-weight: 500;
	}

	.fan-control {
		background: rgba(255, 255, 255, 0.9);
		border-radius: 20rpx;
		padding: 20rpx;
		margin: 20rpx 0;
	}

	.control-title {
		margin-bottom: 20rpx;
		font-weight: bold;
		display: block;
	}


	.fan-buttons {
		display: flex;
		justify-content: space-between;
		gap: 10rpx;
	}

	.fan-btn {
		flex: 1;
		font-size: 28rpx;
		padding: 15rpx 0;
		background: #f5f5f5;
		border: none;
		border-radius: 12rpx;
	}

	.fan-btn.active {
		background: #1989fa;
		color: #fff;
	}
	.direction-control {
		background: rgba(255, 255, 255, 0.9);
		border-radius: 20rpx;
		padding: 30rpx;
		margin: 20rpx 0;
	}

	.direction-pad {
		width: 300rpx;
		margin: 0 auto;
	}

	.direction-row {
		display: flex;
		justify-content: center;
		gap: 10rpx;
		margin: 10rpx 0;
	}

	.direction-btn {
		width: 80rpx;
		height: 80rpx;
		display: flex;
		align-items: center;
		justify-content: center;
		background: #f5f5f5;
		border: none;
		border-radius: 12rpx;
		font-size: 32rpx;
	}

	.direction-btn:active {
		background: #1989fa;
		color: #fff;
	}

	/* 底部导航栏样式沿用天气页面的样式 */
	.tab-bar {
		position: fixed;
		bottom: 0;
		left: 0;
		right: 0;
		background: rgba(255, 255, 255, 0.95);
		backdrop-filter: blur(10px);
		display: flex;
		padding: 16rpx 0;
		border-top: 1rpx solid rgba(0, 0, 0, 0.1);
	}

	.tab-item {
		flex: 1;
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
		font-size: 30rpx;
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
	.container {
	  display: flex;
	  justify-content: center;
	  padding: 20px;
	}
	
	/* Apple-style button */
	.open-button {
	  background-color: #007AFF;
	  color: white;
	  padding: 12px 24px;
	  border: none;
	  border-radius: 8px;
	  font-size: 16px;
	  font-weight: 500;
	  cursor: pointer;
	  transition: background-color 0.2s ease;
	}
	
	.open-button:active {
	  background-color: #0062CC;
	}
	
	/* Apple-style popup content */
	.popup-content {
	  background-color: rgba(250, 250, 250, 0.95);
	  backdrop-filter: blur(10px);
	  -webkit-backdrop-filter: blur(10px);
	  border-radius: 14px;
	  width: 270px;
	  overflow: hidden;
	  box-shadow: 0 8px 30px rgba(0, 0, 0, 0.12);
	  animation: scale-up 0.3s cubic-bezier(0.2, 0, 0.2, 1);
	  border: none;
	  display: flex;
	  flex-direction: column;
	}
	
	.menu-item {
	  font-size: 18px;
	  font-weight: 500;
	  color: #1D1D1F;
	  padding: 20px;
	  text-align: center;
	  flex: 2;
	  display: flex;
	  align-items: center;
	  justify-content: center;
	  min-height: 80px;
	}
	
	.divider {
	  height: 1px;
	  background-color: rgba(60, 60, 67, 0.1);
	  margin: 0 8px;
	}
	
	.control-buttons {
	  display: flex;
	  flex-direction: row;
	  width: 100%;
	  flex: 1;
	  height: 50px; /* Fixed height for buttons section */
	}
	
	.control-button {
	  background-color: transparent;
	  color: #007AFF;
	  border: none;
	  font-size: 17px;
	  font-weight: 400;
	  cursor: pointer;
	  transition: background-color 0.2s ease;
	  flex: 1;
	  border-right: 1px solid rgba(60, 60, 67, 0.1);
	  display: flex;
	  align-items: center;
	  justify-content: center;
	  height: 100%;
	}
	
	.control-button:last-child {
	  border-right: none;
	}
	
	.control-button:active {
	  background-color: rgba(0, 0, 0, 0.05);
	}
	
	.cancel-button {
	  font-weight: 600;
	}
	
	@keyframes scale-up {
	  from {
	    opacity: 0;
	    transform: scale(0.8);
	  }
	  to {
	    opacity: 1;
	    transform: scale(1);
	  }
	}

	
</style>