# zui-joystick 摇杆模拟器

一款漂亮的摇杆模拟器🕹。控制灵活，输出 8 个方向变量，旋转角度，模拟力度等参数。

丰富的控制参数。支持主题设置，轻松更换摇杆样式。

适合游戏，监控控制等场景。



## 🍓 立即使用

```VUE
<zui-joystick @change="onJoyChange" @direction="onDirectionChange" />
```

## 在线演示

**[💻 点我在浏览器里预览 https://uni.imgozi.cn/zui-joystick/](https://uni.imgozi.cn/zui-joystick/?utm_source=uni-plugin-market&utm_medium=readme&utm_campaign=zui-joystick&utm_id=uni-plugin)**

PS: 启动浏览器预览需要打开手机模器

**[📱 扫码体验](https://uni.imgozi.cn/zui-joystick/?utm_source=uni-plugin-market&utm_medium=readme&utm_campaign=zui-joystick&utm_id=uni-plugin)**

<img src="https://uni.imgozi.cn/zui-joystick/static/preview-qr.png?_rnd" width="128" />


## 🍊 参数

| 参数 | 类型 | 说明 |
|---|---|:---|
| size | number | 摇杆组件尺寸 |
| stickSize | number  | 握把尺寸。百分比，相对组件尺寸 |
| axisRange | number | 上、下、左、右 4 个方向的角度范围。[10, 40] |
| stickRange | number | 摇杆活动范围。百分比，相对组件尺寸 |
| eventFreq | number | 事件触发频率。默认 10 毫秒一次 |
| enableArrows | boolean | 是否显示箭头 |
| arrowSize | number | 箭头尺寸 |
| theme | object | 样式设置。对象数据结构见下方 |
| debug | boolean | 开启调试模式，该模式开启后会显示一条紫色线条以跟踪摇杆 |

### theme

```javascript
{
  base: '底图.png',
  handle: '握把.png',
  arrowTop: '上箭头.png',
  arrowRight: '右箭头.png',
  arrowBottom: '下箭头.png',
  arrowLeft: '左箭头.png',
}
```

图片尺寸范围的设定见右侧预览图。



#### 关于 支付宝小程序 的兼容性问题

由于支付宝小程序对 image 缩放模式支持有问题，在设计主题图片的时候需要按实际需求尺寸导出图片。

比如箭头设置的尺寸是24，导出的左右箭头图片的宽度就必须是 24，同理上下箭头的高度必须是24。





## 🍒 事件

以下所列事件按触发顺序列出

### @active=onActive() => void

开始操作时触发

```javascript
// 无参数
```

### @change=onChange(chg) => void

摇杆推动时触发，事件触发间隔可通过参数 `eventFreq` 调整

旋转角度计量以右方为起点，顺时针方向

```javascript
{
  direction: 'top',   // 方向
  radius: 0,   // 半径
  radian: 0,   // 弧度, [0, 2*PI]
  degree: 0,   // 角度, [0, 360]
  strength: 0,   // 模拟力度, [0, ∞]
}
```

`direction` 取值范围: `'none' | 'top' | 'top-right' | 'right' | 'bottom-right' | 'bottom' | 'bottom-left' | 'left' | 'top-left'`

### @direction=onDirectionChange(chg) => void

当方向发生变化时触发

```
{
  from: 'top', // 变化之前的方向
  to: 'top-right', // 变化之后的方向, 即当前方向
  direction: 'top',   // 方向
  radius: 0,   // 半径
  radian: 0,   // 弧度, [0, 2*PI]
  degree: 0,   // 角度, [0, 360]
  strength: 0,   // 模拟力度, [0, ∞]
}
```

### @deactive=onDeactive() => void

结束操作时触发

```javascript
// 无参数
```


## 已知问题

同一页面展示2个或多个控制器时，如果两个组件离得太近，在使用时会发生冲突。


## 🍓 支持

如果组件对您有帮助，请不吝打赏。肥宅快乐水🥤是创作动力！🥤🥤🥤

<span class="banner">
<span class="surport">
<a class="btn btn-support " data-toggle="modal" data-target="#support_modal" style="border: 1px solid #ec4d4d;letter-spacing: 1px;">
  🍓🍇🍉  喜欢就打赏一下  🍒🍑🥭
</a>
</span>
</span>
