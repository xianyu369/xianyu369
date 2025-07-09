<template>
  <view class="weather-container">
    <!-- 顶部导航 -->
    <view class="nav-bar">
      <text class="title">天气中心</text>
      <view class="search-area">
        <input 
          class="search-input" 
          v-show="showSearch" 
          v-model="searchText"
          placeholder="搜索城市" 
          @blur="handleSearchBlur"
        />
        <view class="search-btn" @click="toggleSearch">
          <uni-icons type="search" size="18" color="#fff"></uni-icons>
        </view>
      </view>
    </view>

    <!-- 城市天气信息 -->
    <view class="city-weather">
      <view class="city-header">
        <!-- <text class="city-name">{{Response.results[0].location.name}}</text> -->
        <text class="current-temp">{{Response.results[0].daily[0].high}}°</text>
        <view class="weather-status">
          <text class="status-text">{{Response.results[0].daily[0].text_day}}</text>
          <view class="status-detail">
            <text class="detail-item">{{Response.results[0].daily[0].wind_direction+'风  '+Response.results[0].daily[0].wind_speed}}  {{Response.results[0].daily[0].wind_scale}}级</text>
          </view>
        </view>
      </view>
      
     <view style="display: flex;justify-content: center;align-items: center;">天气预报列表</view>
      <scroll-view class="weather-list" scroll-x>
		  
		  <view v-for="(result,index) in Response.results" :key="result.location.id">
        <view class="weather-item" v-for="(item, index) in result.daily" :key="index">
          <text class="date">{{item.date}}</text>
		  <image :src="getimg(item.code_day)" class="weather-image"></image>
          <view class="temp-range">
            <text class="min">{{item.low}}℃</text>
            <text class="max">{{item.high}}℃</text>
          </view>
        </view>
		</view>
      </scroll-view>
      <!-- 实时数据 -->
      <view class="data-cards">
        <view class="data-card">
          <view class="card-content">
            <view class="card-icon">
              <uni-icons type="cloud-rain" size="24" color="#fff"></uni-icons>
            </view>
            <view class="card-info">
              <text class="value">{{Response.results[0].daily[0].precip}}%</text>
              <text class="label">降水概率</text>
            </view>
          </view>
        </view>
        <view class="data-card">
          <view class="card-content">
            <view class="card-icon">
              <uni-icons type="umbrella" size="24" color="#fff"></uni-icons>
            </view>
            <view class="card-info">
              <text class="value">{{Response.results[0].daily[0].rainfall}}mm</text>
              <text class="label">降水量</text>
            </view>
          </view>
        </view>
        <view class="data-card">
          <view class="card-content">
            <view class="card-icon">
              <uni-icons type="wind" size="24" color="#fff"></uni-icons>
            </view>
            <view class="card-info">
              <text class="value">{{Response.results[0].daily[0].humidity}}%</text>
              <text class="label">湿度</text>
            </view>
          </view>
        </view>
      </view>

      <!-- 室内环境 -->
      <view class="indoor-panel">
        <view class="panel-header">
          <text class="panel-title"  style="display: flex;justify-content: center;align-items: center;">室内环境</text>
          <text class="panel-subtitle"  style="display: flex;justify-content: center;align-items: center;">实时监测数据</text>
        </view>
        <view class="indoor-grid">
          <view class="grid-item">
            <view class="item-icon">
              <uni-icons type="thermometer" size="24" color="#4CAF50"></uni-icons>
            </view>
            <view class="item-data">
              <text class="data-value">{{indoorData.temperature}}28°C</text>
              <text class="data-label">室内温度</text>
            </view>
          </view>
          <view class="grid-item">
            <view class="item-icon">
              <uni-icons type="water" size="24" color="#2196F3"></uni-icons>
            </view>
            <view class="item-data">
              <text class="data-value">{{indoorData.humidity}}30%</text>
              <text class="data-label">室内湿度</text>
            </view>
          </view>
          <view class="grid-item">
            <view class="item-icon">
              <uni-icons type="sun" size="24" color="#FFC107"></uni-icons>
            </view>
            <view class="item-data">
              <text class="data-value">{{indoorData.light}}60lux</text>
              <text class="data-label">室内光照</text>
            </view>
          </view>
        </view>
      </view>
	  <view class="indoor-panel">
	    <view class="panel-header">
	      <text class="panel-title"  style="display: flex;justify-content: center;align-items: center;">ai问答</text>
	    </view>
	    
	  	{{ val }}
	  <view class="input-group" style="padding: 15rpx;">
	  	<input type="text" v-model="postkeys.prompt" class="input" placeholder="有问题就来找我" style="background-color: white;"/>
	  	<button type="primary" class="button" @click="getkeys" style="margin-right: 40rpx;">发送</button>
	  </view>
	  </view>

    </view>
  </view>

  


</template>

<script setup>
import { onMounted, ref ,onUnmounted} from 'vue'
import axios from 'axios';
const showSearch = ref(false)
const searchText = ref('')
const videoUrl = ref('')

