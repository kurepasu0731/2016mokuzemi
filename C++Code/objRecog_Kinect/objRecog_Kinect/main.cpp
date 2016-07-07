#include "Header.h"

KinectApp app;

// 0~3キーで時平滑化のフレーム数が指定できる
// 詳しくは，KinectApp.hのrun()で

void main()
{
	try {		
		app.initialize();
		app.run();
	}
	catch (std::exception& ex){
		std::cout << ex.what() << std::endl;
	}
}