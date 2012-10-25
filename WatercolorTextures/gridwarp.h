#ifndef GRID_WARP
#define GRID_WARP

#include "staticimage.h"

class Grid;

class GridWarp : public StaticImage
{
public:
	/// Types enumerator.
	enum {
		RIGID = 1,
		SIMILARITY = 2,
		AFFINE = 4
	};

	GridWarp() : type(-1), points(0) {}

	void setType(int type);

	void setBaseImage(cv::Mat *image);

	void setPoints(const std::vector<std::vector<cv::Point>> *pointsHistory, int firstPointIndex);

	virtual bool keyListener(int key);

	void warp();

protected:
	int type;

	cv::Mat src;

	const std::vector<std::vector<cv::Point>> *points;

	int firstPointIndex;
};

#endif
