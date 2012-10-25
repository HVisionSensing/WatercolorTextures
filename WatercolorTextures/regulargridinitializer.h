#ifndef REGULAR_GRID_INITIALIZER_H
#define REGULAR_GRID_INITIALIZER_H

#include "igridinitializer.h"

class RegularGridInitializer : public IGridInitializer
{
public:
	/// Parameters enumerator.
	enum {
		HORIZONTAL_TILE_COUNT = 1,
		HORIZONTAL_TILE_SIZE = 2
	};

	RegularGridInitializer() : hTiles(-1), hTileSize(-1) {}

	virtual void setParam(int key, double value);

	virtual void initializeGrid();

protected:
	int hTiles;

	float hTileSize;
};

#endif
