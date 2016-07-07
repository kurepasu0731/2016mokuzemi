#include "kinectApp.h"

void KinectApp::initialize() {
	// �f�t�H���g��Kinect���擾
	ERROR_CHECK(::GetDefaultKinectSensor(&kinect));
	ERROR_CHECK(kinect->Open());

	// ���W�ϊ��C���^�[�t�F�[�X���擾
	ERROR_CHECK(kinect->get_CoordinateMapper(&coordinateMapper));

	// �t���[���̏�����
	initializeColorFrame();
	initializeDepthFrame();

	WindowName[0] = "color";
	WindowName[1] = "banana";
	WindowName[2] = "depth";
	WindowName[3] = "thresh_depth";
	WindowName[4] = "d_depth";
	for (int i = 0; i < 5; i++) {
		cv::namedWindow(WindowName[i]);
	}
	cv::moveWindow(WindowName[0], 700, 0);
	cv::moveWindow(WindowName[1], 1200, 0);
	cv::moveWindow(WindowName[2], 700, 400);
	cv::moveWindow(WindowName[3], 1100, 400);
	cv::moveWindow(WindowName[4], 1500, 400);

	cv::FileStorage fs("../backImg.xml", cv::FileStorage::READ);
	if (fs.isOpened()) {
		std::cout << "backImg exist." << std::endl;
		fs["backImg"] >> backImg;
	}

	sflag = false;

	fs.release();

	flag = 0;

	dirVal[0][0] = dirVal[2][1] = 1.; 
	dirVal[0][1] = dirVal[2][0] = dirVal[4][1] = dirVal[6][0] = 0.;
	dirVal[1][0] = dirVal[1][1] = dirVal[3][1] = dirVal[7][0] = 1. / std::sqrt(2);
	dirVal[3][0] = dirVal[5][0] = dirVal[5][1] = dirVal[7][1] = -1. / std::sqrt(2);
	dirVal[4][0] = dirVal[6][1] = -1.;

	initObjParams(BANANA, "BANANA");
	initObjParams(TUNNEL, "TUNNEL");
	initObjParams(BRIDGE, "BRIDGE");
	initObjParams(APPLE, "APPLE");
	initObjParams(PET, "PET");
	initObjParams(CAN, "CAN");

	char buf[128];

	GetPrivateProfileStringA("Env", "FIFO_NUM", NULL, buf, sizeof(buf), "../parameter.ini");
	FIFO_NUM = atoi(buf);
	GetPrivateProfileStringA("Env", "DEPTH_THRESH", NULL, buf, sizeof(buf), "../parameter.ini");
	DEPTH_THRESH = atoi(buf);
	GetPrivateProfileStringA("Env", "THRESH_SIZE_MIN", NULL, buf, sizeof(buf), "../parameter.ini");
	THRESH_SIZE_MIN = atoi(buf);
	GetPrivateProfileStringA("Env", "THRESH_SIZE_MAX", NULL, buf, sizeof(buf), "../parameter.ini");
	THRESH_SIZE_MAX = atoi(buf);
	GetPrivateProfileStringA("Env", "THRESH_LENGTH_MIN", NULL, buf, sizeof(buf), "../parameter.ini");
	THRESH_LENGTH_MIN = atoi(buf);
	GetPrivateProfileStringA("Env", "THRESH_LENGTH_MAX", NULL, buf, sizeof(buf), "../parameter.ini");
	THRESH_LENGTH_MAX = atoi(buf);
}

