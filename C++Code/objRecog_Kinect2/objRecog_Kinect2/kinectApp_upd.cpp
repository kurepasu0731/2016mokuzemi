#include "KinectApp.h"

void KinectApp::mkHSVandSeparateBanana() {
	cv::GaussianBlur(colorBuffer, hsvImg, cv::Size(11, 11), 15, 15);
	cv::cvtColor(hsvImg, hsvImg, CV_BGR2HSV);

	cv::Vec3b p;
	double sum;
	unsigned int cnt;

	maskImg = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
	sum = 0; cnt = 0;
	if (objVal[BANANA].H_min < objVal[BANANA].H_max) {
		for (int h = 0; h < colorHeight; h++) {
			for (int w = 0; w < colorWidth; w++) {
				p = hsvImg.at<cv::Vec3b>(h, w);
				if (objVal[BANANA].H_min <= p[0] && objVal[BANANA].H_max >= p[0]) {
					sum += maskImg.at<uchar>(h, w) = (((p[1]>objVal[BANANA].S_min && p[1]<objVal[BANANA].S_max) ? p[1] : 0) * ((p[2]>objVal[BANANA].V_min && p[2] < objVal[BANANA].V_max) ? p[2] : 0)) / 255;
					cnt++;
				}
			}
		}
	} else {
		for (int h = 0; h < colorHeight; h++) {
			for (int w = 0; w < colorWidth; w++) {
				p = hsvImg.at<cv::Vec3b>(h, w);
				if (objVal[BANANA].H_min <= p[0] || objVal[BANANA].H_max >= p[0]) {
					sum += maskImg.at<uchar>(h, w) = (((p[1]>objVal[BANANA].S_min && p[1]<objVal[BANANA].S_max) ? p[1] : 0) * ((p[2]>objVal[BANANA].V_min && p[2] < objVal[BANANA].V_max) ? p[2] : 0)) / 255;
					cnt++;
				}
			}
		}
	}
	sum = cnt ? sum / cnt : 0;

	cv::GaussianBlur(maskImg, maskImg, cv::Size(11, 11), 15, 15);

	cv::threshold(maskImg, maskImg, sum, 255, CV_THRESH_BINARY);

	for (int h = 0; h < colorHeight; h++) {
		for (int w = 0; w < colorWidth; w++) {
			NmedMaskImg.at<int>(h, w) = (NmedMaskImg.at<int>(h, w) << 1) | (int)(maskImg.at<uchar>(h, w) & (uchar)1);
			unsigned int m = (unsigned int)(NmedMaskImg.at<int>(h, w));
			// bitカウント
			m = (m & 0x55555555) + ((m >> 1) & 0x55555555);  //  2bitずつ
			m = (m & 0x33333333) + ((m >> 2) & 0x33333333);  //  4bitずつ
			m = (m & 0x0f0f0f0f) + ((m >> 4) & 0x0f0f0f0f);  //  8bitずつ
			m = (m & 0x00ff00ff) + ((m >> 8) & 0x00ff00ff);  // 16bitずつ
			m = (m & 0x0000ffff) + ((m >> 16) & 0x0000ffff); // 完成

			// median
			maskImg.at<uchar>(h, w) = (uchar)(255 * (m > 16));
		}
	}
}

