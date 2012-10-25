#include <opencv2/highgui/highgui.hpp>
#include "exception.h"
#include "grid.h"
#include "regulargridinitializer.h"
#include "randomgridmover.h"
#include "textureapplication.h"
#include "imagestorage.h"
#include "imageresynth.h"
#include "staticimage.h"
#include "textimage.h"
#include "gridwarp.h"

void TextureApplication::setBaseImage(cv::Mat &img)
{
	ASSERT(img.type() == CV_8UC3)
	baseImg = img;
}

void TextureApplication::setBaseImage(std::string filename)
{
	baseImg = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
}

// Constructs the actual application.
void TextureApplication::prepare()
{
 	ASSERT(baseImg.data != 0)

	// Base image.
	StaticImage *image = new StaticImage();
	image->setTarget(img("base", baseImg));

	// Text info (control keys).
	TextImage *info = new TextImage();
	info->setTarget(img("info", baseImg.size(), baseImg.type()));
	info->loadTextFile("info.txt");
	info->setMaskKey('i');
	info->hideOnStartup();

	// Grid.
	IGridInitializer *gridInitializer = new RegularGridInitializer();
	gridInitializer->setParam(RegularGridInitializer::HORIZONTAL_TILE_COUNT, 8);

	IGridMover *gridMover = new RandomGridMover();
	gridMover->setParam(RandomGridMover::MINIMAL_DISTANCE, 5);
	gridMover->setParam(RandomGridMover::MAXIMAL_DISTANCE, 10);

	grid = new Grid();
	grid->setSize(baseImg.size());
	grid->setInitializer(gridInitializer);
	grid->setMover(gridMover);
	grid->setTarget(img("grid", baseImg.size(), baseImg.type()));
	grid->initialize();

	// Warped image.
	GridWarp *warp = new GridWarp();
	warp->setType(GridWarp::RIGID);
	warp->setBaseImage(&baseImg);
	warp->setPoints(grid->getPointHistory(), grid->getFirstPointIndex());
	warp->setTarget(img("warp", baseImg.size(), baseImg.type()));
	warp->setMaskKey('q');
	warp->hideOnStartup();

	// Resynthesized image.
	ImageResynth *resynth = new ImageResynth();
	resynth->setPatchSize(5);
	resynth->setIterCount(5);
	resynth->setResynthSource(image->getTarget());
	resynth->setResynthTarget(warp->getTarget());
	resynth->setTarget(img("resynth", baseImg.size(), baseImg.type()));
	resynth->setMaskKey('e');
	resynth->hideOnStartup();

	// Window.
	Window *window = new Window("Test");
	window->addDevice(image);
	window->addDevice(warp);
	window->addDevice(resynth);
	window->addDevice((IDisplayDevice*)((RandomGridMover*)(gridMover))->getVoronoi());
	window->addDevice(grid);
	window->addDevice(info);
	windows.push_back(window);
}
