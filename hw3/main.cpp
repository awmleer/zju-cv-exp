#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;
int main(int argc, char *argv[]) {
    if(argc==1) return 0;
    char* imgPath = argv[1];
    double thresh = 128;
    if(argc>=3){
        thresh = atof(argv[2]);
    }
    Mat originImg = imread(imgPath, CV_LOAD_IMAGE_COLOR);
    Mat grayscaleImg = imread(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
    imshow("origin", originImg);
    Mat binaryImg;
    threshold(grayscaleImg, binaryImg, thresh, 255, CV_THRESH_BINARY);
    imshow("binary", binaryImg);
    vector<vector<Point>> contours;
    findContours(binaryImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    cout<<contours.size()<<endl;
    for(int i=0; i<contours.size(); i++){
        if(contours[i].size()<6)continue;
        Mat points;
        Mat(contours[i]).convertTo(points, CV_32FC3);
        RotatedRect box = fitEllipse(points);
        ellipse(originImg,box,Scalar(94, 206, 165), 2, LINE_AA);
        cout<<box.center.x<<endl;
        cout<<box.center.y<<endl;
        cout<<"----"<<endl;
    }
    imshow("result",originImg);
    waitKey(0);
    return 0;
}