#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    if (argc != 4) {
        return 0;
    }

    char* path = argv[1];
    double k = atof(argv[2]);
    int apertureSize = atoi(argv[3]);


    return 0;
}