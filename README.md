# Qt YOLOv11 目标检测应用

一个基于 Qt 框架和 YOLOv11 模型的单张图片目标检测应用程序。该应用提供了友好的图形界面，支持选择图片、运行 YOLO 推理、可视化检测结果并显示详细的检测报告。

## 📋 项目简介

本项目将 Ultralytics YOLOv11 目标检测模型集成到 Qt 应用程序中，实现了：
- 图形化的图片选择界面
- 实时的 YOLO 目标检测
- 检测结果的可视化展示（边界框、类别标签、置信度）
- 详细的检测报告表格

## ✨ 功能特性

- 🖼️ **图片选择**：支持 PNG、JPG、JPEG、BMP 格式图片
- 🎯 **目标检测**：基于 YOLOv11 模型进行实时目标检测
- 📊 **结果可视化**：在图片上绘制检测框和标签
- 📋 **检测报告**：以表格形式显示所有检测对象的详细信息
- 🎨 **用户界面**：直观的 Qt 图形界面，左右分屏显示输入和输出

## 🔧 环境要求

### 必需软件

- **Qt**: Qt 5.x 或 Qt 6.x
- **CMake**: 3.16 或更高版本
- **C++ 编译器**: 
  - Windows: MSVC 2019 或更高版本（推荐）
  - 或 MinGW（需要 MinGW 版本的 OpenCV）
- **OpenCV**: 4.8.1（已包含在项目中）

### 系统要求

- Windows 10/11 (64-bit)
- 至少 4GB RAM
- 支持 OpenGL 的显卡（可选，用于加速）

## 📦 依赖项

- **Qt**: Widgets 模块
- **OpenCV**: 4.8.1（包含在 `opencv/` 目录中）
- **YOLOv11 模型**: `yolo11n.onnx`（已包含在项目根目录）

## 📁 项目结构

```
qt_yolov11/
├── CMakeLists.txt          # CMake 构建配置文件
├── main.cpp                # 程序入口
├── mainwindow.h            # 主窗口头文件
├── mainwindow.cpp          # 主窗口实现
├── mainwindow.ui           # Qt Designer UI 文件
├── inference.h             # YOLO 推理类头文件
├── inference.cpp           # YOLO 推理类实现
├── yolo11n.onnx            # YOLOv11 Nano 模型文件
├── opencv/                 # OpenCV 库文件目录
│   └── build/
│       ├── include/        # OpenCV 头文件
│       ├── x64/vc16/       # MSVC 编译的库文件
│       │   ├── lib/        # 静态库文件
│       │   └── bin/        # 动态库文件（DLL）
│       └── ...
├── images/                 # 测试图片目录
└── build/                  # 构建输出目录（自动生成）
```

## 🚀 编译说明

### 使用 Qt Creator

1. **打开项目**
   - 启动 Qt Creator
   - 选择 `文件` → `打开文件或项目`
   - 选择项目根目录下的 `CMakeLists.txt`

2. **配置项目**
   - Qt Creator 会自动检测 CMake 配置
   - 选择构建套件（Kit）：
     - **推荐**: MSVC 2019 64-bit（与 OpenCV vc16 版本匹配）
     - **或**: MinGW 64-bit（需要 MinGW 版本的 OpenCV）
   - 选择构建类型：Debug 或 Release
   - 点击 `配置项目`

3. **构建项目**
   - 点击 `构建` → `构建项目 qt_yolov11`
   - 或使用快捷键 `Ctrl+B`

### 使用命令行

```bash
# 创建构建目录
mkdir build
cd build

# 配置 CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . --config Release
```

### 注意事项

⚠️ **编译器兼容性**：
- 项目中的 OpenCV 库是使用 MSVC 2019 (vc16) 编译的
- 如果使用 MinGW 编译器，需要：
  1. 下载或编译 MinGW 版本的 OpenCV
  2. 修改 `CMakeLists.txt` 中的 `OpenCV_LIB_DIR` 路径

## 📖 使用说明

### 启动应用

1. 编译完成后，可执行文件位于 `build/` 目录下
2. 运行前确保以下文件在同一目录：
   - 可执行文件（`qt_yolov11.exe`）
   - `yolo11n.onnx` 模型文件
   - OpenCV DLL 文件（`opencv/build/x64/vc16/bin/opencv_world481.dll` 或 `opencv_world481d.dll`）

### 使用步骤

1. **选择图片**
   - 点击 `选择图片` 按钮
   - 在文件对话框中选择要检测的图片（支持 PNG、JPG、JPEG、BMP）
   - 选中的图片会显示在左侧的"输入图像"区域

2. **开始检测**
   - 点击 `开始检测` 按钮
   - 程序会运行 YOLO 推理，检测图片中的目标对象
   - 检测结果会显示在右侧的"输出结果"区域

