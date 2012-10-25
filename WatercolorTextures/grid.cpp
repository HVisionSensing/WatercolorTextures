#include <opencv2/highgui/highgui.hpp>
#include "pointgrabber.h"
#include "igridinitializer.h"
#include "igridmover.h"
#include "grid.h"

Grid::Grid() :
	initializer(0), mover(0), pointGrabber(new PointGrabber()), statPtCount(0), size(-1,-1), t(-1)
{
}

Grid::~Grid()
{
	delete initializer;
	delete mover;
	delete pointGrabber;
}

void Grid::setInitializer(IGridInitializer *gridInitializer)
{
	initializer = gridInitializer;
	initializer->setParent(this);
}

void Grid::setMover(IGridMover *gridMover)
{
	mover = gridMover;
	mover->setParent(this);
}

void Grid::initialize()
{
	initializer->initializeGrid();
}

void Grid::redraw()
{
	ASSERT(t > 0);
	*mask = 0;
	redrawRequest = false;

	if (!visibility) return;

	for (size_t i = statPtCount; i < history[t].size();++ i) {
		cv::line(*target, history[t][i], history[t][i], cv::Scalar(128,128,128), 3);
		cv::line(*target, history[t-1][i], history[t][i], cv::Scalar(128,128,128), 1);
		cv::line(*mask, history[t][i], history[t][i], cv::Scalar(255), 3);
		cv::line(*mask, history[t-1][i], history[t][i], cv::Scalar(255), 1);
	}

	int i = 0;
	for (std::vector<cv::Point>::iterator it = history[t-1].begin(); it != history[t-1].end(); ++it, ++i) {
		cv::line(*target, *it, *it, i < statPtCount ? cv::Scalar(0,0,0) : cv::Scalar(255,255,255), 3);
		cv::line(*mask, *it, *it, cv::Scalar(255), 3);
	}

	if (pointGrabber->isActive()) {
		cv::Point *pt = pointGrabber->getActivePoint();
		cv::line(*target, *pt, *pt, cv::Scalar(0,255,0), 3);
	}
}

bool Grid::mouseListener(int event, int x, int y, int flags, void* userdata)
{
	switch (event) {
		case CV_EVENT_LBUTTONDOWN: {
			cv::Point *pt = pointGrabber->findPointNear(cv::Point(x,y), history[t].begin()+statPtCount, history[t].end());
			if (pt != 0 && !pointGrabber->isActive()) {
				pointGrabber->setActive(pt);
				invalidate();
				return true;
			} }
			break;

		case CV_EVENT_LBUTTONUP:
			if (pointGrabber->isActive()) {
				*pointGrabber->getActivePoint() = cv::Point(std::max(0, std::min(x, size.width-1)), std::max(0, std::min(y, size.height-1)));
				pointGrabber->setInactive();
				invalidate();
				return true;
			}
			break;

		case CV_EVENT_MOUSEMOVE:
			if (pointGrabber->isActive()) {
				*pointGrabber->getActivePoint() = cv::Point(x,y);
				invalidate();
				return true;
			}
			break;
	}

	return false;
}

bool Grid::keyListener(int key)
{
	switch (key) {
	// M
	case 77:
	case 109:
		mover->moveGridPoints();
		break;

	// P
	case 80:
	case 112:
		visibility = !visibility;
		invalidate();
		return true;
	default:
		return false;
	}
}

void Grid::setSize(cv::Size size)
{
	this->size = size;
}

cv::Size Grid::getSize() const
{
	return size;
}

const std::vector<std::vector<cv::Point>> *Grid::getPointHistory()
{
	return &history;
}

int Grid::getFirstPointIndex() const
{
	return statPtCount;
}
