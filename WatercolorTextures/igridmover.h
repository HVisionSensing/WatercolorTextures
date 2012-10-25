#ifndef I_GRID_MOVER_H
#define I_GRID_MOVER_H

#include <vector>
#include <opencv2/core/core.hpp>

class Grid;

class IGridMover
{
public:
	IGridMover() : parent(0) {}

	virtual ~IGridMover() {}

	virtual void setParent(Grid *grid) { parent = grid; }

	virtual void setParam(int key, double value) = 0;

	virtual void moveGridPoints() = 0;

	bool hasParent() const { return parent != 0; }

protected:
	Grid *parent;
};

#define REGISTER_MOVER_FRIEND(subClassName) friend void subClassName::moveGridPoints();

#endif
