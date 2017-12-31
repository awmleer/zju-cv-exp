#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

char* imagePath;
int apertureSize;
double k;

void calculateIxAndIy(Mat &imgGray, Mat &ix, Mat &iy);
void calculateEigenMaxAndEigenMin(Mat &imgGray, Mat &eigenMax, Mat &eigenMin);

int main(int argc, char *argv[]) {

    if (argc != 4) {
        return 0;
    }

    imagePath = argv[1];
    k = atof(argv[2]);
    apertureSize = atoi(argv[3]);

    Mat imgGray;
    imgGray = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);

    namedWindow("grayscale", CV_WINDOW_AUTOSIZE);
    imshow("grayscale", imgGray);
    imwrite("grayscale.jpg",imgGray);

    Mat ix=imgGray.clone();
    Mat iy=imgGray.clone();
    calculateIxAndIy(imgGray, ix, iy);
    namedWindow("ix", CV_WINDOW_AUTOSIZE);
    namedWindow("iy", CV_WINDOW_AUTOSIZE);
    imshow("ix", ix);
    imshow("iy", iy);
    imwrite("ix.jpg", ix);
    imwrite("iy.jpg", iy);

    Mat eigenMax=imgGray.clone();
    Mat eigenMin=imgGray.clone();
    calculateEigenMaxAndEigenMin(imgGray, eigenMax, eigenMin);

    waitKey(0);

    return 0;
}


void calculateIxAndIy(const Mat &imgGray, Mat &ix, Mat &iy){
    for(int a=0; a<imgGray.rows; a++){
        for(int b=0; b<imgGray.cols; b++){
            if(a>=apertureSize&&a<imgGray.rows-apertureSize&&b>apertureSize&&b<imgGray.cols-apertureSize){
                int tx=0;
                int ty=0;
                for(int i=-apertureSize; i<=apertureSize; i++){
                    tx+=(int)imgGray.at<uchar>(a+apertureSize, b+i);
                    tx-=(int)imgGray.at<uchar>(a-apertureSize, b+i);
                    ty+=(int)imgGray.at<uchar>(a+i, b+apertureSize);
                    ty-=(int)imgGray.at<uchar>(a+i, b-apertureSize);
                }
                ix.at<uchar>(a,b)=(uchar)abs(tx/(apertureSize*2+1));
                iy.at<uchar>(a,b)=(uchar)abs(ty/(apertureSize*2+1));
            }else{
                ix.at<uchar>(a,b)=0;
                iy.at<uchar>(a,b)=0;
            }
        }
    }
}


void calculateEigenMaxAndEigenMin(const Mat &imgGray, Mat &eigenMax, Mat &eigenMin){

}



