#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

class Face;
class Person;

class Face {
public:
    Mat img;
    Person* person;
    explicit Face(Person* p):person(p){}
};

class Person {
public:
    int id;
    vector<Face*> faces;
    explicit Person(int id):id(id){}
};

vector<Person*> persons;
vector<Face*> allFaces;
vector<Mat> allFaceMats;

Size standardSize = Size(25,25);
Mat covarMat = Mat();
Mat meanMat = Mat();
Mat eigenVectors = Mat();
Mat eigenValues = Mat();
vector<Mat> eigenFaces;

void readFaces();
void calcEigens();
void recognize();

int main() {
    readFaces();
    calcEigens();
//    std::cout << "Hello, World!" << std::endl;
    waitKey(0);
    return 0;
}

void readFaces(){
    for(int i=1; i<=15; i++){
        Person person = Person(i);
        persons.push_back(&person);
        for(int j=1; j<=10; j++){
            Face face = Face(&person);
            Mat img = imread("face-library/"+to_string(i)+"/s"+to_string(j)+".bmp", CV_LOAD_IMAGE_GRAYSCALE);
            resize(img, face.img, standardSize);
            normalize(face.img, face.img, 255, 0, NORM_MINMAX);
            allFaceMats.push_back(face.img);
            person.faces.push_back(&face);
            allFaces.push_back(&face);
        }
    }
}

void calcEigens(){
    calcCovarMatrix(allFaceMats,covarMat,meanMat,COVAR_NORMAL);
//    imshow("covariance",covarMat);
    Mat meanImg;
    meanMat.convertTo(meanImg, CV_8UC1);
    imshow("mean",meanImg);
    eigen(covarMat,eigenValues,eigenVectors);
    cout<<eigenVectors.rows<<" "<<eigenVectors.cols<<endl;
//    eigenValues.row(4).reshape(0,25).convertTo(t, CV_8UC1);
//    cout<<eigenVectors.type()<<endl;
    for(int i=0; i<10; i++){
        Mat t = Mat(standardSize.height, standardSize.width, CV_64F);
        Mat tt = Mat(standardSize.height, standardSize.width, CV_8UC1);
        for(int j=0; j<standardSize.width*standardSize.height; j++){
            t.at<double>(j/standardSize.width,j%standardSize.width)=eigenVectors.at<double>(i,j);
//        cout<<j<<" "<<eigenVectors.at<double>(1,j)<<endl;
        }
        normalize(t,t,255,0,NORM_MINMAX);
        t.convertTo(tt,CV_8UC1);
        eigenFaces.push_back(tt);
    }
    for(int i=0; i<10; i++){
        imshow("eigen face "+to_string(i), eigenFaces.at(i));
    }
}