/*
void KinectApp::separateColors() {
cv::GaussianBlur(colorBuffer, hsvImg, cv::Size(11, 11), 15, 15);
cv::cvtColor(hsvImg, hsvImg, CV_BGR2HSV);

cv::Vec3b p;
double sum;
unsigned int cnt;

unsigned int _x, _y; // この段階で画像重心求めていいのなら

for (int i = 0; i < NUM_COLOR; i++) {
maskImg[i] = cv::Mat::zeros(colorHeight, colorWidth, CV_8UC1);
sum = 0; cnt = 0;
if (hsv_thresh[i][0] < hsv_thresh[i][1]) {
for (int h = 0; h < colorHeight; h++) {
for (int w = 0; w < colorWidth; w++) {
p = hsvImg.at<cv::Vec3b>(h, w);
if (hsv_thresh[i][0] <= p[0] && hsv_thresh[i][1] >= p[0]) {
sum += maskImg[i].at<uchar>(h, w) = (((p[1]>hsv_thresh[i][2] && p[1]<hsv_thresh[i][3]) ? p[1] : 0) * ((p[2]>hsv_thresh[i][4] && p[2]<hsv_thresh[i][5]) ? p[2] : 0)) / 255;
cnt++;
}
}
}
} else {
for (int h = 0; h < colorHeight; h++) {
for (int w = 0; w < colorWidth; w++) {
p = hsvImg.at<cv::Vec3b>(h, w);
if (hsv_thresh[i][0] <= p[0] || hsv_thresh[i][1] >= p[0]) {
sum += maskImg[i].at<uchar>(h, w) = (((p[1]>hsv_thresh[i][2] && p[1]<hsv_thresh[i][3]) ? p[1] : 0) * ((p[2]>hsv_thresh[i][4] && p[2]<hsv_thresh[i][5]) ? p[2] : 0)) / 255;
cnt++;
}
}
}
}
sum = cnt ? sum / cnt : 0;

cv::GaussianBlur(maskImg[i], maskImg[i], cv::Size(11, 11), 15, 15);

cv::threshold(maskImg[i], maskImg[i], sum, 255, CV_THRESH_BINARY);



if (flag) {
_x = 0; _y = 0; cnt = 0;
// ここで時間の考慮がされる
switch (flag) {
case 3:
for (int h = 0; h < colorHeight; h++) {
for (int w = 0; w < colorWidth; w++) {
NmedMaskImg[i].at<int>(h, w) = (NmedMaskImg[i].at<int>(h, w) << 1) | (int)(maskImg[i].at<uchar>(h, w) & (uchar)1);
unsigned int m = (unsigned int)(NmedMaskImg[i].at<int>(h, w));
// bitカウント
m = (m & 0x55555555) + ((m >> 1) & 0x55555555);  //  2bitずつ
m = (m & 0x33333333) + ((m >> 2) & 0x33333333);  //  4bitずつ
m = (m & 0x0f0f0f0f) + ((m >> 4) & 0x0f0f0f0f);  //  8bitずつ
m = (m & 0x00ff00ff) + ((m >> 8) & 0x00ff00ff);  // 16bitずつ
m = (m & 0x0000ffff) + ((m >> 16) & 0x0000ffff); // 完成

// median
maskImg[i].at<uchar>(h, w) = (uchar)(255 * (m > 16));
if (m > 15) { _x += w; _y += h; cnt++; }
}
}
break;

case 2:
for (int h = 0; h < colorHeight; h++) {
for (int w = 0; w < colorWidth; w++) {
NmedMaskImg[i].at<UINT16>(h, w) = (NmedMaskImg[i].at<UINT16>(h, w) << 1) | (UINT16)(maskImg[i].at<uchar>(h, w) & (uchar)1);
UINT16 m = NmedMaskImg[i].at<UINT16>(h, w);
// bitカウント
m = (m & 0x5555) + ((m >> 1) & 0x5555);  // 2bitずつ
m = (m & 0x3333) + ((m >> 2) & 0x3333);  // 4bitずつ
m = (m & 0x0f0f) + ((m >> 4) & 0x0f0f);  // 8bitずつ
m = (m & 0x00ff) + ((m >> 8) & 0x00ff);  // 完成

// median
maskImg[i].at<uchar>(h, w) = (uchar)(255 * (m > 8));
if (m > 7) { _x += w; _y += h; cnt++; }
}
}
break;

case 1:
for (int h = 0; h < colorHeight; h++) {
for (int w = 0; w < colorWidth; w++) {
NmedMaskImg[i].at<uchar>(h, w) = (NmedMaskImg[i].at<uchar>(h, w) << 1) | (maskImg[i].at<uchar>(h, w) & (uchar)1);
uchar m = NmedMaskImg[i].at<uchar>(h, w);
// bitカウント
m = (m & 0x55) + ((m >> 1) & 0x55);  // 2bitずつ
m = (m & 0x33) + ((m >> 2) & 0x33);  // 4bitずつ
m = (m & 0x0f) + ((m >> 4) & 0x0f);  // 完成

// median
maskImg[i].at<uchar>(h, w) = (uchar)(255 * (m > 4));
if (m > 7) { _x += w; _y += h; cnt++; }
}
}
break;
default:
break;
}
if (cnt) { imgX[i] = _x / cnt; imgY[i] = _y / cnt; }
else { imgX[i] = 0; imgY[i] = 0; }
}



}
}
*/

