#pragma once

#include "Header.h"

#define ERROR_CHECK( ret ) \
if ((ret) != S_OK) { \
std::stringstream ss; \
ss << "failed " #ret " " << std::hex << ret << std::endl; \
throw std::runtime_error(ss.str().c_str()); \
}

#define BIT_MASK(n) ((unsigned int)1 << (n))

class KinectApp
{
public:

	~KinectApp() {
		// Kinect�̓�����I������
		if (kinect != nullptr) kinect->Close();
	}

	//**�O���o��**//
	void initialize() {
		kinect = nullptr;
		colorFrameReader = nullptr;

		// �f�t�H���g��Kinect���擾
		ERROR_CHECK(::GetDefaultKinectSensor(&kinect));
		ERROR_CHECK(kinect->Open());

		// ���W�ϊ��C���^�[�t�F�[�X���擾
		//ERROR_CHECK(kinect->get_CoordinateMapper(&coordinateMapper));

		// �t���[���̏�����
		//initializeDepthFrame();
		initializeColorFrame();

		setHSVparams();

		WindowName[0] = "0";
		WindowName[1] = "MARKER_B";
		WindowName[2] = "MARKER_G";
		WindowName[3] = "MARKER_R";
		for (int i = 0; i < 4; i++) {
			cv::namedWindow(WindowName[i]);
		}

		flag = 0;

		dirVal[0][0] = 1; dirVal[0][1] = 0;
		dirVal[1][0] = sqrt(2); dirVal[1][1] = sqrt(2);
		dirVal[2][0] = 0; dirVal[2][1] = 1;
		dirVal[3][0] = -sqrt(2); dirVal[3][1] = sqrt(2);
		dirVal[4][0] = -1; dirVal[4][1] = 0;
		dirVal[5][0] = -sqrt(2); dirVal[5][1] = -sqrt(2);
		dirVal[6][0] = 0; dirVal[6][1] = -1; 
		dirVal[7][0] = sqrt(2); dirVal[7][1] = -sqrt(2);
	}

	void run() {
		while (1) {
			update();
			//tmp0 = colorBuffer;
			int x, y, dir;
			calParams(0, x, y, dir);
			if (x!=-1&&y!=-1&&dir!=-1)
				std::cout << 0 << "->"
				<< "location: (" << x << ", " << y << ") , "
				<< "direction: " << dir << std::endl;
			calParams(1, x, y, dir);
			if (x != -1 && y != -1 && dir != -1)
				std::cout << 1 << "->"
				<< "location: (" << x << ", " << y << ") , "
				<< "direction: " << dir << std::endl;
			calParams(2, x, y, dir);
			if (x != -1 && y != -1 && dir != -1)
				std::cout << 2 << "->"
				<< "location: (" << x << ", " << y << ") , "
				<< "direction: " << dir << std::endl;

			draw();
			auto key = cv::waitKey(10);
			switch (key) {
			case 'q':
				return;
			case '0':
				flag = 0;
				break;
			case '1':
				flag = 1;
				for (int i = 0; i < NUM_COLOR; i++)
					NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
				break;
			case '2':
				flag = 2;
				for (int i = 0; i < NUM_COLOR; i++)
					NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_16UC1);
				break;
			case '3':
				flag = 3;
				for (int i = 0; i < NUM_COLOR; i++)
					NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); //����͍��V�t�g������܂�������
				break;
			default:
				break;
			}
		}
	}

	//**�O���o��**//
	bool calParams(int id, int& x, int& y, int& dir);

	//**�O���o��**//
	void update() {
		//updateDepthFrame();
		updateColorFrame();
		tmp0 = colorBuffer;
	}

	//**�O���o��**//
	void draw() {
		cv::resize(tmp0, tmp0, cv::Size(), 0.5, 0.5);
		cv::resize(tmp[0], tmp[0], cv::Size(), 0.25, 0.25);
		cv::resize(tmp[1], tmp[1], cv::Size(), 0.25, 0.25);
		cv::resize(tmp[2], tmp[2], cv::Size(), 0.25, 0.25);
		cv::imshow(WindowName[0], tmp0);
		cv::imshow(WindowName[1], tmp[0]);
		cv::imshow(WindowName[2], tmp[1]);
		cv::imshow(WindowName[3], tmp[2]);
	}

private:
	unsigned int flag; // ���́AMEDIAN_DEPTH�݂̂Ɏg��

	// Kinect
	CComPtr<IKinectSensor> kinect;
	//CComPtr<ICoordinateMapper> coordinateMapper = nullptr;

	// Color
	CComPtr<IColorFrameReader> colorFrameReader;

	int colorWidth, colorHeight;
	unsigned int colorBytesPerPixel;
	unsigned int cbufferSize;

	cv::Mat colorBuffer;
	cv::Mat hsvImg;
	short int hsv_thresh[NUM_COLOR][6];
	std::vector<cv::Mat> maskImg;
	std::vector<cv::Mat> NmedMaskImg;

	cv::Mat tmp0, tmp[NUM_COLOR];

	unsigned int imgX[NUM_COLOR], imgY[NUM_COLOR]; // �摜�d�S

	double dirVal[8][2];

	// Depth
	/*
	CComPtr<IDepthFrameReader> depthFrameReader = nullptr;

	int depthWidth, depthHeight;
	unsigned int dbufferSize;

	cv::Mat depthBuffer;
	*/

	char* WindowName[4];
	//void initializeDepthFrame();
	void initializeColorFrame();
	void setHSVparams();

	void updateDepthFrame();
	void updateColorFrame();

	// �F�}�[�J���ꂼ��ɕ���
	void separateColors();
	// �d�S�C�����v�Z
	void calParams_c(int id, int& x, int& y, int& dir); //�~���o�g����Ȃ�

};


// update
#include "kinectApp_upd-inl.h"