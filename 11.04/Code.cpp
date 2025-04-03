#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Функция для распознавания формы контура
string identifyShape(const vector<Point>& contour) {
    double epsilon = 0.02 * arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, epsilon, true);

    if (approx.size() == 3)
        return "Triangle";
    else if (approx.size() == 4) {
        Rect boundingRectangle = cv::boundingRect(approx);
        double aspectRatio = (double)boundingRectangle.width / boundingRectangle.height;
        return (aspectRatio >= 0.9 && aspectRatio <= 1.1) ? "Square" : "Rectangle";
    }
    else if (approx.size() == 5)
        return "Pentagon";
    else if (approx.size() == 6)
        return "Hexagon";
    else
        return "Circle";
}

int main() {
    VideoCapture cap("D:/CV/Tue1/video.mp4");
    if (!cap.isOpened()) {
        cerr << "Error: Could not open video." << endl;
        return -1;
    }

    int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    VideoWriter output("output.mp4", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(frame_width, frame_height));

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;

        Mat gray, blurred, edges;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurred, Size(5, 5), 0);
        Canny(blurred, edges, 50, 150);

        vector<vector<Point>> contours;
        findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (const auto& contour : contours) {
            drawContours(frame, vector<vector<Point>>{contour}, -1, Scalar(0, 0, 255), 2);
        }

        output.write(frame);
        imshow("Shape Tracking", frame);
        if (waitKey(10) == 27)
            break;
    }

    cap.release();
    output.release();
    destroyAllWindows();
    return 0;
}
