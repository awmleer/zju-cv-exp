#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

class Face;
class Person;

class Face {
public:
    int id;
    Mat img;
    Person* person;
    Mat coordinates;
    explicit Face(Person* p, int id):person(p),id(id){}
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
Mat AT;
Mat A;
float energyPercent;
int eigenCount;

String testImgPath;
Mat testImg;

void readFaces();
void calcEigens();
void recognize();

int main(int argc, char* argv[]) {
    energyPercent = atof(argv[1]);
    testImgPath = String(argv[2]);
    readFaces();
    calcEigens();
    recognize();
    waitKey(0);
    return 0;
}

void readFaces(){
    for(int i=1; i<=16; i++){
        Person* person = new Person(i);
        persons.push_back(person);
        for(int j=1; j<=10; j++){
            Face* face = new Face(person, j);
            Mat img = imread("face-library/"+to_string(i)+"/s"+to_string(j)+".bmp", CV_LOAD_IMAGE_GRAYSCALE);
            resize(img, img, standardSize);
            face->img = Mat(standardSize.height, standardSize.width, CV_8UC1);
            img.copyTo(face->img);
            normalize(face->img, face->img, 255, 0, NORM_MINMAX);
            allFaceMats.push_back(face->img);
            person->faces.push_back(face);
            allFaces.push_back(face);
        }
    }
    testImg = imread(testImgPath,CV_LOAD_IMAGE_GRAYSCALE);
    resize(testImg, testImg, standardSize);
    normalize(testImg, testImg, 255, 0, NORM_MINMAX);
}

void calcEigens(){
    calcCovarMatrix(allFaceMats,covarMat,meanMat,COVAR_NORMAL);
    Mat meanImg;
    meanMat.convertTo(meanImg, CV_8UC1);
    imshow("mean",meanImg);
    eigen(covarMat,eigenValues,eigenVectors);
    eigenCount = eigenVectors.rows*energyPercent;
    cout<<"eigen count: "<<eigenCount<<endl;
    AT = Mat(eigenCount, standardSize.height*standardSize.width, CV_64F);
    A = Mat(standardSize.height*standardSize.width,eigenCount,CV_64F);
    for(int i=0; i<eigenCount; i++){
        Mat t = Mat(standardSize.height, standardSize.width, CV_64F);
        Mat tt = Mat(standardSize.height, standardSize.width, CV_8UC1);
        for(int j=0; j<standardSize.width*standardSize.height; j++){
            t.at<double>(j/standardSize.width,j%standardSize.width)=eigenVectors.at<double>(i,j);
            AT.at<double>(i,j)=eigenVectors.at<double>(i,j);
            A.at<double>(j,i)=eigenVectors.at<double>(i,j);
        }
        normalize(t,t,255,0,NORM_MINMAX);
        t.convertTo(tt,CV_8UC1);
        eigenFaces.push_back(tt);
    }
    for(int i=0; i<10&&i<eigenCount; i++){
        imshow("eigen face "+to_string(i), eigenFaces.at(i));
    }
}

void recognize(){
    cout<<testImg.reshape(0,1).rows<<"*"<<testImg.reshape(0,1).cols<<endl;
    cout<<A.rows<<"*"<<A.cols<<endl;
    Mat testDoubleMat;
    testImg.reshape(0,1).convertTo(testDoubleMat, CV_64F);
    Mat testCoordinates = testDoubleMat * A;
    double minDistance = -1;
    Face* resultFace;
    cout<<testCoordinates.rows<<"*"<<testCoordinates.cols<<endl;
    for(vector<Face*>::iterator iter=allFaces.begin(); iter!=allFaces.end(); iter++){
        double distance;
        Face* face = *iter;
        Mat doubleMat;
        face->img.reshape(0,1).convertTo(doubleMat, CV_64F);
        face->coordinates = doubleMat * A;
        distance = 0;
        for(int i=0; i<eigenCount; i++){
            distance += pow(face->coordinates.at<double>(0,i)-testCoordinates.at<double>(0,i), 2);
        }
        cout<<face->person->id<<": "<<distance<<endl;
        if(distance<minDistance || minDistance==-1){
            minDistance = distance;
            resultFace = face;
        }
    }
    cout<<"result:\n"<<"person"<<resultFace->person->id<<" img"<<resultFace->id<<endl;
    imshow("most similar face", resultFace->img);
    Mat colorfulTestImg;
    cvtColor(testImg, colorfulTestImg, COLOR_GRAY2BGR);
    putText(colorfulTestImg, to_string(resultFace->person->id), cvPoint(2,20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,0,255), 1, CV_AA);
    imshow("test image", colorfulTestImg);
}
