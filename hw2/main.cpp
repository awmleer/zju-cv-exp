#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

char* imagePath;
int apertureSize;
double k;

Mat imgGray;
Mat ix,iy;
Mat eigenMaxImg, eigenMinImg;
Mat eigenMaxInt, eigenMinInt;
Mat rImg;
Mat r;
Mat rFiltered;
double maxR=0;

void calculateIxAndIy();
void calculateR();

int main(int argc, char *argv[]) {

    if (argc != 4) {
        return 0;
    }

    imagePath = argv[1];
    k = atof(argv[2]);
    apertureSize = atoi(argv[3]);

    imgGray = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);

    namedWindow("grayscale", CV_WINDOW_AUTOSIZE);
    imshow("grayscale", imgGray);
    imwrite("grayscale.jpg",imgGray);

    ix=imgGray.clone();
    iy=imgGray.clone();
    calculateIxAndIy();
    namedWindow("ix", CV_WINDOW_AUTOSIZE);
    namedWindow("iy", CV_WINDOW_AUTOSIZE);
    imshow("ix", ix);
    imshow("iy", iy);
    imwrite("ix.jpg", ix);
    imwrite("iy.jpg", iy);

    eigenMaxImg=imgGray.clone();
    eigenMinImg=imgGray.clone();
    eigenMaxInt = Mat(imgGray.rows, imgGray.cols, CV_64FC1);
    eigenMinInt = Mat(imgGray.rows, imgGray.cols, CV_64FC1);
    r = Mat(imgGray.rows, imgGray.cols, CV_64FC1);
    rImg=imgGray.clone();
    calculateR();
    namedWindow("eigenMax", CV_WINDOW_AUTOSIZE);
    namedWindow("eigenMin", CV_WINDOW_AUTOSIZE);
    imshow("eigenMax", eigenMaxImg);
    imshow("eigenMin", eigenMinImg);
    imwrite("eigenMax.jpg", eigenMaxImg);
    imwrite("eigenMin.jpg", eigenMinImg);
    imshow("r", rImg);
    imwrite("r.jpg", rImg);

    waitKey(0);

    return 0;
}


void calculateIxAndIy(){
    for(int x=0; x<imgGray.rows; x++){
        for(int y=0; y<imgGray.cols; y++){
            if(x>=apertureSize&&x<imgGray.rows-apertureSize&&y>apertureSize&&y<imgGray.cols-apertureSize){
                int tx=0;
                int ty=0;
                for(int i=-apertureSize; i<=apertureSize; i++){
                    tx+=(int)imgGray.at<uchar>(x+apertureSize, y+i);
                    tx-=(int)imgGray.at<uchar>(x-apertureSize, y+i);
                    ty+=(int)imgGray.at<uchar>(x+i, y+apertureSize);
                    ty-=(int)imgGray.at<uchar>(x+i, y-apertureSize);
                }
                ix.at<uchar>(x,y)=(uchar)abs(tx/(apertureSize*2+1));
                iy.at<uchar>(x,y)=(uchar)abs(ty/(apertureSize*2+1));
            }else{
                ix.at<uchar>(x,y)=0;
                iy.at<uchar>(x,y)=0;
            }
        }
    }
}


void calculateR(){
    for(int x=0; x<imgGray.rows; x++){
        for(int y=0; y<imgGray.cols; y++){
            if(x>=apertureSize&&x<imgGray.rows-apertureSize&&y>apertureSize&&y<imgGray.cols-apertureSize){
                Mat m = Mat(2, 2, CV_64FC1);
                int ixVal=ix.at<uchar>(x,y)*ix.at<uchar>(x,y);
                int iyVal=iy.at<uchar>(x,y)*iy.at<uchar>(x,y);
                m.at<int>(0,0)=ixVal*ixVal;
                m.at<int>(1,0)=ixVal*iyVal;
                m.at<int>(0,1)=ixVal*iyVal;
                m.at<int>(1,1)=iyVal*iyVal;
                Mat evects = Mat(2, 2, CV_64FC1);
                Mat evalues = Mat(1, 2, CV_64FC1);
                eigen(m,evalues,evects);
                int lambda1=evalues.at<int>(0,0);
                int lambda2=evalues.at<int>(0,1);
                eigenMaxImg.at<uchar>(x,y)=(uchar)(lambda1/65536);
                eigenMaxInt.at<int>(x,y)=lambda1;
                eigenMinImg.at<uchar>(x,y)=(uchar)(lambda2/65536);
                eigenMinInt.at<int>(x,y)=lambda2;
                double rVal=lambda1*lambda2 - k*(lambda1+lambda2)*(lambda1+lambda2);
                if (rVal>10000000){
                    r.at<double>(x,y)=rVal;
                    rImg.at<uchar>(x,y)=255;
                    if(rVal>maxR){
                        maxR=rVal;
                    }
                }else{
                    r.at<double>(x,y)=0;
                    rImg.at<uchar>(x,y)=0;
                }
//                printf("%d %d %f\n",lambda1, lambda2, rVal);
            }else{
                r.at<double>(x,y)=0;
                rImg.at<uchar>(x,y)=0;
                eigenMaxImg.at<uchar>(x,y)=0;
                eigenMinImg.at<uchar>(x,y)=0;
            }
        }
    }
}


