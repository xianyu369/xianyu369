<template>
  <view class="content">
    <view class="page-title"> VUE 页面示例 </view>
    <view class="joys">
      <zui-joystick
        enable-arrows
        :debug="debug"
        :size="100"
        :theme="theme1"
        :event-freq="30"
        :stick-range="0.25"
        @change="onJoy1Change"
      />
    </view>
    <view class="status">
      <view class="prop"
        ><text class="label">.direction =</text> {{ joyEvent.direction }}</view
      >
      <view class="prop"
        ><text class="label">.radius =</text>
        {{ (joyEvent.radius || 0).toFixed(4) }}</view
      >
      <view class="prop"
        ><text class="label">.degree =</text>
        {{ (joyEvent.degree || 0).toFixed(4) }}</view
      >
      <view class="prop"
        ><text class="label">.radian =</text>
        {{ (joyEvent.radian || 0).toFixed(4) }}</view
      >
      <view class="prop"
        ><text class="label">.strength =</text>
        {{ (joyEvent.strength || 0).toFixed(4) }}</view
      >
    </view>
    <view class="config">
      <view>Debug 模式</view>
      <switch @change="toggleDebugger" />
    </view>
  </view>
</template>

<script>
import arrowTopNormal from "@/static/images/arrow-up-normal.png";
import arrowTopActive from "@/static/images/arrow-up-active.png";
import arrowRightNormal from "@/static/images/arrow-right-normal.png";
import arrowRightActive from "@/static/images/arrow-right-active.png";
import arrowBottomNormal from "@/static/images/arrow-down-normal.png";
import arrowBottomActive from "@/static/images/arrow-down-active.png";
import arrowLeftNormal from "@/static/images/arrow-left-normal.png";
import arrowLeftActive from "@/static/images/arrow-left-active.png";
import joyBase from "@/static/images/background.png";
import joyHandle from "@/static/images/sticker-normal.png";

export default {
  components: {},

  data() {
    return {
      title: "Hello",
      activeDirection1: "",
      theme1: {
        base: joyBase,
        handle: joyHandle,
        arrowTop: arrowTopNormal,
        arrowRight: arrowRightNormal,
        arrowBottom: arrowBottomNormal,
        arrowLeft: arrowLeftNormal,
      },
      activeDirection2: "",
      theme2: {
        base: joyBase,
        handle: joyHandle,
        arrowTop: arrowTopNormal,
        arrowRight: arrowRightNormal,
        arrowBottom: arrowBottomNormal,
        arrowLeft: arrowLeftNormal,
      },
      joyEvent: {},
      debug: false,
    };
  },

  computed: {},

  onLoad() {},

  methods: {
    onJoy1Change(chg) {
      const dir = chg.direction;
      (this.theme1.arrowTop = dir === "top" ? arrowTopActive : arrowTopNormal),
        (this.theme1.arrowRight =
          dir === "right" ? arrowRightActive : arrowRightNormal),
        (this.theme1.arrowBottom =
          dir === "bottom" ? arrowBottomActive : arrowBottomNormal),
        (this.theme1.arrowLeft =
          dir === "left" ? arrowLeftActive : arrowLeftNormal),
        (this.activeDirection1 = dir);

      if (chg.direction !== "none") this.joyEvent = chg;
    },
    onJoy2Change(chg) {
      const dir = chg.direction;
      (this.theme2.arrowTop = dir === "top" ? arrowTopActive : arrowTopNormal),
        (this.theme2.arrowRight =
          dir === "right" ? arrowRightActive : arrowRightNormal),
        (this.theme2.arrowBottom =
          dir === "bottom" ? arrowBottomActive : arrowBottomNormal),
        (this.theme2.arrowLeft =
          dir === "left" ? arrowLeftActive : arrowLeftNormal),
        (this.activeDirection2 = dir);

      if (chg.direction !== "none") this.joyEvent = chg;
    },
    toggleDebugger(evt) {
      this.debug = evt.detail.value
    },
  },
};
</script>

<style>
.content {
  display: flex;
  flex-direction: column;
  align-items: center; /* 水平居中 */
  justify-content: center; /* 垂直居中 */
  height: 100vh; /* 使用视口高度，确保占满整个屏幕 */
  padding: 20px;
  box-sizing: border-box;
}

.page-title {
  font-size: 24px;
  font-weight: bold;
  margin-bottom: 20px;
}

.joys {
  margin-bottom: 20px;
}

.status {
  margin-bottom: 20px;
}

.prop {
  margin-bottom: 10px;
}

.label {
  font-weight: bold;
}

.config {
  display: flex;
  align-items: center;
}

switch {
  margin-left: 10px;
}
</style>