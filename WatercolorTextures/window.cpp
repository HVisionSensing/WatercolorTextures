#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include "window.h"

int Window::instanceCounter = 0;

Window::Window(std::string name)
{
	instanceId = ++instanceCounter;

	if (name.length() == 0) {
		std::stringstream oss;
		oss << "Untitled " << instanceId;
		this->name = oss.str();
	} else {
		this->name = name;
	}
}

void Window::show()
{
	cv::imshow(name, *display.getComposite());
}
