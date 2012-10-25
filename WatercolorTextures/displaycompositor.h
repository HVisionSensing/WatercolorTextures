#ifndef DISPLAY_COMPOSITOR_H
#define DISPLAY_COMPOSITOR_H

#include <map>
#include <opencv2/core/core.hpp>

class IDisplayDevice;

class DisplayCompositor
{
public:
	DisplayCompositor() {}

	~DisplayCompositor();

	void setSize(cv::Size size);

	void addDisplay(IDisplayDevice *displayDevice, int layer = -1);

	void clearDisplayStack();

	int getLayerCount() const;

	cv::Mat *getComposite();

	bool mouseListener(int event, int x, int y, int flags, void* userdata = 0);

	bool keyListener(int key);

protected:
	void redraw();

protected:
	std::map<int, IDisplayDevice*> displays;

	cv::Mat composite;
};

#endif
