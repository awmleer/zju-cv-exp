#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
    char image_name[20];
    string s_image_name;
    cv::VideoWriter writer;
    int isColor = 1;
    int frame_fps = 30;
    int frame_width = 640;
    int frame_height = 480;
    Size size = Size(frame_width,frame_height);
    string video_name = "out.avi";
    writer = VideoWriter(video_name, CV_FOURCC('M', 'J', 'P', 'G'), frame_fps, size,isColor);
    cout << "frame_width is " << frame_width << endl;
    cout << "frame_height is " << frame_height << endl;
    cout << "frame_fps is " << frame_fps << endl;
    int num = 2;//输入的图片总张数
    int i = 0;
    Mat img;
    while (i<num) {
        sprintf(image_name, "%s%d%s", "image", ++i, ".png");
        cout << image_name <<endl;
        s_image_name = image_name;
        img = imread(s_image_name);//读入图片

        if (!img.data) {
            cout << "Could not load image file...\n" << endl;
        }
        //写入
        Mat imgResized;
        resize(img, imgResized, size);
        for(int j=0; j<30; j++){
            writer.write(imgResized);
        }
//        writer.write(img);
    }
    writer.release();

    return 0;
}