void KinectApp::calParams_c(std::vector<ObjectInfo>& obj, cv::Mat& img, int& x, int& y, int& dir) 
{
	cv::Point sum, min, max, p, tmpg, _sq, _sqo;
	int cnt, ssize[4];
	cv::Mat f = img.clone();
	bananaImg = maskImg;

	for (int i = 0; i < colorHeight; i++) {
		for (int j = 0; j < colorWidth; j++) {
			if (f.at<uchar>(i, j)) {
				sum = cv::Point(0, 0); cnt = 0; min = cv::Point(j, i); max = cv::Point(j, i);
				calCoG_c(f, sum, cnt, min, max, cv::Point(j, i), cv::Point(j, i));

				_sq = max - min; _sqo = (max + min) * .5;

				if (cnt>THRESH_SIZE_MIN && cnt<THRESH_SIZE_MAX 
					&& _sq.x > THRESH_LENGTH_MIN && _sq.x < THRESH_LENGTH_MAX 
					&& _sq.y > THRESH_LENGTH_MIN && _sq.y < THRESH_LENGTH_MAX) { // _sq.y / _sq.x等もエッジカットに使えそう

					tmpg = cv::Point(j + sum.x / cnt, i + sum.y / cnt);

					ssize[0] = 0; ssize[1] = 0; ssize[2] = 0; ssize[3] = 0;
					for (int k = 1; k <= _sq.y / 2; k++) {
						for (int l = 1; l <= _sq.x / 2; l++) {
							if (img.at<uchar>(_sqo.y - k, _sqo.x - l)) ssize[0]++;
							if (img.at<uchar>(_sqo.y - k, _sqo.x + l)) ssize[1]++;
							if (img.at<uchar>(_sqo.y + k, _sqo.x + l)) ssize[2]++;
							if (img.at<uchar>(_sqo.y + k, _sqo.x - l)) ssize[3]++;
						}
					}

					// id, g(x,y), g2Color(x,y), dir, h, color(hsv), sq(x,y), sqo(x,y), size, ssize[4]
					ObjectInfo tmpobj = {
						BANANA,
						tmpg,
						tmpg,
						-1,
						0,
						hsvImg.at<cv::Vec3b>(tmpg),
						_sq,
						_sqo,
						cnt,
						{ ssize[0], ssize[1], ssize[2], ssize[3] }
					};
					
					if (objRecogFunc(tmpobj)) {
						obj.push_back(tmpobj);
						objcnt++;
					}
				}
			}
		}
	}

	/*
	for (int cid = 0; cid < NUM_COLOR; cid++) {
		cv::Mat f = maskImg[cid].clone();
		csepImg[cid] = maskImg[cid];
		for (int i = 0; i < colorHeight; i++) {
			for (int j = 0; j < colorWidth; j++) {
				if (f.at<uchar>(i, j)) {
					sum = cv::Point(0, 0); cnt = 0; min = cv::Point(j, i); max = cv::Point(j, i);
					calCoG(f, sum, cnt, min, max, cv::Point(j, i), cv::Point(j, i));
					if (cnt>THRESH_VAL_MIN && max.x - min.x > THRESH_VAL_MIN_X && max.y - min.y > THRESH_VAL_MIN_Y) {
						// g(x,y), dir, sq(x,y), sqo(x,y), size
						obj.push_back({ 
							0,
							cv::Point(j + sum.x / cnt, i + sum.y / cnt),
							0,
							max - min,
							(max + min) * .5,
							cnt });
					}
				}
			}
		}
	}
	*/
}

void KinectApp::calCoG_c(cv::Mat& f, cv::Point& sum, int& cnt, cv::Point& min, cv::Point& max, cv::Point p, cv::Point start)
{
	if (f.at<uchar>(p)) {
		sum += (p-start); cnt++;
		f.at<uchar>(p) = 0;
		if (p.x<min.x) min.x = p.x;
		if (p.x>max.x) max.x = p.x;
		if (p.y<min.y) min.y = p.y;
		if (p.y>max.y) max.y = p.y;

		if (p.x - 1 >= 0) calCoG_c(f, sum, cnt, min, max, cv::Point(p.x - 1, p.y), start);
		if (p.x + 1 < colorWidth) calCoG_c(f, sum, cnt, min, max, cv::Point(p.x + 1, p.y), start);
		if (p.y - 1 >= 0) calCoG_c(f, sum, cnt, min, max, cv::Point(p.x, p.y - 1), start);
		if (p.y + 1 < colorHeight) calCoG_c(f, sum, cnt, min, max, cv::Point(p.x, p.y + 1), start);
	}
}


void KinectApp::depth_thresh(int thresh_value)
{
	//threshImg = cv::abs(dbsum - backImg);
	//threshImg.convertTo(threshImg, CV_8U, 255. / 256000., 0.);
	//cv::threshold(threshImg, threshImg, thresh_value, 255, CV_THRESH_BINARY);
	tdepImg = cv::Mat::zeros(depthHeight, depthWidth, CV_8UC1);
	ddepImg = cv::Mat::zeros(depthHeight, depthWidth, CV_8UC1);
	ddepData = cv::Mat::zeros(depthHeight, depthWidth, CV_16UC1);
	UINT16 tmpv, tmpd;
	for (int i = 0; i < depthHeight; i++) {
		for (int j = 0; j < depthWidth; j++) {
			tmpv = dbcnt.at<uchar>(i, j) ? (UINT16)(dbsum.at<int>(i, j) / dbcnt.at<uchar>(i, j)) : 0;
			if (tmpv && dbcnt.at<uchar>(i, j) > FIFO_NUM * 7 / 8 && (tmpd = std::abs(tmpv - backImg.at<UINT16>(i, j)))>thresh_value) {
				tdepImg.at<uchar>(i, j) = 255;
				ddepData.at<UINT16>(i, j) = tmpd;
				ddepImg.at<uchar>(i, j) = tmpd * 255 / 8000;
			}
			depthBufferVec[i*depthWidth + j] = tmpv;
		}
	}
	
	// depthに対応するcolor座標を求める
	ERROR_CHECK(coordinateMapper->MapDepthFrameToColorSpace(depthBufferVec.size(), &depthBufferVec[0], colorSpace.size(), &colorSpace[0]));

	
}

