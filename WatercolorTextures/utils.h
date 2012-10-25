#ifndef UTILS_H
#define UTILS_H

#include "exception.h"
#include <opencv2/core/core.hpp>

template <typename _Tp>
struct PointComparer_
{
	PointComparer_() {}
	PointComparer_(const PointComparer_ &other) {}
	bool operator()(const cv::Point_<_Tp> &a, const cv::Point_<_Tp> &b)
	{
		return (a.y < b.y || (a.y == b.y && a.x < b.x));
	}
};

typedef PointComparer_<float> Point2fComparer;
typedef PointComparer_<int> Point2iComparer;
typedef Point2iComparer PointComparer;

template <typename _Tp1, typename _Tp2>
_Tp2 euclDist(const cv::Point_<_Tp1> &a, const cv::Point_<_Tp1> &b)
{
	_Tp1 dx = a.x - b.x;
	_Tp1 dy = a.y - b.y;
	return static_cast<_Tp2>(sqrt(static_cast<double>(dx*dx + dy*dy)));
}

template <typename _Tp>
_Tp squareEuclDist(const cv::Point_<_Tp> &a, const cv::Point_<_Tp> &b)
{
	_Tp dx = a.x - b.x;
	_Tp dy = a.y - b.y;
	return dx*dx + dy*dy;
}

/// Returns (1-w)*src1 + w*src2 (assuming w = <0;1>).
void blend(cv::Mat &src1, cv::Mat &src2, cv::Mat &dst, cv::Mat &weight, int itype = CV_64F, double min = 0, double max = 255);

std::vector<std::string> explode(const std::string &string, const char &delim);

#endif
