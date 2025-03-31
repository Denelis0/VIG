#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// Функция для распознавания формы контура
std::string identifyShape(const std::vector<cv::Point>& contour) {
    double approximationAccuracy = 0.01 * cv::arcLength(contour, true);
    std::vector<cv::Point> approximatedContour;
    cv::approxPolyDP(contour, approximatedContour, approximationAccuracy, true);

    if (approximatedContour.size() == 3) {
        return "Triangle";
    }
    else if (approximatedContour.size() == 4) {
        cv::Rect boundingRectangle = cv::boundingRect(approximatedContour);
        double aspectRatio = static_cast<double>(boundingRectangle.width) / boundingRectangle.height;
        if (aspectRatio >= 0.9 && aspectRatio <= 1.1) {
            return "Square";
        }
        else {
            return "Rectangle";
        }
    }
    else if (approximatedContour.size() == 5) {
        return "Pentagon";
    }
    else if (approximatedContour.size() == 6) {
        return "Hexagon";
    }
    else {
        return "Circle";
    }
}

int main() {
  
    std::string imagePath = "D:/CV/Tue1/Form.jpg";
    Mat originalImage = imread(imagePath, IMREAD_COLOR);
    if (originalImage.empty()) {
        std::cout << "Error: Failed to load image!" << std::endl;
        return -1;
    }

    cv::Mat grayscaleImage;
    cv::cvtColor(originalImage, grayscaleImage, cv::COLOR_BGR2GRAY);

    cv::Mat blurredImage;
    cv::GaussianBlur(grayscaleImage, blurredImage, cv::Size(9, 9), 1.5);

    //Поиск границ с помощью алгоритма Кэнни
    cv::Mat edgeImage;
    cv::Canny(blurredImage, edgeImage, 10, 50);

    // Поиск контуров
    std::vector<std::vector<cv::Point>> shapeContours;
    cv::findContours(edgeImage, shapeContours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // Создание выходного изображения с контурами
    cv::Mat contourImage = cv::Mat::zeros(originalImage.size(), originalImage.type());
    for (size_t i = 0; i < shapeContours.size(); i++) {

        // Определение формы контура
        std::string detectedShape = identifyShape(shapeContours[i]);

        // Отрисовка контура
        cv::drawContours(contourImage, shapeContours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);

        // Вычисление центра массы контура
        cv::Moments contourMoments = cv::moments(shapeContours[i]);
        if (contourMoments.m00 != 0) {
            int centerX = static_cast<int>(contourMoments.m10 / contourMoments.m00);
            int centerY = static_cast<int>(contourMoments.m01 / contourMoments.m00);

            // Вывод текста с названием формы
            cv::putText(contourImage, detectedShape, cv::Point(centerX - 25, centerY), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 100, 0), 2);
        }
    }

    cv::imshow("Detected Shapes", contourImage);
    cv::waitKey(0);
    return 0;
}

