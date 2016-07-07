/*
inline void KinectApp::updateDepthFrame() {
	CComPtr<IDepthFrame> depthFrame;

	HRESULT ret = depthFrameReader->AcquireLatestFrame(&depthFrame);
	if (FAILED(ret)) return;

	// データを取得する
	ERROR_CHECK(depthFrame->AccessUnderlyingBuffer(&dbufferSize, reinterpret_cast<UINT16**>(&depthBuffer.data)));
}
*/

inline void KinectApp::updateColorFrame() {
	CComPtr<IColorFrame> colorFrame;

	HRESULT ret = colorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(ret)) return;

	// データを取得する
	ERROR_CHECK(colorFrame->CopyConvertedFrameDataToArray(cbufferSize, reinterpret_cast<BYTE*>(colorBuffer.data), COLOR_FORMAT));
	separateColors();
}

inline void KinectApp::separateColors() {
	cv::cvtColor(colorBuffer, hsvImg, CV_BGR2HSV);
#if _MKMASKIMG
	cv::imwrite("../hsvdata.bmp", hsvImg);
#endif
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
						sum += maskImg[i].at<uchar>(h, w) = ((p[1]>hsv_thresh[i][2]?p[1]:0) * p[2]) / 255;
						cnt++;
					}
				}
			}
		} else {
			for (int h = 0; h < colorHeight; h++) {
				for (int w = 0; w < colorWidth; w++) {
					p = hsvImg.at<cv::Vec3b>(h, w);
					if (hsv_thresh[i][0] <= p[0] || hsv_thresh[i][1] >= p[0]) {
						sum += maskImg[i].at<uchar>(h, w) = ((p[1]>hsv_thresh[i][2]?p[1]:0) * p[2]) / 255;
						cnt++;
					}
				}
			}
		}
		sum = cnt ? sum / cnt : 0;
#if _GAUSSIAN
		cv::GaussianBlur(maskImg[i], maskImg[i], cv::Size(7, 7), 10, 10);
		//cv::GaussianBlur(maskImg[i], maskImg[i], cv::Size(5, 5), 2, 2);
#endif
		cv::threshold(maskImg[i], maskImg[i], sum*2, 255, CV_THRESH_BINARY);

#if _GOMA
		// erode先：白ゴマ潰し，dilate先：黒ゴマ潰し
		erode(maskImg[i], maskImg[i], cv::Mat(), cv::Point(-1, -1), GOMA0);  // 収縮
		dilate(maskImg[i], maskImg[i], cv::Mat(), cv::Point(-1, -1), GOMA1); // 膨張
#endif

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
		
		
#if _MKMASKIMG
		std::stringstream s;
		s << "../test" << i << ".bmp";
		cv::imwrite(s.str(), maskImg[i]);
		s << std::endl;
#endif
	}
}
