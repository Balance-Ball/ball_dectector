# ball-detector

`ball-detector` 是基于C++17 + Opencv的寻球程序，其作用是在图像/视频流中通过Blob算法寻球并通过Linux串口设备将数据返回给下位机。其实现的应用场景是用于复刻平面小球平衡。

## Build

你需要在你的电脑上安装好以下工具链

- 支持C++17+的C++编译器
- CMake 3.23+
- Opencv4/5
- Linux系统平台

```bash
# Build with cmake in console
git clone https://github.com/Balance-Ball/ball_dectector.git
cd ball_dectector
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Usage

```bash
ball_detector --help
   --source <image or video device>
   --display <true/false>
```

启用该应用的视觉处理功能，还需要搭配一个json配置文件，用于提供视觉处理的必须或可配置的参数

```jsonc
{
    "hsv_begin": [20, 60, 100],
    "hsv_end":   [40, 90, 255],

    // optional config fields(All of them are default in this example json configration)
    "resolution": [640, 480],
    "video_dev_index": 0,
    // open and close kernal size must be odd
    "open_kernal_size": 3,
    "close_kernal_size": 9,
    "circle_min_area": 200,
    "circle_max_area": 2000,

    // circularity must be smaller than 1
    "min_circularity": 0.8
}
```

## Implementation

该程序算法可以简单分为下列几个阶段

- 缩放并转换为HSV
- 二值化
- 形态学去噪
- 寻找二值化图像中色块的闭合轮廓
- 从筛选后的轮廓中进行圆判定
