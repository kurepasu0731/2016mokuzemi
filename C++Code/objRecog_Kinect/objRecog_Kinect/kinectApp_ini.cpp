#include "kinectApp.h"
/*
void KinectApp::initializeDepthFrame() {
	// Depthリーダーを取得
	CComPtr<IDepthFrameSource> depthFrameSource;
	ERROR_CHECK(kinect->get_DepthFrameSource(&depthFrameSource));
	ERROR_CHECK(depthFrameSource->OpenReader(&depthFrameReader));

	// Depth画像のサイズを取得
	CComPtr<IFrameDescription> depthFrameDescription;
	ERROR_CHECK(depthFrameSource->get_FrameDescription(&depthFrameDescription));
	ERROR_CHECK(depthFrameDescription->get_Width(&depthWidth));
	ERROR_CHECK(depthFrameDescription->get_Height(&depthHeight));
	std::cout << "depthWidth : " << depthWidth << std::endl;
	std::cout << "depthHeight: " << depthHeight << std::endl;

	dbufferSize = depthHeight * depthWidth * sizeof(unsigned short);

	// Depthの最大値、最小値を取得
	UINT16 minDepthReliableDistance;
	UINT16 maxDepthReliableDistance;
	ERROR_CHECK(depthFrameSource->get_DepthMinReliableDistance(&minDepthReliableDistance));
	ERROR_CHECK(depthFrameSource->get_DepthMaxReliableDistance(&maxDepthReliableDistance));
	std::cout << "Depth最小値: " << minDepthReliableDistance << std::endl;
	std::cout << "Depth最大値: " << maxDepthReliableDistance << std::endl;

	// バッファ作成
	depthBuffer = cv::Mat::zeros(depthHeight, depthWidth, CV_16UC1);
}
*/

void KinectApp::initializeColorFrame() {
	// カラーリーダーを取得
	CComPtr<IColorFrameSource> colorFrameSource;
	ERROR_CHECK(kinect->get_ColorFrameSource(&colorFrameSource));
	ERROR_CHECK(colorFrameSource->OpenReader(&colorFrameReader));

	// カラー画像のサイズを取得
	CComPtr<IFrameDescription> colorFrameDescription;
	ERROR_CHECK(colorFrameSource->CreateFrameDescription(COLOR_FORMAT, &colorFrameDescription));
	ERROR_CHECK(colorFrameDescription->get_Width(&colorWidth));
	ERROR_CHECK(colorFrameDescription->get_Height(&colorHeight));
	ERROR_CHECK(colorFrameDescription->get_BytesPerPixel(&colorBytesPerPixel));
	std::cout << "colorWidth      : " << colorWidth << std::endl;
	std::cout << "colorHeight     : " << colorHeight << std::endl;
	std::cout << "colorBytes/Pixel: " << colorBytesPerPixel << std::endl;

	cbufferSize = colorHeight * colorWidth * 4 * sizeof(unsigned char);

	// バッファ作成
	colorBuffer = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC4);
	
	hsvImg = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC3);
	maskImg.resize(NUM_COLOR);
	NmedMaskImg.resize(NUM_COLOR);

	for (int i = 0; i < NUM_COLOR; i++) {
		maskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
		/*
		switch (flag) {
		case 1: // 8Fでメディアン
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
			break;
		case 2: // 16Fでメディアン
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_16UC1);
			break;
		case 3: // 32Fでメディアン
			NmedMaskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_32SC1); //今回は左シフトだからまあいいや
			break;
		case 0:
		default:
			break;
		}
		*/
	}
	tmp0 = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC4);
}