#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;
int main(int argc, char *argv[]) {
    if(argc==1) return 0;
    char* imgPath = argv[1];
    Mat img = imread(imgPath, 0);
    imshow("origin", img);
    vector<vector<Point>> contours;
    findContours(img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    cout<<contours.size()<<endl;
    for(int i=0; i<contours.size(); i++){
        if(contours[i].size()<6)continue;
        Mat points;
        Mat(contours[i]).convertTo(points, CV_32FC3);
        RotatedRect box = fitEllipse(points);
        ellipse(img,box,Scalar(94, 206, 165), 2, LINE_AA);
        cout<<box.center.x<<endl;
        cout<<box.center.y<<endl;
        cout<<"----"<<endl;
    }
    imshow("result",img);
    waitKey(0);
    return 0;
}