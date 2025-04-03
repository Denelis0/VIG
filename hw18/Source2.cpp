#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    string filePath = "D:/CV/Tue1/test.jpg";  // Путь к изображению

    Mat inputImage = imread(filePath);  // Загрузка изображения

    if (inputImage.empty()) {  // Проверка на успешную загрузку изображения
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Рисуем линию
    line(inputImage, Point(50, 50), Point(200, 50), Scalar(0, 255, 0), 2);

    // Рисуем прямоугольник
    rectangle(inputImage, Point(400, 80), Point(200, 150), Scalar(255, 0, 0), 2);

    // Рисуем круг
    circle(inputImage, Point(300, 250), 50, Scalar(0, 0, 255), 2);

    // Рисуем эллипс
    ellipse(inputImage, Point(200, 350), Size(60, 30), 30, 0, 360, Scalar(255, 255, 0), 2);

    // Рисуем многоугольник
    vector<Point> polygonPoints = { Point(50, 400), Point(100, 450), Point(150, 400), Point(100, 350) };
    polylines(inputImage, polygonPoints, true, Scalar(100, 3, 255), 2);

    // Добавляем текст
    putText(inputImage, "Arcane", Point(500, 400), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 254), 2);

    // Отображаем изображение
    imshow("Modified Image", inputImage);

    // Сохраняем изменённое изображение
    imwrite("Edited_Image.jpg", inputImage);

    // Ожидание нажатия клавиши
    waitKey(0);

    return 0;
}