const toggleSearch = () => {
	
  showSearch.value = !showSearch.value
  if (showSearch.value) {
    // 显示输入框时自动聚焦
    setTimeout(() => {
      const input = document.querySelector('.search-input')
      input && input.focus()
    }, 100)
  }
}

const handleSearchBlur = () => {
  // 失去焦点时如果输入框为空则隐藏
  if (!searchText.value) {
    showSearch.value = false

  }
  	console.log(searchText.value);
	uni.request({
		url:"https://brief-lioness-initially.ngrok-free.app:8080/getweather?location="+searchText.value,
		method:'GET',
		success: (res) => {
			Response.value=res.data;
			uni.showToast({
				title:'查询成功',
				icon:'success'
			})
		}
	})
}

// 定义 Weather 对象
const Weather = ref({
  date: "0", // 日期
  text_day: "0", // 白天天气描述
  code_day: "0", // 白天天气代码
  text_night: "0", // 夜间天气描述
  code_night: "0", // 夜间天气代码
  high: "0", // 最高温度
  low: "0", // 最低温度
  precip: "0", // 降水量
  wind_direction: "0", // 风向
  wind_direction_degree: "0", // 风向角度
  wind_speed: "0", // 风速
  wind_scale: "0", // 风力等级
  rainfall: "0", // 降雨量
  humidity: "0" // 湿度
});

// 定义 Location 对象
const Location = ref({
  id: "0", // 地点 ID
  name: "0", // 地点名称
  country: "0", // 国家
  path: "0", // 路径
  timezone: "0", // 时区
  timezone_offset: "0" // 时区偏移
});

// 定义 Result 对象
const Result = ref({
  location: {}, // 地点信息
  daily: [], // 每日天气数据
  last_update: "" // 最后更新时间
});

// 定义 Response 对象
const Response =ref( {
  results: [] // 结果列表
});

function getimg(type){
	return '../../static/a'+type+'.png';
}
function test(){
	uni.request({
		url:"https://brief-lioness-initially.ngrok-free.app/getweather?location=石家庄",
		header: {
		        'ngrok-skip-browser-warning': 'true' // 添加此请求头
		    },
		method:'GET',
		success: (res) => {
			Response.value=res.data;
			console.log(Response.value)
		}
	})
}
// 室内数据
const indoorData = ref({
  temperature: 22,
  humidity: 45,
  light: 320
})
function test1(){
	uni.request({
		url:"https://brief-lioness-initially.ngrok-free.app/GetAllKeys",
		header: {
		        'ngrok-skip-browser-warning': 'true' // 添加此请求头
		    },
		method:'GET',
		success: (res) => {
			
			indoorData.value.humidity=res.data.aht10_humi;
			indoorData.value.temperature=res.data.aht10_temp;
			indoorData.value.light=res.data.ap3216c_ps;
		}
	})
}
// 天气预报数据

// 实时数据
const realTimeData = ref({
  rainProb: 0.00,
  rainfall: 0.00,
  humidity: 53
})

onMounted(()=>{
	test();
	test1();
	  // 设置定时器，每5秒调用一次 test1 函数
	  const intervalId = setInterval(() => {
	    test1();
	  }, 5000); // 5000毫秒（5秒）
	
	  // 在组件卸载时清除定时器，防止内存泄漏
	  onUnmounted(() => {
	    clearInterval(intervalId);
	  });
})
const val = ref('');
const postkeys = ref({
	model: 'deepseek-r1:7b',
	prompt: '',
	max_tokens: 50,
	temperature: 0.7,
	top_p: 1,
	stream: true
});

function getkeys() {
	val.value='';
socket.send({
        data: postkeys.value.prompt,

        success: () => console.log('消息发送成功'),
        fail: () => console.error('消息发送失败'),
      })

}
uni.connect
const socket=uni.connectSocket({
	url: 'https://brief-lioness-initially.ngrok-free.app/Stream', // WebSocket服务器地址
	header:{
		'ngrok-skip-browser-warning': 'true'
	},
	      success: () => console.log('WebSocket连接成功'),
	      fail: (err) => console.error('WebSocket连接失败', err),
})
socket.onOpen(() => {
      console.log('WebSocket连接已打开');
      // 连接打开后，可以发送消息
      // this.sendMessage('Hello WebSocket');
    });

    socket.onMessage((message) => {
			const data = JSON.parse(message.data);
			if(!data||!message.data){
				val.value='';
			}
			else{
					val.value=val.value+data.response;
			}
		
      console.log('收到服务器内容：', message.data);
    });

    socket.onError((error) => {
      console.error('WebSocket连接发生错误', error);
    });

    socket.onClose(() => {
      console.log('WebSocket连接已关闭');
    });


</script>

<style lang="scss" scoped>
// .weather-container {
//   min-height: 100vh;
//   background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
//   color: #fff;
//   padding-bottom: 120rpx;
// }
.weather-container {
 min-height: 100vh;
 background-image: linear-gradient(to bottom, #2695de, #9ce4fe);
 // background-color: #fff;
 // background: linear-gradient(180deg, #1e4c88 0%, #0c2d5f 100%);
 padding: 30rpx;
 padding-bottom: 120rpx;
}

.nav-bar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 40rpx 30rpx;
  background: rgba(255,255,255,0.1);
  backdrop-filter: blur(10px);
}

