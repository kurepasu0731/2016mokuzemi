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
	typedef struct _ObjectInfo {
		// ID
		int id;
		// 重心
		cv::Point g;
		//カラー座標変換後
		cv::Point g2Color;
		// 方向
		int dir;
		// 高さ
		UINT16 h;
		// 色(HSV)
		cv::Vec3b color;
		// 矩形サイズ
		cv::Point sq;
		// 矩形中心
		cv::Point sqo;
		// 面積
		int size;
		// 矩形中心原点の象限毎の面積
		int ssize[4];
	} ObjectInfo;

	typedef struct _ObjectRecVal {
		bool dir;
		
		UINT16 height_min; 
		UINT16 height_max;
		
		uchar H_min; 
		uchar H_max;
		
		uchar S_min; 
		uchar S_max;
		
		uchar V_min; 
		uchar V_max;
		
		int sq_length_min; 
		int sq_length_max;
		
		int size_min; 
		int size_max;
		
		float paramA_min; 
		float paramA_max;

		float paramB_min;
		float paramB_max;
	} ObjectRecVal;

	ObjectRecVal objVal[6];

	typedef enum obj_id {
		BANANA, TUNNEL, BRIDGE, APPLE, PET, CAN
	};

	~KinectApp() {
		// Kinectの動作を終了する
		if (kinect != nullptr) kinect->Close();
	}

	

	// パラメータ設定を行う
	void setEnvParams(
		int _FIFO_NUM,
		int _DEPTH_THRESH,
		int _THRESH_SIZE_MIN, int _THRESH_SIZE_MAX,
		int _THRESH_LENGTH_MIN, int _THRESH_LENGTH_MAX
		)
	{
		FIFO_NUM = _FIFO_NUM;
		
		DEPTH_THRESH = _DEPTH_THRESH;

		THRESH_SIZE_MIN = _THRESH_SIZE_MIN;
		THRESH_SIZE_MAX = _THRESH_SIZE_MAX;
		
		THRESH_LENGTH_MIN = _THRESH_LENGTH_MIN;
		THRESH_LENGTH_MAX = _THRESH_LENGTH_MAX;
	}

	void initObjParams(int, char*);
	
	void setObjParams(
		int obj_num,

		UINT16 height_min, UINT16 height_max,
		uchar H_min, uchar H_max,
		uchar S_min, uchar S_max,
		uchar V_min, uchar V_max,
		int sq_length_min, int sq_length_max,
		int size_min, int size_max,
		float paramA_min, float paramA_max
	)
	{
		objVal[obj_num].height_min = height_min; 
		objVal[obj_num].height_max = height_max;
		
		objVal[obj_num].H_min = H_min;
		objVal[obj_num].H_max = H_max;
		
		objVal[obj_num].S_min = S_min;
		objVal[obj_num].S_max = S_max;
		
		objVal[obj_num].V_min = V_min;
		objVal[obj_num].V_max = V_max;
		
		objVal[obj_num].sq_length_min = sq_length_min;
		objVal[obj_num].sq_length_max = sq_length_max;
		
		objVal[obj_num].size_min = size_min;
		objVal[obj_num].size_max = size_max;
		
		objVal[obj_num].paramA_min = paramA_min;
		objVal[obj_num].paramA_max = paramA_max;
	}

	// 物体認識を行う
	int objRecog()
	{
		std::cout << "\nobjRecog start.\n" << std::endl;
		while (1) {
			objcnt = 0; //一応debug用，vectorのsizeだけでどうにでもなる奴
			update();
			colorImg = colorBuffer;
			std::vector<ObjectInfo> obj;
			int x, y, dir;
			calParams_c(obj, maskImg, x, y, dir);
			if (sflag) calParams_d(obj, tdepImg, x, y, dir);
			std::cout << "\rtotal: " << obj.size() << "\t";

			std::vector<ObjectInfo>::iterator it = obj.begin();
			for (it = obj.begin(); it != obj.end(); ++it) {
				cv::circle(colorImg, (*it).g2Color, 40, cv::Scalar(0, 127, 0), 2);
				if ((*it).dir != -1) {
					cv::line(colorImg, (*it).g2Color, cv::Point(((*it).g2Color).x + 40 * dirVal[(*it).dir][0], ((*it).g2Color).y + 40 * dirVal[(*it).dir][1]), cv::Scalar(0, 127, 0), 3, 2);
					//std::cout << (*it).dir << std::endl;
				}
				std::cout << (*it).id << "_" << (*it).dir << ", ";
			}
			std::cout << "             ";
			draw();

			auto key = cv::waitKey(10);
			switch (key) {
			case 'q':
				return objcnt;
			
			case 'b': // 背景画像作成
				mkBackImg();
				std::cout << "\nmake backImg        \n" << std::endl;
				break;

			case 's': // save
				saveObjInfo(obj);
				return objcnt;

			case 'r': // reset
				ndb = 0;
				mkBackImg();
				sflag = false;
				break;

			/*
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
				NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); //今回は左シフトだからまあいいや
				break;
			*/

			default:
				break;
			}

		}
	}

	void initialize();

	void saveObjInfo(std::vector<ObjectInfo>& obj) {
		cv::FileStorage fs("../objRecog.xml", cv::FileStorage::WRITE);
		fs << "numObjects" << objcnt;
		for (int i = 0; i < objcnt; i++) {
			char idtag[8], xtag[8], ytag[8], dirtag[8];
			sprintf_s(idtag, "id%d", i);
			sprintf_s(xtag, "x%d", i);
			sprintf_s(ytag, "y%d", i);
			sprintf_s(dirtag, "dir%d", i);

			fs << idtag << obj[i].id
				<< xtag << obj[i].g2Color.x
				<< ytag << obj[i].g2Color.y
				<< dirtag << obj[i].dir;
		}
		fs.release();
	}

