#ifndef TEXTURE_APPLICATION_H
#define TEXTURE_APPLICATION_H

#include <opencv2/core/core.hpp>
#include "application.h"

class Grid;

class TextureApplication : public Application
{
public:
	TextureApplication() : grid(0) {}

	TextureApplication(int argc, char **argv) : grid(0) { if (argc > 1) setBaseImage(argv[1]); }

	~TextureApplication() {}

	void setBaseImage(cv::Mat &img);

	void setBaseImage(std::string filename);

protected:
	virtual void prepare();

protected:
	cv::Mat baseImg;

	Grid *grid;
};

#endif
