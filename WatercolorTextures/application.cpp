#include <opencv2/highgui/highgui.hpp>
#include "idisplaydevice.h"
#include "window.h"
#include "application.h"
#include "imagestorage.h"

Application::~Application()
{
	for (std::vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it)
		delete *it;
}

int Application::run()
{
	prepare();

	for (std::vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it) {
		cv::namedWindow((*it)->getName());
		cv::setMouseCallback((*it)->getName(), mouseListener, (void*)this);
		(*it)->show();
	}

	while (true) {
		int key = cv::waitKey(40);
		if (key == 27) break;

		// TODO - zde znovuvykreslit okna.
		for (std::vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it) {
			(*it)->show();
		}

		if (key == -1) continue;
		for (std::vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it) {
			bool processed = (*it)->getDisplay()->keyListener(key);
			if (processed) break;
		}
	}

	ImageStorage::cleanup();

	return 0;
}

void Application::mouseListener(int event, int x, int y, int flags, void* userdata)
{
	Application *app = (Application*)userdata;
	
	for (std::vector<Window*>::iterator it = app->windows.begin(); it != app->windows.end(); ++it) {
		bool processed = (*it)->getDisplay()->mouseListener(event, x, y, flags);
		if (processed) break;
	}
}
