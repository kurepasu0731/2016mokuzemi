#include "Header.h"

KinectApp app;

// 0~3�L�[�Ŏ��������̃t���[�������w��ł���A�Ƃ����@�\�͔p�~
// �m�C�Y�����������팸�������ʁC���������͔������Ȃ�����

void main()
{
	try {	
		app.initialize();
		// FIFO_NUM�t�B���^���C�[�x�G���[臒l�C���̖ʐϔ͈́C��`�̕Ӕ͈�
		//app.setEnvParams(64, 35, 5, 100000, 5, 10000);
		// ����ID�C�����͈́C�F���͈́C�ʓx�͈́C���x�͈́C��`�Ӕ͈́C���̖ʐϔ͈́C�ǉ��p�����[�^
		//app.setObjParams(KinectApp::BANANA, 0, 400, 25, 35, 180, 255, 180, 255, 0, 1000, 0, 2000, 0.f, 0.f, 0.f, 0.f);
		app.objRecog();
	}
	catch (std::exception& ex){
		std::cout << ex.what() << std::endl;
	}
}