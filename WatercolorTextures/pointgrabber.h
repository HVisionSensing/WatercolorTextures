#ifndef POINT_GRABBER_H
#define POINT_GRABBER_H

#include <vector>
#include <opencv2/core/core.hpp>

class PointGrabber
{
public:
	PointGrabber() : activePoint(0) {}

	void setActive(cv::Point *activePoint) { this->activePoint = activePoint; }

	void setInactive() { activePoint = 0; }

	bool isActive() const { return activePoint != 0; }

	cv::Point *getActivePoint() { return activePoint; }

	cv::Point *findPointNear(cv::Point pivot, std::vector<cv::Point> &points, int maxAllowedDistance = 4);

	cv::Point *findPointNear(cv::Point pivot, std::vector<cv::Point>::iterator begin, std::vector<cv::Point>::iterator end, int maxAllowedDistance = 4);

protected:
	cv::Point *activePoint;
};

#endif
