#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

class Face;
class Person;

class Face {
public:
    Mat originImg;
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
Mat meanMat = Mat(standardSize.height, standardSize.width, CV_64F);

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
            resize(img, face.originImg, standardSize);
            allFaceMats.push_back(face.originImg);
            person.faces.push_back(&face);
            allFaces.push_back(&face);
        }
    }
}

void calcEigens(){
    calcCovarMatrix(allFaceMats,covarMat,meanMat,COVAR_NORMAL);
    imshow("covariance",covarMat);
    Mat meanImg;
    meanMat.convertTo(meanImg, CV_8UC1);
    imshow("mean",meanImg);

}