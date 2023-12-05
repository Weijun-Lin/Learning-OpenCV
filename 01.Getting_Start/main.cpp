#include <iostream>
// opencv 的基础模块
#include <opencv2/core.hpp>
// opencv 读写模块
#include <opencv2/imgcodecs.hpp>
// opencv 窗口显示模块
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "usage: GettingStart <Image_Path>" << endl;
        return -1;
    }
    /*  读取图片 imread(file_path, flags)
        flags 一般可取：
        - IMREAD_COLOR      8-bit BGR 三通道图片，默认值
        - IMREAD_UNCHANGED  按原样加载图片，opencv 不作额外处理
        - IMREAD_GRAYSCALE  读取灰度图像，仅一个通道，彩色图会被转换
    */
    Mat img = imread(argv[1], IMREAD_COLOR);
    if (img.empty()) {
        cerr << "No image data" << endl;
        return -1;
    }

    // 创建一个名为 Getting Start 的窗口
    namedWindow("Getting Start", WINDOW_AUTOSIZE);
    // 将图片显示在 Getting Start 窗口上
    imshow("Getting Start", img);

    // 等待用户按健，返回按下的字符
    int k = waitKey(0);
    if (k == 's') {
        // 按下 s 保存图片
        imwrite("temp.jpeg", img);
    }
    
    return 0;
}