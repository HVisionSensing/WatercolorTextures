#include "imageresynth.h"
#include "patchmatch.h"

void ImageResynth::setPatchSize(int size)
{
	ASSERT(size >= 5 && size % 2 == 1)
	patchSize = size;
}

void ImageResynth::setIterCount(int count)
{
	ASSERT(count > 1)
	iterCount = count;
}

void ImageResynth::setResynthSource(cv::Mat *image)
{
	ASSERT(image->data != 0)
	rSource = image;
}

void ImageResynth::setResynthTarget(cv::Mat *image)
{
	ASSERT(image->data != 0)
	rTarget = image;
}

void ImageResynth::resynthesize()
{
	ASSERT(patchSize != -1 && iterCount != -1 && rSource != 0 && rTarget != 0)

	PatchMatch pm(*rSource, patchSize, *rTarget);
	pm.initialize();
	pm.iterate(iterCount);

	pm.getNnfImage().copyTo(*target);
	invalidate();
}

bool ImageResynth::keyListener(int key)
{
	bool response = false;
	
	switch (key) {
	// R
	case 82:
	case 114:
		resynthesize();
		response = true;
		*mask = opacity; // always show after resynthesis
		break;
	}

	response = response || StaticImage::keyListener(key);
	if (response) invalidate();

	return response;
}
