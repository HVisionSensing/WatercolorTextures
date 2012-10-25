#ifndef RANDOM_GRID_MOVER
#define RANDOM_GRID_MOVER

#include "igridmover.h"

class DiscreteVoronoi;

class RandomGridMover : public IGridMover
{
public:
	/// Parameters enumerator.
	enum {
		MINIMAL_DISTANCE = 1,
		MAXIMAL_DISTANCE = 2
	};

	RandomGridMover();

	virtual void setParam(int key, double value);

	virtual void setParent(Grid *grid);

	virtual void moveGridPoints();

	DiscreteVoronoi *getVoronoi();

protected:
	int minDist;

	int maxDist;

	DiscreteVoronoi *voronoi;
};

#endif
