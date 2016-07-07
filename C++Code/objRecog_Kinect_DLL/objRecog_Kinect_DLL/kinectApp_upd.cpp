#include "KinectApp.h"

/*
void KinectApp::calParams_c(int id, int& x, int& y, int& dir) {
	cv::Mat f = maskImg[id];
	//cv::GaussianBlur(f, f, cv::Size(7, 7), 10, 10);
	std::vector<cv::Vec3f> circles;
	// srcImg, dstVec, method, 分解能, minDist, 二値化閾値, 投票閾値, min半径, max半径 
	cv::HoughCircles(f, circles, CV_HOUGH_GRADIENT, 2, 6, 255, 30, 4, 40);
	tmp[id] = f;
	std::vector<cv::Vec3f>::iterator it = circles.begin();
	for (; it != circles.end(); ++it) {
		cv::Point center(cv::saturate_cast<int>((*it)[0]), cv::saturate_cast<int>((*it)[1]));
		int radius = cv::saturate_cast<int>((*it)[2]);
		cv::circle(tmp0, center, radius, cv::Scalar(0, 0, 255), 2);
	}
	
}
*/

bool KinectApp::calParams(int id, int& x, int& y, int& dir) {
	cv::Mat f = maskImg[id];
	int _x, _y, _dir = -1, cnt;
	int i, j, w, t, b, r, l;
	tmp[id] = f;

//#if MEDIAN_DEPTH==0
	// 画像1枚から判断しなきゃいけないとき
	// 画像の重心を求める
	if (!flag) {
		_x = 0, _y = 0, cnt = 0;
		for (i = 0; i < colorHeight; i++) {
			for (j = 0; j < colorWidth; j++) {
				if (f.at<uchar>(i, j)) { _x += j; _y += i; cnt++; }
			}
		}
		if (cnt) { imgX[id] = _x / cnt; imgY[id] = _y / cnt; } else { imgX[id] = 0; imgY[id] = 0; }
	}
//#endif

	// 画像の重心から探索範囲を狭める
	_x = imgX[id], _y = imgY[id]; cnt = 1;
	for (w = colorWidth / 4; cnt && (w*32 > colorWidth); w /= 2) {
		//calParamsSub(f, _x, _y, _dir, w);

		if ((t = _y - w) < 0) { t = 0; b = _y + w; }
		else (b = _y + w) < colorHeight ? 1 : b = colorHeight - 1;

		if ((l = _x - w) < 0) { l = 0; r = _x + w; } 
		else (r = _x + w) < colorWidth ? 1 : r = colorWidth - 1;

		_x = 0, _y = 0, cnt = 0;
		for (i = t; i <= b; i++) {
			for (j = l; j <= r; j++) {
				if (f.at<uchar>(i, j)) { _x += j; _y += i; cnt++; }
			}
		}
		if (cnt) { _x /= cnt; _y /= cnt; } 
		else { x = -1; y = -1; dir = -1; return false; }
	}

	tmp[id] = f;
	x = _x; y = _y;
	
	cv::Mat ctempMask = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
	cv::circle(ctempMask, cv::Point(_x, _y), DIR_CIRCLE, cv::Scalar(255, 255, 255), -1);
	ctempMask &= f;
	w = DIR_CIRCLE * 2.5;
	if ((t = _y - w) < 0) { t = 0; b = _y + w; } 
	else (b = _y + w) < colorHeight ? 1 : b = colorHeight - 1;
	if ((l = _x - w) < 0) { l = 0; r = _x + w; } 
	else (r = _x + w) < colorWidth ? 1 : r = colorWidth - 1;

	_x = 0, _y = 0, cnt = 0;
	for (i = t; i <= b; i++) {
		for (j = l; j <= r; j++) {
			if (ctempMask.at<uchar>(i, j)) { _x += j; _y += i; cnt++; }
		}
	}
	if (cnt) { _x /= cnt; _y /= cnt; } 
	else { x = -1; y = -1; dir = -1; return false; }


	dir = (int)(((atan2((double)(y - _y), (double)(x - _x)) + M_PI * 17./8.) / M_PI) * 4) % 8;



	cv::circle(tmp0, cv::Point(_x, _y), 60, cv::Scalar(255, 0, 0), 2);
	cv::line(tmp0, cv::Point(x, y), cv::Point(-5*(_x-x)+x, -5*(_y-y)+y), cv::Scalar(255, 0, 0), 5, 2);
	cv::line(tmp0, cv::Point(x, y), cv::Point(x + 40*dirVal[dir][0], y + 40*dirVal[dir][1]), cv::Scalar(0, 0, 255), 3, 2);
	//cv::rectangle(tmp0, cv::Point(x - colorWidth / 16, y - colorWidth / 16), cv::Point(x + colorWidth / 16, y + colorWidth / 16), cv::Scalar(255, 0, 0), 3, 4);

	return true;
}