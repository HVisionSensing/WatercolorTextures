#include "exception.h"
#include "imagestorage.h"

std::map<std::string, cv::Mat*> ImageStorage::storage;

std::vector<std::string> ImageStorage::created;

void ImageStorage::cleanup()
{
	for (std::vector<std::string>::iterator it = created.begin(); it != created.end(); ++it) {
		delete storage[*it];
	}
}

void ImageStorage::createEmptyImage(cv::Size size, int type, std::string name)
{
	ASSERT(name.length() != 0 && storage.count(name) == 0)
	cv::Mat *tmp = new cv::Mat();
	tmp->create(size, type);
	created.push_back(name);
	storage[name] = tmp;
}

void ImageStorage::createEmptyImages(cv::Size size, int type, std::string name, std::string name2)
{
	createEmptyImage(size, type, name);
	createEmptyImage(size, type, name2);
}

void ImageStorage::createEmptyImages(cv::Size size, int type, std::string name, std::string name2, std::string name3)
{
	createEmptyImage(size, type, name);
	createEmptyImage(size, type, name2);
	createEmptyImage(size, type, name3);
}

cv::Mat *ImageStorage::add(std::string name, cv::Mat &image)
{
	ASSERT(name.length() != 0 && storage.count(name) == 0)
	storage[name] = &image;
	return storage[name];
}

cv::Mat *ImageStorage::add(std::string name, cv::Mat *image, bool deleteOnCleanup)
{
	ASSERT(name.length() != 0 && storage.count(name) == 0)
	storage[name] = image;
	if (deleteOnCleanup) created.push_back(name);
	return storage[name];
}

cv::Mat *ImageStorage::get(std::string name)
{
	ASSERT(storage.count(name) != 0)
	return storage[name];
}

cv::Mat *img(std::string name)
{
	return ImageStorage::get(name);
}

cv::Mat *img(std::string name, cv::Mat &image)
{
	return ImageStorage::add(name, image);
}

cv::Mat *img(std::string name, cv::Mat *image, bool deleteOnCleanup)
{
	return ImageStorage::add(name, image, deleteOnCleanup);
}

cv::Mat *img(std::string name, cv::Size size, int type)
{
	ImageStorage::createEmptyImage(size, type, name);
	return ImageStorage::get(name);
}
