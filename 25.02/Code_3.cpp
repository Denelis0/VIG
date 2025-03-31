#include <opencv2/opencv.hpp>
#include <iostream>

int main() {

    cv::Mat inputImage = cv::imread("D:/CV/Tue1/test.jpg");
    if (inputImage.empty()) {
        std::cerr << "Ошибка" << std::endl;
        return -1;
    }

    cv::Mat lineImage = inputImage.clone();
    cv::Mat circleImage = inputImage.clone();

    cv::Mat grayscaleImage;
    cv::cvtColor(inputImage, grayscaleImage, cv::COLOR_BGR2GRAY);

    cv::Mat edgeMap;
    cv::Canny(grayscaleImage, edgeMap, 50, 150, 3);
    std::vector<cv::Vec2f> detectedLines;
    cv::HoughLines(edgeMap, detectedLines, 1, CV_PI / 180, 150);

    for (size_t index = 0; index < detectedLines.size(); index++) {
        float rho = detectedLines[index][0];
        float theta = detectedLines[index][1];
        double cosTheta = cos(theta), sinTheta = sin(theta);
        double xOrigin = cosTheta * rho, yOrigin = sinTheta * rho;
        cv::Point startPoint(cvRound(xOrigin + 1000 * (-sinTheta)), cvRound(yOrigin + 1000 * (cosTheta)));
        cv::Point endPoint(cvRound(xOrigin - 1000 * (-sinTheta)), cvRound(yOrigin - 1000 * (cosTheta)));
        cv::line(lineImage, startPoint, endPoint, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    
    cv::GaussianBlur(grayscaleImage, grayscaleImage, cv::Size(9, 9), 2, 2);
    
    std::vector<cv::Vec3f> detectedCircles;
    cv::HoughCircles(grayscaleImage, detectedCircles, cv::HOUGH_GRADIENT, 1, grayscaleImage.rows / 8, 100, 50, 0, 0);

    for (size_t index = 0; index < detectedCircles.size(); index++) {
        cv::Vec3i circleData = detectedCircles[index];
        cv::Point center(circleData[0], circleData[1]);
        int radius = circleData[2];
        cv::circle(circleImage, center, 3, cv::Scalar(0, 255, 0), -1, cv::LINE_AA); 
        cv::circle(circleImage, center, radius, cv::Scalar(255, 0, 0), 2, cv::LINE_AA); 
    }
    cv::imshow("Линии", lineImage);
    cv::imshow("Круги", circleImage);
    cv::waitKey(0);
    return 0;
}
