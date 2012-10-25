#ifndef DISCRETE_VORONOI
#define DISCRETE_VORONOI

#include "staticimage.h"

class DiscreteVoronoi : public StaticImage
{
public:
	DiscreteVoronoi() : siteCount(0) {}

	void setSize(cv::Size size);

	void updateSiteMap(std::vector<cv::Point>::const_iterator begin, std::vector<cv::Point>::const_iterator end, int firstPointIndex = 0);

	bool arePointsInSameCell(cv::Point &a, cv::Point &b);

protected:
	void visualize(std::vector<cv::Point>::const_iterator begin, std::vector<cv::Point>::const_iterator end, int min, int max);

protected:
	int siteCount;

	cv::Mat map;
};

#endif
