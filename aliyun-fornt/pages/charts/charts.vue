<template>
	<view class="container">
		<view class="chart-container">
			<canvas canvas-id="lightChart" class="chart-canvas"></canvas>
		</view>
		<view>{{msg}}</view>
	</view>
</template>

<script setup>
import { ref,onMounted } from 'vue';
import * as echarts from 'echarts';

let myChart = null;
const msg=ref();

const eventSource = new EventSource('http://localhost:8080/sse?userId=1');
        eventSource.onmessage = (event) => {
            // msg.value=JSON.parse(event.data).living;
			const data = JSON.parse(event.data);
			switch(true){
				case event.data.includes("living"):
					msg.value="几VB收到飞杯v";
					break;
				
			}
        };
        eventSource.onerror = (error) => {
            console.error('EventSource failed:', error);
            eventSource.close();
        };

// 生成随机数据，晚上开灯多
function generateRandomData() {
	// 从中午12点开始，到早上8点结束
	const hours = Array.from({ length: 20 }, (_, i) => {
		let hour = (12 + i) % 24;
		return `${hour.toString().padStart(2, '0')}:00`;
	});
	const livingRoomData = hours.map((_, i) => getRandomLightCount(i));
	const kitchenData = hours.map((_, i) => getRandomLightCount(i));
	const bedroomData = hours.map((_, i) => getRandomLightCount(i));
	const bathroomData = hours.map((_, i) => getRandomLightCount(i));
	const balconyData = hours.map((_, i) => getRandomLightCount(i));

	return {
		hours,
		livingRoomData,
		kitchenData,
		bedroomData,
		bathroomData,
		balconyData
	};
}


// 根据时间生成随机开灯次数，晚上开灯多
function getRandomLightCount(hourIndex) {
	// 晚上18:00 - 06:00
	let hour = (12 + hourIndex) % 24;
	if (hour >= 18 || hour < 6) {
		return Math.floor(Math.random() * 30) + 10; // 10 - 39
	} else {
		return Math.floor(Math.random() * 10) + 1; // 1 - 10
	}
}

onMounted(() => {
	initChart();
});

function initChart() {
	const data = generateRandomData();
	myChart = echarts.init(document.querySelector('.chart-canvas'));

	const option = {
		title: {
			text: 'Hourly Light On Times',
			left: 'center',
			fontSize: 18,
			fontWeight: 'bold'
		},
		tooltip: {
			trigger: 'axis',
			axisPointer: {
				type: 'cross'
			}
		},
		legend: {
			data: ['Living Room', 'Kitchen', 'Bedroom', 'Bathroom', 'Balcony'],
			bottom: '5%',
			textStyle: {
				color: '#333'
			}
		},
		grid: {
			left: '5%',
			right: '5%',
			bottom: '15%',
			containLabel: true
		},
		toolbox: {
			feature: {
				saveAsImage: {}
			}
		},
		xAxis: {
			type: 'category',
			boundaryGap: false,
			data: data.hours,
			axisLabel: {
				rotate: 45,
				fontSize: 12,
				interval: 2 // 间隔显示
			}
		},
		yAxis: {
			type: 'value',
			axisLabel: {
				fontSize: 12
			}
		},
		series: [
			{
				name: 'Living Room',
				type: 'line',
				stack: 'Total',
				data: data.livingRoomData,
				lineStyle: {
					color: '#ff7f50'
				}
			},
			{
				name: 'Kitchen',
				type: 'line',
				stack: 'Total',
				data: data.kitchenData,
				lineStyle: {
					color: '#87cefa'
				}
			},
			{
				name: 'Bedroom',
				type: 'line',
				stack: 'Total',
				data: data.bedroomData,
				lineStyle: {
					color: '#da70d6'
				}
			},
			{
				name: 'Bathroom',
				type: 'line',
				stack: 'Total',
				data: data.bathroomData,
				lineStyle: {
					color: '#32cd32'
				}
			},
			{
				name: 'Balcony',
				type: 'line',
				stack: 'Total',
				data: data.balconyData,
				lineStyle: {
					color: '#6495ed'
				}
			}
		]
	};

	myChart.setOption(option);
}
</script>

<style lang="scss" scoped>
	.container {
		padding: 20px;
		font-size: 14px;
		line-height: 24px;
	}
	.chart-container {
		width: 100%;
		height: 400px;
	}
	.chart-canvas {
		width: 100%;
		height: 100%;
	}
</style>