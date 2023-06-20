// 对摄像头识别人脸
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // 加载人脸识别模型
    CascadeClassifier face_cascade;
    face_cascade.load("haarcascade_frontalface_default.xml");

    // 打开摄像头
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Failed to open camera!" << endl;
        return -1;
    }

    while (true) {
        // 读取摄像头图像帧
        Mat frame;
        cap >> frame;

        // 图像预处理
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        // 人脸检测
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(40, 40));

        // 绘制检测框
        for (size_t i = 0; i < faces.size(); i++) {
            rectangle(frame, faces[i], Scalar(255, 0, 0), 2);
        }

        // 显示结果
        imshow("Face Detection", frame);

        // 按下 Space 键保存面积最大头像
        if (waitKey(10) == 32) {
            int max_area = 0;
            Rect max_rect;
            for (size_t i = 0; i < faces.size(); i++) {
                int area = faces[i].width * faces[i].height;
                if (area > max_area) {
                    max_area = area;
                    max_rect = faces[i];
                }
            }

            // 截取人脸图像并保存到文件中
            string whole_img = "./whole_img.jpg";
            string face_img = "./face_img.jpg";

            imwrite(whole_img, frame);
            Mat image = imread(whole_img);

            Mat face = image(max_rect);
            cout << "face.empty:" << face.empty() << endl;
            imwrite(face_img, face);
            cout << "Capture saved to " << face_img << endl;

            Mat savedImg = imread(face_img);
            namedWindow("Saved Image", WINDOW_NORMAL); // 创建一个新的窗口
            imshow("Saved Image", savedImg);           // 显示保存的图像
            waitKey(0);                                // 等待按下任意键，然后关闭窗口
            destroyWindow("Saved Image");              // 关闭窗口
        }

        // 等待按下ESC键退出程序
        if (waitKey(1) == 27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}
