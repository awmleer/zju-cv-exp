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
Mat AT = Mat(10,standardSize.height*standardSize.width,CV_64F);
float energyPercent;
String dataDirName;
String testImgName;
int eigenCount;

void readFaces();
void readData();


int main(int argc, char* argv[]) {
    testImgName = String(argv[1]);
    dataDirName = String(argv[2]);
    readFaces();
    readData();
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

void readData(){
    AT = imread(dataDirName+"/AT", CV_LOAD_IMAGE_GRAYSCALE);
    meanMat = imread(dataDirName+"/mean", CV_LOAD_IMAGE_GRAYSCALE);
}