private:
	unsigned int flag; // 今は、MEDIAN_DEPTHのみに使う
	bool sflag;
	int objcnt;
	int FIFO_NUM;
	int DEPTH_THRESH;
	int THRESH_SIZE_MIN, THRESH_SIZE_MAX, THRESH_LENGTH_MIN, THRESH_LENGTH_MAX;

	// Kinect
	CComPtr<IKinectSensor> kinect = nullptr;
	CComPtr<ICoordinateMapper> coordinateMapper = nullptr;

	// Color
	CComPtr<IColorFrameReader> colorFrameReader = nullptr;

	int colorWidth, colorHeight;
	unsigned int colorBytesPerPixel;
	unsigned int cbufferSize;

	cv::Mat colorBuffer;

	cv::Mat hsvImg, colorImg, bananaImg/*csepImg[NUM_COLOR]*/;
	cv::Mat resize_colorImg, resize_bananaImg/*resize_csepImg[NUM_COLOR]*/;
	
	cv::Mat maskImg;
	cv::Mat NmedMaskImg;

	//std::vector<cv::Mat> maskImg;
	//std::vector<cv::Mat> NmedMaskImg;

	double dirVal[8][2];

	// Depth
	CComPtr<IDepthFrameReader> depthFrameReader = nullptr;

	int depthWidth, depthHeight;
	unsigned int dbufferSize;

	cv::Mat depthBuffer;
	std::vector<UINT16> depthBufferVec;
	std::vector<ColorSpacePoint> colorSpace;
	

	int ndb;

	cv::Mat depthImg, tdepImg, ddepImg;
	cv::Mat resize_depthImg, resize_tdepImg, resize_ddepImg;

	cv::Mat dbsum, backImg, dbcnt, ddepData;
	std::queue<cv::Mat> dbque;
	

	char* WindowName[5];
	
	
	
	void initializeDepthFrame();
	void initializeColorFrame();

	void updateDepthFrame();
	void updateColorFrame();

	// カラー分離
	//void separateColors();
	void mkHSVandSeparateBanana();

	// 重心，向き計算
	void calParams_c(std::vector<ObjectInfo>&, cv::Mat&, int& x, int& y, int& dir);
	void calCoG_c(cv::Mat&, cv::Point&, int&, cv::Point&, cv::Point&, cv::Point, cv::Point);
	void depth_thresh(int thresh_value);
	void calParams_d(std::vector<ObjectInfo>&, cv::Mat&, int& x, int& y, int& dir);
	void calCoG_d(cv::Mat&, cv::Point&, int&, cv::Point&, cv::Point&, cv::Point);
	
	// 取得情報から，物体の種類，向き特定を行う
	bool objRecogFunc(ObjectInfo&);
	bool findObjectID(ObjectInfo&);

	void mkBackImg() {
		backImg = cv::Mat::zeros(depthHeight, depthWidth, CV_16UC1);
		for (int i = 0; i < depthHeight; i++) {
			for (int j = 0; j < depthWidth; j++) {
				if (dbcnt.at<uchar>(i, j)) backImg.at<UINT16>(i, j) = dbsum.at<int>(i, j) / dbcnt.at<uchar>(i, j);
			}
		}
		cv::FileStorage fs("../backImg.xml", cv::FileStorage::WRITE);
		fs << "backImg" << backImg;
		fs.release();
		sflag = true;
	}

	void update() {
		updateColorFrame();
		updateDepthFrame();
	}

	void draw() {
		cv::resize(colorImg, resize_colorImg, cv::Size(), 0.25, 0.25);
		cv::resize(bananaImg, resize_bananaImg, cv::Size(), 0.25, 0.25);
		if (!depthImg.empty()) {
			cv::resize(depthImg, depthImg, cv::Size(), 0.75, 0.75);
			if (sflag) {
				cv::resize(tdepImg, resize_tdepImg, cv::Size(), 0.75, 0.75);
				cv::resize(ddepImg, resize_ddepImg, cv::Size(), 0.75, 0.75);
			}
		}
		cv::imshow(WindowName[0], resize_colorImg);
		cv::imshow(WindowName[1], resize_bananaImg);
		if (!depthImg.empty()) {
			cv::imshow(WindowName[2], depthImg);
			if (sflag) {
				cv::imshow(WindowName[3], resize_tdepImg);
				cv::imshow(WindowName[4], resize_ddepImg);
			}
		}
	}

};

// update
#include "kinectApp_upd-inl.h"