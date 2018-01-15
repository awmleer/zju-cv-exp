#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

class Face {
public:
    Mat originImg;
};

class Person {
public:
    int id;
    vector<Face> faces;
    explicit Person(int id):id(id){}
};

vector<Person> persons;

void readFaces();
void calcEigens();
void recognize();

int main() {
    readFaces();
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}

void readFaces(){
    for(int i=1; i<=15; i++){
        Person person = Person(i);
        persons.push_back(person);
        for(int j=1; j<=10; j++){
            Face face = Face();
            face.originImg = imread("face-library/"+to_string(i)+"/s"+to_string(j)+".bmp", CV_LOAD_IMAGE_GRAYSCALE);
            person.faces.push_back(face);
        }
    }
}