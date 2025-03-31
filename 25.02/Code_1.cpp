#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {

    Mat image = imread("D:/CV/Tue1/test.jpg"); 
    if (image.empty()) {
        cout << "Ошибка загрузки изображения!" << endl;
        return -1;
    }

   
    Mat image_hsv, image_lab, image_yuv, image_xyz, image_gray;

    //HSV
    cvtColor(image, image_hsv, COLOR_BGR2HSV);
    //Lab
    cvtColor(image, image_lab, COLOR_BGR2Lab);
    //YUV
    cvtColor(image, image_yuv, COLOR_BGR2YUV);
    //XYZ
    cvtColor(image, image_xyz, COLOR_BGR2XYZ);
    //Grayscale 
    cvtColor(image, image_gray, COLOR_BGR2GRAY);

    imshow("Original Image (BGR)", image); 
    imshow("HSV", image_hsv);              // HSV
    imshow("Lab", image_lab);              // Lab
    imshow("YUV", image_yuv);              // YUV
    imshow("XYZ", image_xyz);              // XYZ
    imshow("Grayscale", image_gray);       // Gray
    waitKey(0);

    return 0;
}