void KinectApp::initObjParams(int obj_id, char* obj_name) {
	char buf[128];

	GetPrivateProfileStringA(obj_name, "dir", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].dir = (atoi(buf) != 0);

	GetPrivateProfileStringA(obj_name, "height_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].height_min = (UINT16)atoi(buf);
	GetPrivateProfileStringA(obj_name, "height_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].height_max = (UINT16)atoi(buf);

	GetPrivateProfileStringA(obj_name, "H_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].H_min = (uchar)atoi(buf);
	GetPrivateProfileStringA(obj_name, "H_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].H_max = (uchar)atoi(buf);
	GetPrivateProfileStringA(obj_name, "S_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].S_min = (uchar)atoi(buf);
	GetPrivateProfileStringA(obj_name, "S_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].S_max = (uchar)atoi(buf);
	GetPrivateProfileStringA(obj_name, "V_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].V_min = (uchar)atoi(buf);
	GetPrivateProfileStringA(obj_name, "V_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].V_max = (uchar)atoi(buf);

	GetPrivateProfileStringA(obj_name, "sq_length_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].sq_length_min = atoi(buf);
	GetPrivateProfileStringA(obj_name, "sq_length_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].sq_length_max = atoi(buf);
	GetPrivateProfileStringA(obj_name, "size_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].size_min = atoi(buf);
	GetPrivateProfileStringA(obj_name, "size_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].size_max = atoi(buf);
	GetPrivateProfileStringA(obj_name, "paramA_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].paramA_min = atof(buf);
	GetPrivateProfileStringA(obj_name, "paramA_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].paramA_max = atof(buf);
	GetPrivateProfileStringA(obj_name, "paramB_min", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].paramB_min = atof(buf);
	GetPrivateProfileStringA(obj_name, "paramB_max", NULL, buf, sizeof(buf), "../parameter.ini");
	objVal[obj_id].paramB_max = atof(buf);

}

void KinectApp::initializeColorFrame() {
	// �J���[���[�_�[���擾
	CComPtr<IColorFrameSource> colorFrameSource;
	ERROR_CHECK(kinect->get_ColorFrameSource(&colorFrameSource));
	ERROR_CHECK(colorFrameSource->OpenReader(&colorFrameReader));

	// �J���[�摜�̃T�C�Y���擾
	CComPtr<IFrameDescription> colorFrameDescription;
	ERROR_CHECK(colorFrameSource->CreateFrameDescription(COLOR_FORMAT, &colorFrameDescription));
	ERROR_CHECK(colorFrameDescription->get_Width(&colorWidth));
	ERROR_CHECK(colorFrameDescription->get_Height(&colorHeight));
	ERROR_CHECK(colorFrameDescription->get_BytesPerPixel(&colorBytesPerPixel));
	std::cout << "colorWidth      : " << colorWidth << std::endl;
	std::cout << "colorHeight     : " << colorHeight << std::endl;
	std::cout << "colorBytes/Pixel: " << colorBytesPerPixel << std::endl;

	cbufferSize = colorHeight * colorWidth * 4 * sizeof(unsigned char);

	// �o�b�t�@�쐬
	colorBuffer = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC4);
	
	hsvImg = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC3);
	
	maskImg = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
	NmedMaskImg = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); // ���f�B�A���t�B���^�p

	/* �����J���[�F�� & ���������g�p�t���[�����ϗp
	maskImg.resize(NUM_COLOR);
	NmedMaskImg.resize(NUM_COLOR);
	
	for (int i = 0; i < NUM_COLOR; i++) {
		maskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
		NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); //����͍��V�t�g������܂�������
		
		switch (flag) {
		case 1: // 8F�Ń��f�B�A��
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
			break;
		case 2: // 16F�Ń��f�B�A��
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_16UC1);
			break;
		case 3: // 32F�Ń��f�B�A��
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); //����͍��V�t�g������܂�������
			break;
		case 0:
		default:
			break;
		}
		
	}
	*/
	colorImg = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC4);
}

void KinectApp::initializeDepthFrame() {
	// Depth���[�_�[���擾
	CComPtr<IDepthFrameSource> depthFrameSource;
	ERROR_CHECK(kinect->get_DepthFrameSource(&depthFrameSource));
	ERROR_CHECK(depthFrameSource->OpenReader(&depthFrameReader));

	// Depth�摜�̃T�C�Y���擾
	CComPtr<IFrameDescription> depthFrameDescription;
	ERROR_CHECK(depthFrameSource->get_FrameDescription(&depthFrameDescription));
	ERROR_CHECK(depthFrameDescription->get_Width(&depthWidth));
	ERROR_CHECK(depthFrameDescription->get_Height(&depthHeight));
	std::cout << "depthWidth : " << depthWidth << std::endl;
	std::cout << "depthHeight: " << depthHeight << std::endl;

	dbufferSize = depthHeight * depthWidth * sizeof(UINT16);

	// Depth�̍ő�l�A�ŏ��l���擾
	UINT16 minDepthReliableDistance;
	UINT16 maxDepthReliableDistance;
	ERROR_CHECK(depthFrameSource->get_DepthMinReliableDistance(&minDepthReliableDistance));
	ERROR_CHECK(depthFrameSource->get_DepthMaxReliableDistance(&maxDepthReliableDistance));
	std::cout << "Depth�ŏ��l: " << minDepthReliableDistance << std::endl;
	std::cout << "Depth�ő�l: " << maxDepthReliableDistance << std::endl;

	// �o�b�t�@�쐬
	depthBuffer = cv::Mat::zeros(depthHeight, depthWidth, CV_16UC1);
	depthBufferVec.resize(depthHeight*depthWidth);

	dbsum = cv::Mat::zeros(depthHeight, depthWidth, CV_32SC1);
	backImg = cv::Mat::zeros(depthHeight, depthWidth, CV_16UC1);
	dbcnt = cv::Mat::zeros(depthHeight, depthWidth, CV_8UC1);

	colorSpace.resize(depthWidth * depthHeight);
}