.city-header {
  padding: 40rpx 30rpx;
  text-align: center;
}

.city-name {
  font-size: 48rpx;
  font-weight: 600;
}

.current-temp {
  font-size: 80rpx;
  font-weight: 200;
  margin-top: 20rpx;
  display: block;
  font-weight: bold;
}

.weather-list {
  white-space: nowrap;
  padding: 20rpx;
  background: rgba(255,255,255,0.1);
}

.weather-item {
  display: inline-flex;
  flex-direction: column;
  align-items: center;
  padding: 20rpx 40rpx;
  gap: 16rpx;
}

.date {
  font-size: 28rpx;
  color: rgba(255,255,255,0.9);
}

.temp {
  font-size: 40rpx;
  font-weight: bold;
}

.weather-text {
  font-size: 28rpx;
  color: rgba(255,255,255,0.9);
}

.temp-range {
  display: flex;
  gap: 20rpx;
  font-size: 28rpx;
  color: rgba(255,255,255,0.8);
}

.data-cards {
  display: flex;
  padding: 20rpx;
  gap: 20rpx;
}

.data-card {
  flex: 1;
  background: rgba(255,255,255,0.15);
  border-radius: 20rpx;
  padding: 30rpx;
  backdrop-filter: blur(5px);
}

.card-content {
  display: flex;
  align-items: center;
  gap: 20rpx;
}

.card-info {
  display: flex;
  flex-direction: column;
}

.indoor-panel {
  margin: 30rpx;
  background: rgba(255,255,255,0.9);
  border-radius: 30rpx;
  padding: 30rpx;
  color: #333;
}

.panel-header {
  margin-bottom: 30rpx;
}

.panel-title {
  font-size: 32rpx;
  font-weight: 600;
  display: block;
}

.panel-subtitle {
  font-size: 24rpx;
  color: #666;
}

.indoor-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 30rpx;
}

.grid-item {
  background: #f5f5f5;
  border-radius: 20rpx;
  padding: 20rpx;
  text-align: center;
}

.item-icon {
  margin-bottom: 10rpx;
}

.data-value {
  font-size: 32rpx;
  font-weight: 600;
  display: block;
}

.data-label {
  font-size: 24rpx;
  color: #666;
}

.tab-bar {
  position: fixed;
  bottom: 0;
  left: 0;
  right: 0;
  display: flex;
  background: rgba(255,255,255,0.95);
  padding: 16rpx 0;
  backdrop-filter: blur(10px);
  border-top: 1rpx solid rgba(0,0,0,0.1);
}

.tab-item {
  flex: 1;
  text-align: center;
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

.weather-status {
  text-align: center;
  margin: 10rpx 0;
}

.status-text {
  font-size: 32rpx;
  color: rgba(255,255,255,0.9);
  margin-bottom: 8rpx;
}

.status-detail {
  display: flex;
  justify-content: center;
  align-items: center;
}

.detail-item {
  padding: 4rpx 20rpx;
  background: rgba(255,255,255,0.2);
  border-radius: 30rpx;
  font-size: 24rpx;
  color: rgba(255,255,255,0.8);
}






.weather-image {
  width: 50px; /* 根据需要调整图片宽度 */
  height: 50px; /* 根据需要调整图片高度 */
  margin-bottom: 5px;
  border-radius: 4px;
}

.temp-range {
  display: flex;
  justify-content: space-between;
  width: 100%;
}

.min, .max {
  font-size: 14px;
}
.input-box {
  border: 2px solid #333; /* 设置边框为深色和粗细 */
  padding: 10px;
  border-radius: 5px; /* 设置圆角 */
}
.search-area {
  display: flex;
  align-items: center;
  gap: 20rpx;
}

.search-input {
  width: 400rpx;
  height: 60rpx;
  background: rgba(255,255,255,0.2);
  border-radius: 30rpx;
  padding: 0 30rpx;
  color: #fff;
  font-size: 28rpx;
  transition: all 0.3s ease;
}

.search-input::placeholder {
  color: rgba(255,255,255,0.6);
}

.search-btn {
  width: 60rpx;
  height: 60rpx;
  background: rgba(255,255,255,0.2);
  border-radius: 30rpx;
  display: flex;
  align-items: center;
  justify-content: center;
  cursor: pointer;
}

.output {
	width: 100%;
	margin-bottom: 20px;
	padding: 10px;
	background-color: #f5f5f5;
	border: 1px solid #ddd;
	border-radius: 5px;
	overflow: auto;
}

.input-group {
	display: flex;
	width: 100%;
}

.input {
	flex: 1;
	margin-right: 10px;
	padding: 10px;
	border: 1px solid #ddd;
	border-radius: 5px;
}

.button {
	
	border-radius: 5px;
	width: 160rpx;
	height: 90rpx;
}
</style>
