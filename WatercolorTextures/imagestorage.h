#ifndef IMAGE_STORAGE_H
#define IMAGE_STORAGE_H

#include <opencv2/core/core.hpp>

class ImageStorage
{
public:
	static cv::Mat *add(std::string name, cv::Mat &image);

	static cv::Mat *add(std::string name, cv::Mat *image, bool deleteOnCleanup = false);

	static cv::Mat *get(std::string name);

	static void createEmptyImage(cv::Size size, int type, std::string name);

	static void createEmptyImages(cv::Size size, int type, std::string name, std::string name2);

	static void createEmptyImages(cv::Size size, int type, std::string name, std::string name2, std::string name3);

	static void cleanup();

protected:
	ImageStorage() {}

protected:
	static std::map<std::string, cv::Mat*> storage;

	static std::vector<std::string> created;
};

cv::Mat *img(std::string name);

cv::Mat *img(std::string name, cv::Mat &image);

cv::Mat *img(std::string name, cv::Mat *image, bool deleteOnCleanup = false);

cv::Mat *img(std::string name, cv::Size size, int type);

#endif
