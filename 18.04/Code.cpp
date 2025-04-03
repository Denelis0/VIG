#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Загрузка изображений
    Mat img1 = imread("D:/CV/Tue1/image0.png");
    Mat img2 = imread("D:/CV/Tue1/image1.png");

    if (img1.empty() || img2.empty()) {
        cerr << "Ошибка: не удалось загрузить изображения!" << endl;
        return -1;
    }

    // Преобразование в градации серого
    Mat gray1, gray2;
    cvtColor(img1, gray1, COLOR_BGR2GRAY);
    cvtColor(img2, gray2, COLOR_BGR2GRAY);

    // Детектирование ключевых точек и вычисление дескрипторов с помощью ORB
    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;

    orb->detectAndCompute(gray1, noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(gray2, noArray(), keypoints2, descriptors2);

    // Сопоставление дескрипторов методом Brute-Force с кросс-проверкой
    BFMatcher matcher(NORM_HAMMING, true);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Сортировка совпадений по расстоянию
    sort(matches.begin(), matches.end(), [](const DMatch& a, const DMatch& b) {
        return a.distance < b.distance;
        });

    // Отображение лучших совпадений (например, 50)
    const int numGoodMatches = 50;
    matches.resize(min(numGoodMatches, (int)matches.size()));

    // Визуализация совпадений
    Mat imgMatches;
    drawMatches(img1, keypoints1, img2, keypoints2, matches, imgMatches);
    imshow("Совпадения ключевых точек", imgMatches);

    // Преобразование совпадений в точки
    vector<Point2f> points1, points2;
    for (const auto& match : matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }

    // Вычисление гомографии с использованием RANSAC
    Mat H = findHomography(points2, points1, RANSAC);

    // Создание панорамы с помощью перспективного преобразования
    Mat panorama;
    warpPerspective(img2, panorama, H, Size(img1.cols + img2.cols + img1.cols + img2.cols, img1.rows));

    // Копирование первого изображения на панораму
    Mat roi(panorama, Rect(0, 0, img1.cols, img1.rows));
    img1.copyTo(roi);

    // Отображение результата без линий
    imshow("Панорама", panorama);

    waitKey(0);
    return 0;
}
