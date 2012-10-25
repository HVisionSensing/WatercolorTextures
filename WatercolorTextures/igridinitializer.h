#ifndef I_GRID_INITIALIZER_H
#define I_GRID_INITIALIZER_H

class Grid;

class IGridInitializer
{
public:
	IGridInitializer() : parent(0) {}

	virtual ~IGridInitializer() {}

	virtual void setParent(Grid *grid) { parent = grid; }

	virtual void setParam(int key, double value) = 0;

	virtual void initializeGrid() = 0;

	bool hasParent() const { return parent != 0; }

protected:
	Grid *parent;
};

#define REGISTER_INITIALIZER_FRIEND(subClassName) friend void subClassName::initializeGrid();

#endif
