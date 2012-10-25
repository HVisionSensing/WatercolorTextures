#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

class Window;

class Application
{
	friend void Application::mouseListener(int event, int x, int y, int flags, void* userdata);

public:
	Application() {}

	virtual ~Application();

	int run();

	static void mouseListener(int event, int x, int y, int flags, void* userdata = 0);

protected:
	virtual void prepare() = 0;

protected:
	std::vector<Window*> windows;
};

#endif