void KinectApp::calParams_d(std::vector<ObjectInfo>& obj, cv::Mat& img, int& x, int& y, int& dir)
{
	cv::Point sum, min, max, p, tmpg, tmpg2c, _sq, _sqo;
	int cnt, ssize[4];
	cv::Mat f = img.clone();
	for (int i = 0; i < depthHeight; i++) {
		for (int j = 0; j < depthWidth; j++) {
			if (f.at<uchar>(i, j)) {
				sum = cv::Point(0, 0); cnt = 0; min = cv::Point(j, i); max = cv::Point(j, i);
				calCoG_d(f, sum, cnt, min, max, cv::Point(j, i));


				_sq = max - min; _sqo = (max + min) * .5;

				if (cnt>THRESH_SIZE_MIN && cnt<THRESH_SIZE_MAX
					&& _sq.x > THRESH_LENGTH_MIN && _sq.x < THRESH_LENGTH_MAX
					&& _sq.y > THRESH_LENGTH_MIN && _sq.y < THRESH_LENGTH_MAX) { // _sq.y / _sq.x等もエッジカットに使えそう

					tmpg = cv::Point(sum.x / cnt, sum.y / cnt);
					tmpg2c = cv::Point(static_cast<int>(colorSpace[tmpg.y*depthWidth + tmpg.x].X + 0.5f), static_cast<int>(colorSpace[tmpg.y*depthWidth + tmpg.x].Y + 0.5f));

					if (tmpg2c.x >= 0 && tmpg2c.y >= 0 && tmpg2c.x < colorWidth && tmpg2c.y < colorHeight) {
						ssize[0] = 0; ssize[1] = 0; ssize[2] = 0; ssize[3] = 0;
						for (int k = 1; k <= _sq.y / 2; k++) {
							for (int l = 1; l <= _sq.x / 2; l++) {
								if (img.at<uchar>(_sqo.y - k, _sqo.x - l)) ssize[0]++;
								if (img.at<uchar>(_sqo.y - k, _sqo.x + l)) ssize[1]++;
								if (img.at<uchar>(_sqo.y + k, _sqo.x + l)) ssize[2]++;
								if (img.at<uchar>(_sqo.y + k, _sqo.x - l)) ssize[3]++;
							}
						}

						// id, g(x,y), g2Color(x,y), dir, h, color(hsv), sq(x,y), sqo(x,y), size, ssize[4]
						ObjectInfo tmpobj = {
							-1,
							tmpg,
							tmpg2c,
							-1,
							ddepData.at<UINT16>(tmpg),
							hsvImg.at<cv::Vec3b>(tmpg2c),
							_sq,
							_sqo,
							cnt,
							{ ssize[0], ssize[1], ssize[2], ssize[3] }
						};

						if (objRecogFunc(tmpobj)) {
							obj.push_back(tmpobj);
							objcnt++;
						}
					}
				}
			}
		}
	}
	

	
}

void KinectApp::calCoG_d(cv::Mat& f, cv::Point& sum, int& cnt, cv::Point& min, cv::Point& max, cv::Point p)
{
	if (f.at<uchar>(p)) {
		sum += p; cnt++;
		f.at<uchar>(p) = 0;
		if (p.x<min.x) min.x = p.x;
		if (p.x>max.x) max.x = p.x;
		if (p.y<min.y) min.y = p.y;
		if (p.y>max.y) max.y = p.y;

		if (p.x - 1 >= 0) calCoG_d(f, sum, cnt, min, max, cv::Point(p.x - 1, p.y));
		if (p.x + 1 < depthWidth) calCoG_d(f, sum, cnt, min, max, cv::Point(p.x + 1, p.y));
		if (p.y - 1 >= 0) calCoG_d(f, sum, cnt, min, max, cv::Point(p.x, p.y - 1));
		if (p.y + 1 < depthHeight) calCoG_d(f, sum, cnt, min, max, cv::Point(p.x, p.y + 1));
	}
}
