#include "staticimage.h"

void StaticImage::setMaskKey(int key)
{
	maskKey = key;
}

void StaticImage::setMaskKey(char key)
{
	maskKey = static_cast<int>(key);
}

void StaticImage::hideOnStartup()
{
	ASSERT(hasTarget())
	*mask = 0;
}

void StaticImage::setOpacity(uchar opacity)
{
	ASSERT(opacity > 0)
	this->opacity = opacity;
}

void StaticImage::redraw()
{
	redrawRequest = false;
}

bool StaticImage::mouseListener(int event, int x, int y, int flags, void* userdata)
{
	return false;
}

bool StaticImage::keyListener(int key)
{
	int complementKey = (maskKey >= 65 && maskKey <= 90) ? maskKey+32 : ((maskKey >= 97 && maskKey <= 122) ? maskKey-32 : -2); // if key is lower/capital alphabet letter, then this is its complement
	if (maskKey > 0 && (key == maskKey || key == complementKey)) {
		*mask = (mask->at<uchar>(0,0) == 0) ? opacity : 0;
		invalidate();
		return true;
	}
	return false;
}
