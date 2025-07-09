<template>
    <view v-if="visible" class="popup-container">
        <view class="popup-content">
            <view class="popup-header">
                <text>{{ title }}</text>
            </view>
            <view class="popup-body">
                <view v-for="(item, index) in array" :key="index" class="item">
                    <text>{{ item }}</text>
                </view>
            </view>
            <view class="popup-footer">
                <button @click="toggleAutoScroll">停止</button>
                <button @click="closePopup">关闭</button>
            </view>
        </view>
    </view>
</template>

<script>
export default {
    props: {
        title: {
            type: String,
            default: '菜单'
        },
        array: {
            type: Array,
            default: () => []
        }
    },
    data() {
        return {
            visible: false,
            autoScrollInterval: null,
            currentIndex: 0
        };
    },
    methods: {
        openPopup() {
            this.visible = true;
            this.startAutoScroll();
        },
        closePopup() {
            this.visible = false;
            this.stopAutoScroll();
        },
        startAutoScroll() {
            this.autoScrollInterval = setInterval(() => {
                this.currentIndex = (this.currentIndex + 1) % this.array.length;
            }, 1000); // 每隔1秒自动滚动到下一个菜单项
        },
        stopAutoScroll() {
            clearInterval(this.autoScrollInterval);
            this.autoScrollInterval = null;
        },
        toggleAutoScroll() {
            if (this.autoScrollInterval) {
                this.stopAutoScroll();
                this.$refs.toggleButton.text = '开始';
            } else {
                this.startAutoScroll();
                this.$refs.toggleButton.text = '停止';
            }
        }
    }
};
</script>

<style scoped>
.popup-container {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background-color: rgba(0, 0, 0, 0.5);
    display: flex;
    justify-content: center;
    align-items: center;
}
.popup-content {
    background-color: #fff;
    padding: 20px;
    border-radius: 10px;
}
.popup-header {
    font-size: 18px;
    font-weight: bold;
    margin-bottom: 10px;
}
.popup-body {
    margin-bottom: 10px;
    overflow: hidden;
}
.item {
    margin-bottom: 5px;
    transition: transform 0.5s ease;
}
.popup-footer {
    text-align: right;
}
</style>