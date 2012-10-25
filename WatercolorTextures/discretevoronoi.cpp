#include <opencv2/imgproc/imgproc.hpp>
#include "discretevoronoi.h"
#include "imagestorage.h"

void DiscreteVoronoi::setSize(cv::Size size)
{
	ASSERT(map.data == 0)
	map.create(size, CV_32S);
	setTarget(img("voronoi", size, CV_8UC3));
	hideOnStartup();
	setMaskKey('v');
}

void DiscreteVoronoi::updateSiteMap(std::vector<cv::Point>::const_iterator begin, std::vector<cv::Point>::const_iterator end, int firstPointIndex)
{
	ASSERT(map.data != 0)

	siteCount = 0;
	cv::Mat src(cv::Mat::ones(map.size(), CV_8U)), dst;
	for (std::vector<cv::Point>::const_iterator it = begin; it != end; ++it, ++siteCount)
		src.at<uchar>(*it) = 0;
	
	cv::distanceTransform(src, dst, map, CV_DIST_L2, 5);
	map += firstPointIndex-1;

	visualize(begin, end, firstPointIndex, firstPointIndex+siteCount-1);
	invalidate();
}

void DiscreteVoronoi::visualize(std::vector<cv::Point>::const_iterator begin, std::vector<cv::Point>::const_iterator end, int min, int max)
{
	cv::Mat tmp;
	map.convertTo(tmp, CV_32F);

	tmp = (tmp-min) / (max-min) * 255;
	tmp.convertTo(tmp, CV_8U);
	cv::cvtColor(tmp, tmp, CV_GRAY2BGR);
	tmp.copyTo(*target);

	for (std::vector<cv::Point>::const_iterator it = begin; it != end; ++it) {
		cv::line(*target, *it, *it, cv::Scalar(0,0,255), 5);
	}
}

bool DiscreteVoronoi::arePointsInSameCell(cv::Point &a, cv::Point &b)
{
	return map.at<int>(a) == map.at<int>(b);
}
