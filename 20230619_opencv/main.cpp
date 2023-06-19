#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // 打开默认的摄像头
    VideoCapture cap(0);

    // 检查摄像头是否成功打开
    if (!cap.isOpened()) {
        cerr << "Error: Unable to open camera." << endl;
        return -1;
    }

    // 创建窗口以显示捕获的图像
    namedWindow("Camera", WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;

        // 从摄像头中捕获一帧图像
        cap >> frame;

        // 检查图像是否成功捕获
        if (frame.empty()) {
            cerr << "Error: Unable to capture frame." << endl;
            break;
        }

        // 在窗口中显示图像
        imshow("Camera", frame);

        // 等待按下ESC键退出程序
        if (waitKey(10) == 27) {
            break;
        }

        // 按下 Space 键保存图像
        if (waitKey(10) == 32) {
            string filename = "./capture.jpg";
            imwrite(filename, frame);
            cout << "Capture saved to " << filename << endl;

            Mat savedImg = imread(filename);           // 读取保存的图像
            namedWindow("Saved Image", WINDOW_NORMAL); // 创建一个新的窗口
            imshow("Saved Image", savedImg);           // 显示保存的图像
            waitKey(0);                                // 等待按下任意键，然后关闭窗口
            destroyWindow("Saved Image");              // 关闭窗口
        }
    }

    // 释放摄像头并关闭窗口
    cap.release();
    destroyAllWindows();

    return 0;
}
