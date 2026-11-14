// v0.1 — 最小视觉程序：读图 → 灰度 → 二值化 → 显示/保存
//
// 目标：理解工业视觉最基础的三步，并学会用 CMake 构建。
// 编译运行（Ubuntu）：
//   mkdir -p build && cd build
//   cmake .. && make -j
//   ./hello_vision              # 使用内置合成图
//   ./hello_vision 图片路径      # 使用真实图片

#include <opencv2/opencv.hpp>

#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

// 没有传入图片时，生成一张「白底 + 黑色缺陷」的合成图，方便第一次运行。
// 注意：后面那块浅灰色的方块，阈值 128 可能检测不到——这正是我们要观察的现象。
static cv::Mat makeSyntheticImage() {
    cv::Mat image(480, 640, CV_8UC3, cv::Scalar(235, 235, 235));  // 480 行 × 640 列，3 通道

    cv::circle(image, cv::Point(160, 160), 40, cv::Scalar(30, 30, 30), cv::FILLED);  // 大缺陷
    cv::circle(image, cv::Point(360, 220), 25, cv::Scalar(30, 30, 30), cv::FILLED);  // 小缺陷
    cv::rectangle(image, cv::Rect(420, 300, 120, 80), cv::Scalar(30, 30, 30), cv::FILLED);
    cv::rectangle(image, cv::Rect(80, 340, 100, 60), cv::Scalar(200, 200, 200), cv::FILLED);

    return image;
}

int main(int argc, char** argv) {
    // 1. 获取图像：命令行传入，或使用内置合成图
    cv::Mat image;
    if (argc >= 2) {
        image = cv::imread(argv[1], cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cerr << "无法读取图片: " << argv[1] << '\n';
            return 1;
        }
        std::cout << "读取图片: " << argv[1] << '\n';
    } else {
        image = makeSyntheticImage();
        std::cout << "未提供图片，使用内置合成图。\n";
    }

    std::cout << "尺寸: " << image.cols << "x" << image.rows
              << "，通道数: " << image.channels() << '\n';

    // 2. 灰度化：把 BGR 三通道变成单通道亮度图
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // 3. 二值化：亮度低于阈值的像素变白（255），其余变黑（0）
    //    工业检测中常用 INV 模式：白底上的黑色缺陷会被提取成白色前景
    cv::Mat binary;
    const double thresholdValue = 128.0;
    cv::threshold(gray, binary, thresholdValue, 255, cv::THRESH_BINARY_INV);

    // 统计暗像素（缺陷）占比
    const double defectRatio =
        static_cast<double>(cv::countNonZero(binary)) / binary.total();
    std::cout << "阈值: " << thresholdValue
              << "，缺陷像素占比: " << defectRatio * 100.0 << "%\n";

    // 4. 保存结果：即使没有图形界面也能检查结果
    fs::create_directories("out");
    if (!cv::imwrite("out/gray.png", gray) ||
        !cv::imwrite("out/binary.png", binary)) {
        std::cerr << "保存结果失败\n";
        return 1;
    }
    std::cout << "已保存: out/gray.png, out/binary.png\n";

    // 5. 显示窗口（需要图形界面；没有界面时只保存文件）
    try {
        cv::imshow("v0.1 原图", image);
        cv::imshow("v0.1 灰度", gray);
        cv::imshow("v0.1 二值化（缺陷为白色）", binary);
        std::cout << "按任意键关闭窗口...\n";
        cv::waitKey(0);
    } catch (const cv::Exception&) {
        std::cout << "无法打开窗口（可能没有图形界面），结果已保存到 out/。\n";
    }

    return 0;
}
