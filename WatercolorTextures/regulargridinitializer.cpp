#include "exception.h"
#include "grid.h"
#include "regulargridinitializer.h"

void RegularGridInitializer::setParam(int key, double value)
{
	switch (key) {
	case HORIZONTAL_TILE_COUNT:
		ASSERT(value >= 2)
		hTiles = static_cast<int>(value);
		hTileSize = -1;
		break;
	case HORIZONTAL_TILE_SIZE:
		ASSERT(value >= 10)
		hTileSize = static_cast<float>(value);
		hTiles = -1;
		break;
	default:
		THROW(E_UnkParReq)
	}
}

void RegularGridInitializer::initializeGrid()
{
	ASSERT(hasParent() && (hTiles != -1 || hTileSize != -1))

	if (hTiles != -1) hTileSize = static_cast<float>(parent->size.width) / static_cast<float>(hTiles);
	int vTiles = static_cast<int>(floor(.5f + static_cast<float>(parent->size.height) / hTileSize));
	float vTileSize = static_cast<float>(parent->size.height) / static_cast<float>(vTiles);

	parent->history.clear();
	std::vector<cv::Point> cage, points;

	for (float fy = 0; fy <= parent->size.height; fy += vTileSize) {
		int y = static_cast<int>(std::min(fy, static_cast<float>(parent->size.height-1)));
		
		for (float fx = 0; fx <= parent->size.width; fx += hTileSize) {
			int x = static_cast<int>(std::min(fx, static_cast<float>(parent->size.width-1)));

			if (y == 0 || y == parent->size.height-1 || x == 0 || x == parent->size.width-1) {
				cage.push_back(cv::Point(x,y));
			} else {
				points.push_back(cv::Point(x,y));
			}
		}
	}

	parent->t = 1;
	parent->statPtCount = cage.size();
	cage.insert(cage.end(), points.begin(), points.end());
	parent->history.push_back(cage);
	parent->history.push_back(cage);
}

