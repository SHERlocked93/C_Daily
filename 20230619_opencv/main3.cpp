// 对单个图片识别人脸
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // 加载人脸识别模型
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_default.xml");

    // 加载待识别图像
    Mat img = imread("img.jpeg");

    // 图像预处理
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    // 人脸检测
    vector<Rect> faces;
    face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    // 人脸识别
    for (size_t i = 0; i < faces.size(); i++) {
        Mat face = gray(faces[i]);
        // 在这里进行人脸识别
    }

    // 显示结果
    for (size_t i = 0; i < faces.size(); i++) {
        rectangle(img, faces[i], Scalar(255, 0, 0), 2);
    }
    imshow("result", img);
    waitKey(0);

    return 0;
}
