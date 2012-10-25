#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>
#include "exception.h"
#include "displaycompositor.h"

class Window
{
public:
	Window(std::string name = "");

	~Window() {};

	void setName(std::string name) { ASSERT(name.length() > 0) this->name = name; }

	std::string getName() const { return name; }

	void addDevice(IDisplayDevice *displayDevice, int layer = -1) { display.addDisplay(displayDevice, layer); }

	void show();

	DisplayCompositor *getDisplay() { return &display; }

protected:
	static int instanceCounter;

	int instanceId;

	std::string name;

	DisplayCompositor display;
};

#endif
