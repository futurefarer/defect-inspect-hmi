# Hello Vision（v0.1）

![CI](https://github.com/futurefarer/defect-inspect-hmi/actions/workflows/ci.yml/badge.svg)

> 目标：用约 70 行代码走通「读图 → 灰度 → 二值化」这条工业视觉最基础的链路。

## 一、前置条件

Ubuntu 22.04 / 24.04，已安装：

```bash
sudo apt install -y build-essential cmake libopencv-dev
```

## 二、构建与运行

```bash
mkdir -p build
cd build
cmake ..
make -j

./hello_vision              # 用内置合成图
./hello_vision 你的图片.jpg  # 用真实图片
```

## 三、预期结果

1. 终端输出图片尺寸、通道数、缺陷像素占比；
2. 弹出三个窗口：原图、灰度图、二值化图（按任意键关闭）；
3. 当前目录下生成 `out/gray.png` 和 `out/binary.png`。

## 四、逐行理解清单（必须能讲清）

| 代码 | 你要能回答 |
|---|---|
| `cv::Mat` | 它是什么？为什么图像用它存？ |
| `cv::imread(..., IMREAD_COLOR)` | 读进来是什么颜色顺序？为什么？ |
| `cv::cvtColor(..., COLOR_BGR2GRAY)` | 灰度是怎么算出来的？ |
| `cv::threshold(..., THRESH_BINARY_INV)` | 为什么这里是 INV 而不是普通 BINARY？ |
| `cv::countNonZero(binary)` | 它统计的是什么？怎么变成“缺陷占比”？ |
| `cv::imwrite` / `imshow` / `waitKey` | 保存和显示分别适用什么场景？ |
| `find_package(OpenCV)` + `target_link_libraries` | CMake 怎么找到 OpenCV 并把库链接进来？ |

## 五、三个动手实验（做完才算 L2）

### 实验 1：改阈值
把 `thresholdValue` 分别改成 `60`、`200`，重新编译运行：

- 二值化图有什么变化？
- 浅灰色方块（亮度约 200）在哪个阈值下被检测到？为什么？
- 记录：现象 → 原因 → 结论。

### 实验 2：换模式
把 `THRESH_BINARY_INV` 改成 `THRESH_BINARY`：

- 黑白的含义发生了什么反转？
- 如果后续用 `findContours` 找缺陷，哪种模式更合适？

### 实验 3：真实图片
找一张有瑕疵的物体照片（或者用手机拍一张）：

- 直接灰度化+固定阈值能检测出来吗？
- 为什么工业现场必须控制光源？（这是面试高频问题）

## 六、本步验收（L3 标准）

- [ ] 不看代码，能按顺序讲清每一步在做什么
- [ ] 能预测阈值变化后的结果，再运行验证
- [ ] 能解释“白底黑缺陷为什么用 `THRESH_BINARY_INV`”
- [ ] 完成三个实验，并写下 5 行记录

> 完成后进入下一步：用 Qt 做一个窗口，把结果显示在界面里。

## 七、开发流程（企业协作）

1. 每个任务对应一个 GitHub Issue
2. 从 `main` 拉分支：`git switch -c feature/xxx`
3. 本地开发并自测
4. 提交：`git add -A && git commit -m "feat: ..."`
5. 推送：`git push -u origin feature/xxx`
6. 在 GitHub 开 Pull Request
7. CI 通过 + Review 通过后合并

