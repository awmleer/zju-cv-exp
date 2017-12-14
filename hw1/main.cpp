#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv ) {
    char imagePath[200];

    //initialize video writer
    VideoWriter writer;
    bool isColor = true;
    int frameFps = 30;
    int frameWidth = 640;
    int frameHeight = 360;
    Size finalSize = Size(frameWidth,frameHeight);
    string videoName = "out.avi";
    writer = VideoWriter(videoName, CV_FOURCC('M', 'J', 'P', 'G'), frameFps, finalSize,isColor);

    //join images
    int i = 0;
    Mat img;
    while (true) {
        sprintf(imagePath, "%s/%d.jpg", argv[1], ++i);
        img = imread(imagePath);//read images
        if (!img.data) {
            break;
        }
        cout << imagePath <<endl;
        //写入
        Mat imgResized;
        resize(img, imgResized, finalSize);
        putText(imgResized, "3150104785 HaoGuangBo", cvPoint(30,320), FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(200,200,250), 1, CV_AA);
        for(int j=0; j<60; j++){// 60 frames = 2 seconds
            writer.write(imgResized);
        }
    }

    //handle video
    char videoPath[200];
    sprintf(videoPath, "%s/1.avi", argv[1]);
    VideoCapture capture = VideoCapture(videoPath);
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
