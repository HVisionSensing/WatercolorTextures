#include "pointgrabber.h"
#include "utils.h"

cv::Point *PointGrabber::findPointNear(cv::Point pivot, std::vector<cv::Point> &points, int maxAllowedDistance)
{
	return findPointNear(pivot, points.begin(), points.end(), maxAllowedDistance);
}

cv::Point *PointGrabber::findPointNear(cv::Point pivot, std::vector<cv::Point>::iterator begin, std::vector<cv::Point>::iterator end, int maxAllowedDistance)
{
	maxAllowedDistance *= maxAllowedDistance;

	for (std::vector<cv::Point>::iterator it = begin; it != end; ++it) {
		if (squareEuclDist(pivot, *it) <= maxAllowedDistance) return &(*it);
	}

	return 0;
}
