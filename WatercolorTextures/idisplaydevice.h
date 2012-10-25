#ifndef I_DISPLAY_DEVICE_H
#define I_DISPLAY_DEVICE_H

#include <map>
#include <string>
#include <opencv2/core/core.hpp>
#include "window.h"

class IDisplayDevice
{
public:
	IDisplayDevice() : target(0), mask(0), redrawRequest(true), visibility(true) {}

	virtual ~IDisplayDevice() { delete mask; }

	void setTarget(cv::Mat *target) { ASSERT(this->target == 0) this->target = target; if (mask == 0) mask = new cv::Mat(); mask->create(target->size(), CV_8U); mask->setTo(255); }

	cv::Mat *getTarget() { return target; }

	bool hasTarget() const { return target != 0; }

	cv::Mat *getMask() { return mask; }

	Window *createParentWindow(std::string name = "") { Window *w = new Window(); w->setName(name); w->addDevice(this); return w; } 

	void invalidate() { redrawRequest = true; }

	bool isRedrawNeeded() const { return redrawRequest; }

	virtual void redraw() = 0;

	virtual bool mouseListener(int event, int x, int y, int flags, void* userdata = 0) = 0;

	virtual bool keyListener(int key) = 0;

protected:
	bool visibility;

	cv::Mat *target;

	cv::Mat *mask;

	bool redrawRequest;
};

#endif
