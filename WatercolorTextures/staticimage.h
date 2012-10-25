#ifndef STATIC_IMAGE
#define STATIC_IMAGE

#include "idisplaydevice.h"

class StaticImage : public IDisplayDevice
{
public:
	StaticImage() : maskKey(-2), opacity(255) {}

	void setMaskKey(int key);

	void setMaskKey(char key);

	void setOpacity(uchar opacity);

	void hideOnStartup();

	virtual void redraw();

	virtual bool mouseListener(int event, int x, int y, int flags, void* userdata = 0);

	virtual bool keyListener(int key);

protected:
	int maskKey;

	uchar opacity;
};

#endif
