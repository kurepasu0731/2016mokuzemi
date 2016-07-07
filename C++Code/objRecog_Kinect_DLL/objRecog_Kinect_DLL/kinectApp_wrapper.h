#define DLLExport __declspec (dllexport)

extern "C"
{
	//**外部にさらす部分**//
	DLLExport void* openKinectApp();

	DLLExport void initialize(void* kinectApp);

	DLLExport bool calParams(void* kinectApp, int id, int position[]);

	DLLExport void update(void* kinectApp);

	DLLExport void draw(void* kinectApp);

	//ウィンドウ破棄
	DLLExport void destroyAllWindows()
	{
		cv::destroyAllWindows();
	};
}
