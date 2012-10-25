#ifndef IMAGE_RESYNTH
#define IMAGE_RESYNTH

#include "staticimage.h"

class ImageResynth : public StaticImage
{
public:
	ImageResynth() : patchSize(-1), iterCount(-1), rSource(0), rTarget(0) {}

	void setPatchSize(int size);

	void setIterCount(int count);

	void setResynthSource(cv::Mat *image);

	void setResynthTarget(cv::Mat *image);

	void resynthesize();

	virtual bool keyListener(int key);

protected:
	int patchSize;

	int iterCount;

	cv::Mat *rSource;

	cv::Mat *rTarget;
};

#endif