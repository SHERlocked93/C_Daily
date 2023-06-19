#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture camera0(0);
    camera0.set(CAP_PROP_FRAME_WIDTH, 1920);
    camera0.set(CAP_PROP_FRAME_HEIGHT, 1080);

    while (true) {
        Mat frame;
        camera0 >> frame;
        namedWindow("读取视频", 0);
        imshow("读取视频", frame);
        waitKey(1); // 延时1
    }
    return 0;
}
