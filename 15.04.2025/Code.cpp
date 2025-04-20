#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::CascadeClassifier detector_face, detector_eyes, detector_smile;

    if (
        !detector_face.load("./haarcascade_frontalface_default.xml") ||
        !detector_eyes.load("./haarcascade_eye.xml") ||
        !detector_smile.load("./haarcascade_smile.xml")
    ) {
        std::cerr << "Не удалось загрузить каскады" << std::endl;
        return -1;
    }

    cv::VideoCapture video_capture(""D:\\CV\\Tue1\\video.mp4"");

    if (!video_capture.isOpened()) {
        std::cerr << "Ошибка видео!" << std::endl;
        return -1;
    }

    cv::Mat video_frame;
    while (video_capture.read(video_frame)) {
        if (video_frame.empty()) break;

        cv::Mat gray_frame;
        cv::cvtColor(video_frame, gray_frame, cv::COLOR_BGR2GRAY);

        cv::equalizeHist(gray_frame, gray_frame);

        cv::GaussianBlur(gray_frame, gray_frame, cv::Size(5, 5), 0);

        std::vector<cv::Rect> detected_faces;
        detector_face.detectMultiScale(gray_frame, detected_faces, 1.1, 5, 0, cv::Size(150, 150));

        for (const auto& detected_face : detected_faces) {
            cv::rectangle(video_frame, detected_face, cv::Scalar(255, 0, 0), 2);

            cv::Mat face_region_gray = gray_frame(detected_face);
            cv::Mat face_region_color = video_frame(detected_face);

            std::vector<cv::Rect> detected_eyes;
            detector_eyes.detectMultiScale(face_region_gray, detected_eyes, 1.1, 10, 0, cv::Size(50, 50));

            for (const auto& detected_eye : detected_eyes) 
                cv::rectangle(face_region_color, detected_eye, cv::Scalar(0, 0, 255), 2);

            std::vector<cv::Rect> detected_smiles;
            detector_smile.detectMultiScale(face_region_gray, detected_smiles, 1.24, 15, 0, cv::Size(40, 40));

            for (const auto& detected_smile : detected_smiles) 
                cv::rectangle(face_region_color, detected_smile, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Face Detection", video_frame);
        if (cv::waitKey(30) >= 0) break;
    }

    video_capture.release();
    cv::destroyAllWindows();
    return 0;
}
