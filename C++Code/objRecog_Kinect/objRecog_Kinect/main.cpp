#include "Header.h"

KinectApp app;

// 0~3�L�[�Ŏ��������̃t���[�������w��ł���
// �ڂ����́CKinectApp.h��run()��

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