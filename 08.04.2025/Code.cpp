#include <filesystem>
#include <opencv2/opencv.hpp>
#include <iostream>

namespace fsys = std::filesystem;

struct CardTemplate {
	std::string name;
	cv::Mat image;
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;
};

std::vector<cv::DMatch> getBestMatches(const std::vector<std::vector<cv::DMatch>>& matches, double threshold);
bool loadCardSet(const std::string& folderPath, std::vector<CardTemplate>& cardList, cv::Ptr<cv::SIFT> detector);
void annotateMatch(cv::Mat& canvas, const CardTemplate& card, const std::vector<cv::Point2f>& region);

int main() {
	const double AREA_LIMIT = 1000.0;
	const int MATCH_THRESHOLD = 4;

	cv::Ptr<cv::SIFT> extractor = cv::SIFT::create();
	cv::BFMatcher matcher(cv::NORM_L2);
	std::vector<CardTemplate> templateCards;

	std::string datasetPath = "D:\\CV\\cards"; 
	if (!loadCardSet(datasetPath, templateCards, extractor)) {
		std::cerr << "Ошибка: Ни одно эталонное изображение не загружено." << std::endl;
		return -1;
	}

	cv::Mat input = cv::imread("D:\\CV\Tue1\\foto.png");
	if (input.empty()) {
		std::cerr << "Ошибка: Не удалось загрузить целевое изображение." << std::endl;
		return -1;
	}

	std::vector<cv::KeyPoint> inputKeypoints;
	cv::Mat inputDescriptors;
	extractor->detectAndCompute(input, cv::noArray(), inputKeypoints, inputDescriptors);

	std::cout << "Обнаружено ключевых точек на целевом изображении: " << inputKeypoints.size() << std::endl;

	cv::Mat visualOutput = input.clone();
	bool cardFound = false;

	for (const auto& card : templateCards) {
		if (card.descriptors.empty() || inputDescriptors.empty()) {
			std::cout << "Пропуск " << card.name << ": пустые дескрипторы" << std::endl;
			continue;
		}

		std::vector<std::vector<cv::DMatch>> rawMatches;
		matcher.knnMatch(card.descriptors, inputDescriptors, rawMatches, 2);

		auto matchesFiltered = getBestMatches(rawMatches);
		std::cout << card.name << " совпадений: " << matchesFiltered.size() << std::endl;

		if (matchesFiltered.size() < MATCH_THRESHOLD) {
			std::cout << card.name << ": недостаточно совпадений" << std::endl;
			continue;
		}

		std::vector<cv::Point2f> pts_card, pts_input;
		for (const auto& match : matchesFiltered) {
			pts_card.push_back(card.keypoints[match.queryIdx].pt);
			pts_input.push_back(inputKeypoints[match.trainIdx].pt);
		}

		cv::Mat H = cv::findHomography(pts_card, pts_input, cv::RANSAC);
		if (H.empty()) {
			std::cout << card.name << ": не удалось найти гомографию" << std::endl;
			continue;
		}

		std::vector<cv::Point2f> cardBox = {
			{0, 0},
			{(float)card.image.cols, 0},
			{(float)card.image.cols, (float)card.image.rows},
			{0, (float)card.image.rows}
		};
		std::vector<cv::Point2f> projectedBox;
		cv::perspectiveTransform(cardBox, projectedBox, H);

		double area = cv::contourArea(projectedBox);
		std::cout << card.name << " площадь контура: " << area << std::endl;

		if (area < AREA_LIMIT) {
			std::cout << card.name << ": слишком маленькая площадь" << std::endl;
			continue;
		}

		annotateMatch(visualOutput, card, projectedBox);
		cardFound = true;
	}

	if (!cardFound) {
		std::cout << "Совпадения не найдены." << std::endl;
	}

	cv::imshow("Результат распознавания", visualOutput);
	cv::waitKey(0);
	return 0;
}

std::vector<cv::DMatch> getBestMatches(const std::vector<std::vector<cv::DMatch>>& matches, double threshold) {
	std::vector<cv::DMatch> best;
	for (const auto& m : matches) {
		if (m.size() >= 2 && m[0].distance < threshold * m[1].distance)
			best.push_back(m[0]);
	}
	return best;
}

bool loadCardSet(const std::string& folderPath, std::vector<CardTemplate>& cardList, cv::Ptr<cv::SIFT> detector) {
	if (!fsys::exists(folderPath)) {
		std::cerr << "Ошибка: Папка '" << folderPath << "' не найдена." << std::endl;
		return false;
	}

	for (const auto& file : fsys::directory_iterator(folderPath)) {
		if (file.path().extension() == ".png") {
			CardTemplate card;
			card.name = file.path().stem().string();
			card.image = cv::imread(file.path().string());

			if (card.image.empty()) {
				std::cerr << "Не удалось загрузить изображение: " << file.path() << std::endl;
				continue;
			}

			detector->detectAndCompute(card.image, cv::noArray(), card.keypoints, card.descriptors);
			cardList.push_back(card);
		}
	}

	return !cardList.empty();
}

void annotateMatch(cv::Mat& canvas, const CardTemplate& card, const std::vector<cv::Point2f>& region) {
	for (size_t i = 0; i < 4; ++i)
		cv::line(canvas, region[i], region[(i + 1) % 4], cv::Scalar(0, 255, 0), 2);

	cv::Point2f center(0, 0);
	for (const auto& pt : region) center += pt;
	center *= 1.0f / static_cast<float>(region.size());

	cv::putText(canvas, card.name, center - cv::Point2f(50, 0), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
}
