<template>
  <view class="zui-joystick">
    <view
      ref="eleWrapper"
      :class="{ 'zui-joystick-wrapper': 1, debug }"
      :style="style"
    >
      <div class="background">
        <image class="base-img" :src="theme.base" mode="aspectFit"></image>
      </div>
      <div
        class="handle"
        :style="handleStyle"
        @touchstart.prevent="dragStart"
        @touchmove.prevent="dragMoving"
        @touchcancel.prevent="dragCancel"
        @touchend.prevent="dragEnd"
      >
        <image class="handle-img" :src="theme.handle" mode="aspectFit"> </image>
      </div>

      <template v-if="enableArrows">
        <image class="arrow top" mode="top" :src="theme.arrowTop"></image>
        <image class="arrow right" mode="right" :src="theme.arrowRight"></image>
        <image
          class="arrow bottom"
          mode="bottom"
          :src="theme.arrowBottom"
        ></image>
        <image class="arrow left" mode="left" :src="theme.arrowLeft"></image>
      </template>

      <view v-if="debug" class="pointer" />
    </view>
  </view>
</template>

<script>
const TWO_PI = 2 * Math.PI;

export default {
  props: {
    /**
     * 摇杆尺寸
     */
    size: {
      type: Number,
      default: 150,
    },

    /**
     * 握把尺寸
     */
    stickSize: {
      type: Number,
      default: 0.5,
    },

    /**
     * 上、下、左、右 4 个方向的角度范围
     *
     * 默认：30度，可调节范围为 [10, 40]
     */
    axisRange: {
      type: Number,
      default: 30,
    },

    /**
     * 握把活动半径
     *
     * 百分比, 默认全范围活动
     */
    stickRange: {
      type: Number,
      default: undefined,
    },

    /**
     * 事件触发频率
     *
     * 单位: 毫秒
     */
    eventFreq: {
      type: Number,
      default: 10,
    },

    theme: {
      type: Object,
      default: () => ({}),
    },

    /**
     * 是否显示箭头
     */
    enableArrows: Boolean,

    /**
     * 箭头尺寸
     *
     * 仅当 enableArrows = true 时有效
     */
    arrowSize: {
      type: Number,
      default: 24,
    },

    /**
     * 开启 debug 模式
     */
    debug: Boolean,
  },

  data() {
    return {
      actived: false,
      centerX: 0,
      centerY: 0,
      tx: 0,
      ty: 0,
      //
      fixUniHead: 0,

      direction: "none",

      // 防止事件过于频繁触发
      _eventLock: false,
      _eventLockIId: 0,
      //
      // Test config
      //
      rotate: 0,
      radius: 0,
    };
  },

  computed: {
    /**
     * 算上方向键后的尺寸
     */
    outterSize() {
      return this.enableArrows ? this.arrowSize * 2 + this.size : this.size;
    },

    style() {
      const style = {
        width: `${this.outterSize}px`,
        height: `${this.outterSize}px`,
        "--zui-joystick-size": `${this.size}px`,
      };

      if (this.debug) {
        style["--zui-joystick-pointer-left"] = `${this.centerX}px`;
        style["--zui-joystick-pointer-top"] = `${this.centerY}px`;
        style["--zui-joystick-pointer-rotate"] = `rotateZ(${this.rotate}rad)`;
        style["--zui-joystick-pointer-length"] = `${this.radius}px`;
      }

      if (this.enableArrows) {
        style["--zui-joystick-arrow-size"] = `${this.arrowSize}px`;
      } else {
        style["--zui-joystick-arrow-size"] = `0`;
      }

      return Object.keys(style)
        .map((key) => `${key}:${style[key]}`)
        .join(";");
    },

    handleStyle() {
      const style = {
        transform: `translate(${this.tx}px, ${this.ty}px)`,
        width: `${this.stickSize * this.size}px`,
        height: `${this.stickSize * this.size}px`,
        "--zui-joystick-handle-size": `${this.stickSize * this.size}px`,
      };

      return Object.keys(style)
        .map((key) => `${key}:${style[key]}`)
        .join(";");
    },

    // theme() {
    //   return {
    //     handle:
    //   }
    // },

    maxInnerRadius() {
      if (this.stickRange) return (this.stickRange * this.size) / 2 - 2;

      const r = ((1 - this.stickSize) / 2) * this.size - 2;
      return r;
    },

    axisRangeAry() {
      const half = Math.min(Math.max(10, this.axisRange), 40) / 2;
      return [
        // 大于比较
        { name: "right", val: 360 - half },
        { name: "top-right", val: 270 + half },
        { name: "top", val: 270 - half },
        { name: "top-left", val: 180 + half },
        { name: "left", val: 180 - half },
        { name: "bottom-left", val: 90 + half },
        { name: "bottom", val: 90 - half },
        { name: "bottom-right", val: half },
        { name: "right", val: 0 },
      ];
    },
  },

  mounted() {
    this.updateCenter();
  },

  methods: {
    updateCenter() {
      // #ifdef H5
      const rect = this.$refs.eleWrapper.$el.getBoundingClientRect();
      this.centerX = rect.left + rect.width / 2;
      this.centerY = rect.top + rect.height / 2;

      if (typeof document !== "undefined") {
        const head = document.querySelector(".uni-page-head");
        this.fixUniHead = !!head ? 44 : 0;
      }
      // #endif

      // #ifndef H5
      const query = uni.createSelectorQuery().in(this);
      query
        .select(".zui-joystick-wrapper")
        .fields({ size: true, rect: true })
        .exec((rst) => {
          if (!rst) return;
          if (!rst[0]) return;
          this.centerX = rst[0].left + rst[0].width / 2;
          this.centerY = rst[0].top + rst[0].height / 2;
        });
      // #endif
    },

    tickCount(count) {
      if (count === 0) {
        this.skipCalc = false;
        return;
      }

      this.$nextTick(() => {
        this.tickCount(count--);
      });
    },

    calucateAngle(nx, ny) {
      const dx = nx - this.centerX;
      const dy = ny - this.centerY;
      const radius = Math.sqrt(dx * dx + dy * dy);
      const radian = (Math.atan2(dy, dx) + TWO_PI) % TWO_PI;
      const degree = ((radian * 180) / Math.PI + 360) % 360;

      const direction = this.axisRangeAry.find(
        (item) => degree >= item.val
      ).name;
      const state = {
        radius,
        radian,
        degree,
        direction,
        strength: Math.pow(radius / ((this.size * 0.95) / 2), 1.225),
      };

      if (this.debug) {
        this.rotate = radian;
        this.radius = radius;
      }

      return state;
    },

    getTouchPoint(evt, inRange) {
      let dis = Number.MAX_SAFE_INTEGER;
      let idx = -1;
      evt.touches.forEach((p, _idx) => {
        const dx = p.clientX - this.centerX;
        const dy = p.clientY - this.centerY;
        const _dis = dx * dx + dy * dy;
        if (_dis < dis) {
          dis = _dis;
          idx = _idx;
        }
      });

      const rad = Math.sqrt(dis)
      if (inRange && rad > (this.size / 2)) return null

      return evt.touches[idx];
    },

    dragStart(evt) {
      const p = this.getTouchPoint(evt, true);
      if (!p) return;

      this.actived = true;
      this.$emit("active");
      this.dragMoving(evt);
    },

    dragMoving(evt) {
      if (!this.actived) return;

      const p = this.getTouchPoint(evt);
      if (!p) return;

      const change = this.calucateAngle(
        p.clientX,
        /**
         * H5 环境下, 如果启用了 uni 的标题栏, 则需要将 clientY + 44
         */
        p.clientY + this.fixUniHead
      );

      const radius =
        change.radius > this.maxInnerRadius
          ? this.maxInnerRadius
          : change.radius;
      const radian = change.radian + 90 * (Math.PI / 180);
      this.tx = Math.sin(radian) * radius;
      this.ty = -Math.cos(radian) * radius;

      this.fireEvent(change);

      if (this.direction !== change.direction) {
        this.$emit("direction", {
          ...change,
          from: this.direction,
          to: change.direction,
        });
        this.direction = change.direction;
      }
    },

    dragCancel() {
      this.reset();
    },

    dragEnd() {
      this.reset();
    },

    reset() {
      this.actived = false;
      this.radius = this.tx = this.ty = 0;
      this.direction = "none";

      if (this._eventLockIId) {
        clearTimeout(this._eventLockIId);
		this._eventLock = false;
      }
      this.$emit("change", {
        radius: 0,
        radian: 0,
        degree: 0,
        direction: "none",
      });

      this.$emit("direction", {
        from: this.direction,
        to: "none",
      });
      this.$emit("deactive");
    },

    fireEvent(change) {
      if (this._eventLock) return;
      this._eventLock = true;

      this.$emit("change", change);

      this._eventLockIId = setTimeout(() => {
        this._eventLock = false;
      }, this.eventFreq);
    },
  },
};
</script>

