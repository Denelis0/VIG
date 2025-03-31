#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Проверяем версию OpenCV
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // Загружаем изображение
    cv::Mat image = cv::imread("D:\\CV\\Tue1\\test.jpg");
    if (image.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    // Отображаем изображение в окне
    cv::imshow("Проверка OpenCV", image);

    // Ждем нажатия любой клавиши
    cv::waitKey(0);

    return 0;
}
