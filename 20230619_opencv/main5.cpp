// 对摄像头识别二维码
// g++ -std=c++17 main5.cpp -o main `pkg-config --libs opencv4`

#include <opencv2/imgproc/types_c.h>

#include <iostream>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    // 打开默认摄像头
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "无法打开摄像头。" << endl;
        return -1;
    }

    // 创建 QRCodeDetector 对象
    QRCodeDetector detector;

    // 实时检测二维码
    while (true) {
        Mat frame;
        cap >> frame;

        vector<Point> points;
        String data = detector.detectAndDecode(frame, points);

        // 绘制边框
        if (points.size() == 4) {
            rectangle(frame, points[0], points[2], Scalar(255, 0, 0), 2);
        }

        // 输出二维码信息
        if (data.length() > 0) {
            cout << "二维码信息：" << endl
                 << data << endl;
        } else {
            cout << "未能识别二维码。" << endl;
        }

        imshow("Frame", frame);

        // 等待按键事件
        int key = waitKey(1);
        if (key == 27) {
            break;
        }
    }

    // 释放摄像头
    cap.release();

    return 0;
}