<style scoped>
.zui-joystick {
  position: relative;
  border-radius: 50%;
  display: flex;
}

.zui-joystick-wrapper {
  position: relative;
}

.zui-joystick-wrapper .background {
  position: relative;
  top: var(--zui-joystick-arrow-size);
  left: var(--zui-joystick-arrow-size);
  width: var(--zui-joystick-size);
  height: var(--zui-joystick-size);
  border-radius: 50%;
  border: 1px solid #ffffff;
  box-shadow: inset 0 1px 3px 3px rgba(132, 132, 132, 0.14);
  background-image: linear-gradient(180deg, #b7b7b7 0%, #f4f4f4 100%);
  box-sizing: border-box;
}

.base-img {
  width: var(--zui-joystick-size);
  height: var(--zui-joystick-size);
}

.handle {
  border-radius: 50%;
  display: flex;
  position: absolute;
  left: 0;
  right: 0;
  top: 0;
  bottom: 0;
  margin: auto;

  box-shadow: 0 5px 6px 0 rgba(0, 0, 0, 0.64);
  transform-origin: center;
  transition: all 0.1s cubic-bezier(0.175, 0.885, 0.32, 1.275);
  background-image: linear-gradient(180deg, #fcfcfc 0%, #cfcfcf 100%);
}

.handle-img {
  width: var(--zui-joystick-handle-size);
  height: var(--zui-joystick-handle-size);
}

.arrow {
  position: absolute;
  z-index: 50;
  width: var(--zui-joystick-arrow-size);
  height: var(--zui-joystick-arrow-size);
  background-size: contain;
  background-repeat: no-repeat;
}

.arrow.top {
  top: 0%;
  left: 50%;
  width: var(--zui-joystick-size);
  transform: translate(-50%, 0%);
}
.arrow.right {
  top: 50%;
  right: 0%;
  height: var(--zui-joystick-size);
  transform: translate(0%, -50%);
}
.arrow.bottom {
  bottom: 0%;
  left: 50%;
  width: var(--zui-joystick-size);
  transform: translate(-50%, 0);
}
.arrow.left {
  top: 50%;
  left: 0;
  height: var(--zui-joystick-size);
  transform: translate(0, -50%);
}

.pointer {
  display: none;
  position: fixed;
  left: var(--zui-joystick-pointer-left);
  top: var(--zui-joystick-pointer-top);
  width: var(--zui-joystick-pointer-length);
  height: 1px;
  transform-origin: left center;
  transform: var(--zui-joystick-pointer-rotate);
  background-color: #f0f;
}

.debug .pointer {
  display: block;
}
.debug .arrow {
  box-sizing: border-box;
  border: 1px solid #f0f;
}
.debug .handle {
  border: 1px solid #0ff;
}
</style>
