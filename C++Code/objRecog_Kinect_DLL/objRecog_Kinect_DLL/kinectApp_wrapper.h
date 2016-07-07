#define DLLExport __declspec (dllexport)

extern "C"
{
	//**�O���ɂ��炷����**//
	DLLExport void* openKinectApp();

	DLLExport void initialize(void* kinectApp);

	DLLExport bool calParams(void* kinectApp, int id, int position[]);

	DLLExport void update(void* kinectApp);

	DLLExport void draw(void* kinectApp);

	//�E�B���h�E�j��
	DLLExport void destroyAllWindows()
	{
		cv::destroyAllWindows();
	};
}
