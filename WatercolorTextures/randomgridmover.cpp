#include "exception.h"
#include "discretevoronoi.h"
#include "grid.h"
#include "randomgridmover.h"
#include "utils.h"

RandomGridMover::RandomGridMover() :
	minDist(-1), maxDist(-1), voronoi(new DiscreteVoronoi())
{
}

void RandomGridMover::setParent(Grid *grid)
{
	parent = grid;
	voronoi->setSize(grid->getSize());
}

DiscreteVoronoi *RandomGridMover::getVoronoi()
{
	return voronoi;
}

void RandomGridMover::setParam(int key, double value)
{
	switch (key) {
	case MINIMAL_DISTANCE:
		ASSERT(value > 0 && (maxDist == -1 || static_cast<int>(value) < maxDist))
		minDist = static_cast<int>(value);
		break;
	case MAXIMAL_DISTANCE:
		ASSERT(value > 1 && (minDist == -1 || static_cast<int>(value) > minDist + 1))
		maxDist = static_cast<int>(value);
		break;
	default:
		THROW(E_UnkParReq)
	}
}

void RandomGridMover::moveGridPoints()
{
	ASSERT(parent->t > 0 && minDist != -1 && maxDist != -1 && minDist < maxDist)

	voronoi->updateSiteMap(parent->history[parent->t].begin()+parent->statPtCount, parent->history[parent->t].end(), parent->statPtCount);
	
	int i = parent->statPtCount;
	for (std::vector<cv::Point>::iterator it = parent->history[parent->t].begin()+parent->statPtCount; it != parent->history[parent->t].end(); ++it, ++i) {
		cv::Rect bounds(
			cv::Point(
				std::max(it->x-maxDist, 0),
				std::max(it->y-maxDist, 0)
			),
			cv::Point(
				std::min(it->x+maxDist, parent->size.width),
				std::min(it->y+maxDist, parent->size.height)
			)
		);
		
		float sqDist, minSqDist = static_cast<float>(minDist*minDist), maxSqDist = static_cast<float>(maxDist*maxDist);
		cv::Point pt;
		do {
			pt = cv::Point(rand() % bounds.width + bounds.x, rand() % bounds.height + bounds.y);
			sqDist = squareEuclDist(pt, *it);
		} while (sqDist < minSqDist || sqDist > maxSqDist || !voronoi->arePointsInSameCell(pt, *it));

		*it = pt;
	}

	parent->invalidate();
}
