#ifndef SPARSE_GRID_H
#define SPARSE_GRID_H

#include "idisplaydevice.h"
#include "igridinitializer.h"
#include "igridmover.h"
#include "regulargridinitializer.h"
#include "randomgridmover.h"

class PointGrabber;

class Grid : public IDisplayDevice
{
	REGISTER_INITIALIZER_FRIEND(RegularGridInitializer)
	
	REGISTER_MOVER_FRIEND(RandomGridMover)

public:
	Grid();

	~Grid();

	void setInitializer(IGridInitializer *gridInitializer);

	void setMover(IGridMover *gridMover);

	void setSize(cv::Size size);

	cv::Size getSize() const;

	void initialize();

	const std::vector<std::vector<cv::Point>> *getPointHistory();

	int getFirstPointIndex() const;

	virtual void redraw();

	virtual bool mouseListener(int event, int x, int y, int flags, void* userdata = 0);

	virtual bool keyListener(int key);

protected:
	cv::Size size;

	IGridInitializer *initializer;

	IGridMover *mover;

	PointGrabber *pointGrabber;

	int statPtCount;

	std::vector<std::vector<cv::Point>> history;

	int t;
};

#endif
