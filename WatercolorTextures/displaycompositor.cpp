#include "exception.h"
#include "idisplaydevice.h"
#include "displaycompositor.h"
#include "utils.h"

DisplayCompositor::~DisplayCompositor()
{
	for (std::map<int, IDisplayDevice*>::iterator it = displays.begin(); it != displays.end(); ++it)
		delete it->second;
}

void DisplayCompositor::setSize(cv::Size size)
{
	ASSERT(composite.data == 0)
	composite.create(size, CV_8UC3);
}

void DisplayCompositor::addDisplay(IDisplayDevice *displayDevice, int layer)
{
	layer = (layer == -1) ? (displays.empty() ? 0 : displays.rbegin()->first+1) : layer;
	if (composite.data == 0 && displayDevice->hasTarget()) setSize(displayDevice->getTarget()->size());

	ASSERT(
		displays.count(layer) == 0 &&
		displayDevice->hasTarget() &&
		displayDevice->getTarget()->size() == composite.size() &&
		displayDevice->getTarget()->type() == CV_8UC3
	)
	
	displays[layer] = displayDevice;
}

void DisplayCompositor::clearDisplayStack()
{
	displays.clear();
}

int DisplayCompositor::getLayerCount() const
{
	return displays.size();
}

cv::Mat *DisplayCompositor::getComposite()
{
	bool redrawRequest = false;
	for (std::map<int, IDisplayDevice*>::iterator it = displays.begin(); it != displays.end(); ++it)
		if (it->second->isRedrawNeeded()) {
			it->second->redraw();
			redrawRequest = true;
		}

	if (redrawRequest) redraw();

	return &composite;
}

void DisplayCompositor::redraw()
{
	cv::Mat tmp(composite.size(), composite.type());
	tmp.copyTo(composite);
	for (std::map<int, IDisplayDevice*>::iterator it = displays.begin(); it != displays.end(); ++it) {
		//blend(composite, *it->second->getTarget(), composite, *it->second->getMask(), CV_32F);
		it->second->getTarget()->copyTo(composite, *it->second->getMask());
	}
}

bool DisplayCompositor::mouseListener(int event, int x, int y, int flags, void* userdata)
{
	bool processed = false;
	for (std::map<int, IDisplayDevice*>::iterator it = displays.begin(); it != displays.end(); ++it) {
		processed = processed || it->second->mouseListener(event, x, y, flags, userdata);
	}
	return processed;
}

bool DisplayCompositor::keyListener(int key)
{
	bool processed = false;
	for (std::map<int, IDisplayDevice*>::iterator it = displays.begin(); it != displays.end(); ++it) {
		processed = processed || it->second->keyListener(key);
	}
	return processed;
}
