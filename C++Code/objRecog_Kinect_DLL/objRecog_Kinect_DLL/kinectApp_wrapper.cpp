#include "kinectApp.h"
#include "kinectApp_dll.h"

DLLExport void* openKinectApp()
{
	return static_cast<void *>(new KinectApp());
}

DLLExport void initialize(void* kinectApp)
{
	auto app = static_cast<KinectApp*>(kinectApp);
	app->initialize();
}

DLLExport bool calParams(void* kinectApp, int id, int position[])
{
	auto app = static_cast<KinectApp*>(kinectApp);
	int x, y, dir;
	bool result = app->calParams(id, x, y, dir);
	position[0] = x;
	position[1] = y;
	position[2] = dir;

	return result;
}

DLLExport void update(void* kinectApp)
{
	auto app = static_cast<KinectApp*>(kinectApp);
	app->update();
}


DLLExport void draw(void* kinectApp)
{
	auto app = static_cast<KinectApp*>(kinectApp);
	app->draw();
}
