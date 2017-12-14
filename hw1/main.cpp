#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
    char image_name[20];

    //initialize video writer
    cv::VideoWriter writer;
    bool isColor = true;
    int frame_fps = 30;
    int frame_width = 640;
    int frame_height = 360;
    Size finalSize = Size(frame_width,frame_height);
    string video_name = "out.avi";
    writer = VideoWriter(video_name, CV_FOURCC('M', 'J', 'P', 'G'), frame_fps, finalSize,isColor);

    //join images
    int imageAmount = 2;//amount of images
    int i = 0;
    Mat img;
    while (i<imageAmount) {
        sprintf(image_name, "%s%d%s", "image", ++i, ".png");
        cout << image_name <<endl;
        img = imread(image_name);//read images

        if (!img.data) {
            throw "Error when reading image file";
        }
        //写入
        Mat imgResized;
        resize(img, imgResized, finalSize);
        for(int j=0; j<60; j++){// 60 frames = 2 seconds
            writer.write(imgResized);
        }
    }

    //handle video
    VideoCapture capture = VideoCapture("1.avi");
    Mat frame,frameResized;
    cout<<"1.avi"<<endl;
    if( !capture.isOpened() ){
        throw "Error when reading avi";
    }
    namedWindow( "w", 1);
    while(true) {
        capture >> frame;
        if(frame.empty())
            break;
        resize(frame, frameResized, finalSize);
        putText(frameResized, "3150104785 HaoGuangBo", cvPoint(30,320), FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(200,200,250), 1, CV_AA);
        writer.write(frameResized);
    }

    writer.release();

    return 0;
}
