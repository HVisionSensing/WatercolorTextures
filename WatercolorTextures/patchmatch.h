#ifndef PATCHMATCH_H
#define PATCHMATCH_H

#include <opencv2/core/core.hpp>

// 0 ~ nothing is excluded; 1 ~ exact same coordinate; other ~ euclidean distance
#define EXCLUDE_RADIUS 0

#define DEST_MASK 0

class PatchMatch
{
public:
	PatchMatch(cv::Mat &src, int patchSize, cv::Mat &dst = cv::Mat(), cv::Mat &mask = cv::Mat(), int maxRadius = -1, float radiusRatio = -1.0f);

	~PatchMatch();

	void initialize();

	void iterate();

	void iterate(int count);

	cv::Mat getNnfImage();

	cv::Mat getNnf();

	int patchSize() const { return ps; };

	cv::Mat source() const { return src; };

	cv::Mat destination() const { return dst; };

protected:
	int ssdError(cv::Mat &src, int xs, int ys, cv::Mat &dst, int xd, int yd, int prevErr);

protected:
	int ps;

	int pr;

	int pad;

	int iter;

	cv::Mat src;

	cv::Mat dst;

	cv::Mat nnf;

	cv::Mat mask;

	cv::Mat bounds;

	std::vector<int> R;
};

#endif
