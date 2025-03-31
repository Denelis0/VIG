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

    Mat image_gray;
    cvtColor(image, image_gray, COLOR_BGR2GRAY);

    //Фильтр Гаусса 
    Mat image_blurred;
    GaussianBlur(image_gray, image_blurred, Size(5, 5), 0);

    
    imshow("Оригинал", image);               
    imshow("Оттенки серого", image_gray);     
    imshow("Сглаживание", image_blurred); 
    waitKey(0);

    return 0;
}
