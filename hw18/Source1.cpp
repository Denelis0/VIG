#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загрузка изображения
    cv::Mat image = cv::imread("D:/CV/Tue1/test.jpg");
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    int width = image.cols;
    int height = image.rows;
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    // Создаем маски 
    cv::Mat topLeft = image(cv::Rect(0, 0, halfWidth, halfHeight)).clone();
    cv::Mat topRight = image(cv::Rect(halfWidth, 0, halfWidth, halfHeight)).clone();
    cv::Mat bottomLeft = image(cv::Rect(0, halfHeight, halfWidth, halfHeight)).clone();
    cv::Mat bottomRight = image(cv::Rect(halfWidth, halfHeight, halfWidth, halfHeight)).clone();


    //Верхняя левая часть - инвертирование цветов
    cv::Mat invertedTopLeft;
    cv::bitwise_not(topLeft, invertedTopLeft);

    //Верхняя правая часть - оттенки серого
    cv::Mat grayTopRight;
    cv::cvtColor(topRight, grayTopRight, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayTopRight, grayTopRight, cv::COLOR_GRAY2BGR);  // Для объединения

    //Нижняя левая часть - оттенки серого
    cv::Mat grayBottomLeft;
    cv::cvtColor(bottomLeft, grayBottomLeft, cv::COLOR_BGR2GRAY);
    cv::cvtColor(grayBottomLeft, grayBottomLeft, cv::COLOR_GRAY2BGR);

    //Нижняя правая часть - инвертирование цветов
    cv::Mat invertedBottomRight;
    cv::bitwise_not(bottomRight, invertedBottomRight);

    // Объединение частей
    cv::Mat topRow, bottomRow, combinedImage;
    cv::hconcat(invertedTopLeft, grayTopRight, topRow);
    cv::hconcat(grayBottomLeft, invertedBottomRight, bottomRow);
    cv::vconcat(topRow, bottomRow, combinedImage);

    // Отображение изображений
    cv::imshow("Оригинал", image);
    cv::imshow("Обработанное изображение", combinedImage);

    cv::waitKey(0);
    return 0;
}