3. **查看结果**
   - **可视化结果**：右侧预览区域显示带有检测框和标签的图片
   - **检测报告**：底部表格显示所有检测对象的详细信息：
     - 类别：检测到的对象类别（如 person, car, dog 等）
     - 置信度：检测的置信度分数（0-1）
     - X, Y：边界框左上角坐标
     - W, H：边界框的宽度和高度

### 支持的检测类别

YOLOv11 可以检测 80 种常见对象类别，包括：
- 人物：person
- 交通工具：car, bus, truck, motorcycle, bicycle, airplane, boat
- 动物：cat, dog, bird, horse, cow, sheep, etc.
- 家具：chair, couch, bed, dining table
- 电子产品：tv, laptop, mouse, keyboard, cell phone
- 以及其他常见物体...

完整类别列表请参考 [COCO 数据集类别](https://cocodataset.org/#explore)

## 🔍 技术细节

### YOLO 推理流程

1. **图像预处理**
   - 将输入图像转换为 OpenCV Mat 格式
   - 使用 Letterbox 方法将图像调整为 640x640（保持宽高比）
   - 归一化像素值到 [0, 1] 范围

2. **模型推理**
   - 使用 OpenCV DNN 模块加载 ONNX 模型
   - 运行前向传播获取检测结果
   - 支持 CPU 和 CUDA 加速（需配置）

3. **后处理**
   - 解析模型输出（支持 YOLOv5 和 YOLOv8/v11 格式）
   - 应用非极大值抑制（NMS）去除重复检测
   - 将坐标转换回原始图像尺寸

4. **结果可视化**
   - 在图像上绘制彩色边界框
   - 显示类别名称和置信度标签
   - 为每个检测对象分配随机颜色

### 关键参数

- **模型输入尺寸**: 640x640
- **置信度阈值**: 0.25
- **NMS 阈值**: 0.50
- **分数阈值**: 0.45

这些参数可以在 `inference.h` 中修改。

## ⚠️ 常见问题

### 1. 链接错误：undefined reference to cv::Mat

**原因**: OpenCV 库未正确链接

**解决方案**:
- 确保使用与 OpenCV 库匹配的编译器（MSVC 对应 vc16）
- 检查 `CMakeLists.txt` 中的库路径是否正确
- 清理构建目录后重新配置

### 2. 运行时错误：找不到模型文件

**原因**: `yolo11n.onnx` 不在可执行文件目录

**解决方案**:
- 将 `yolo11n.onnx` 复制到可执行文件所在目录
- 或修改 `mainwindow.cpp` 中的模型路径

### 3. 运行时错误：找不到 OpenCV DLL

**原因**: OpenCV 动态库未在系统路径中

**解决方案**:
- 将 `opencv/build/x64/vc16/bin/` 目录添加到系统 PATH
- 或将 DLL 文件复制到可执行文件目录

### 4. 检测结果为空

**可能原因**:
- 图片中没有可检测的对象
- 置信度阈值设置过高
- 模型文件损坏

**解决方案**:
- 尝试使用包含常见对象的测试图片
- 降低 `modelScoreThreshold` 参数值

## 🛠️ 开发说明

### 修改检测参数

在 `inference.h` 中可以调整以下参数：

```cpp
float modelConfidenceThreshold {0.25};  // 置信度阈值
float modelScoreThreshold      {0.45};  // 分数阈值
float modelNMSThreshold        {0.50};  // NMS 阈值
```

### 启用 CUDA 加速

在 `mainwindow.cpp` 的构造函数中，将 `false` 改为 `true`：

```cpp
m_inference = new Inference(modelPath.toStdString(), cv::Size(640, 640), "", true);
```

**注意**: 需要 OpenCV 支持 CUDA，并且系统安装了 NVIDIA GPU 驱动。

### 更换模型

1. 下载其他 YOLOv11 模型（如 `yolo11s.onnx`, `yolo11m.onnx` 等）
2. 替换项目根目录下的 `yolo11n.onnx`
3. 或修改 `mainwindow.cpp` 中的模型路径

## 📄 许可证

本项目中的 YOLO 推理代码基于 Ultralytics 的 AGPL-3.0 许可证。

## 🙏 致谢

- [Ultralytics YOLOv11](https://github.com/ultralytics/ultralytics) - YOLO 模型
- [OpenCV](https://opencv.org/) - 计算机视觉库
- [Qt](https://www.qt.io/) - 跨平台 GUI 框架

## 📝 更新日志

### v0.1 (当前版本)
- ✅ 集成 YOLOv11 目标检测功能
- ✅ 实现 Qt 图形界面
- ✅ 支持单张图片检测
- ✅ 检测结果可视化
- ✅ 检测报告表格显示

## 📧 联系方式

如有问题或建议，欢迎提交 Issue 或 Pull Request。

---

**注意**: 本项目仅供学习和研究使用。商业使用请遵守相关许可证要求。

