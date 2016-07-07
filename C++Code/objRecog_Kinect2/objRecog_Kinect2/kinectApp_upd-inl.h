inline void KinectApp::updateColorFrame() {
	CComPtr<IColorFrame> colorFrame;

	HRESULT ret = colorFrameReader->AcquireLatestFrame(&colorFrame);
	if (FAILED(ret)) return;

	// データを取得する
	ERROR_CHECK(colorFrame->CopyConvertedFrameDataToArray(cbufferSize, reinterpret_cast<BYTE*>(colorBuffer.data), COLOR_FORMAT));
	//separateColors();
	mkHSVandSeparateBanana();
}

inline void KinectApp::updateDepthFrame() {
	CComPtr<IDepthFrame> depthFrame;
	int i, j, k;

	HRESULT ret = depthFrameReader->AcquireLatestFrame(&depthFrame);
	if (FAILED(ret)) return;

	// データを取得する
	ERROR_CHECK(depthFrame->AccessUnderlyingBuffer(&dbufferSize, reinterpret_cast<UINT16**>(&depthBuffer.data)));

	k = 0;
	
	depthBuffer.convertTo(depthImg, CV_8UC1, -255. / 8000., 255.);

	for (i = 0; i < depthHeight; i++) {
		for (j = 0; j < depthWidth; j++) {
			if (depthBuffer.at<UINT16>(i, j)) {
				dbcnt.at<uchar>(i, j)++;
				dbsum.at<int>(i, j) += (int)(depthBuffer.at<UINT16>(i, j));
			}
		}
	}
	ndb++;
	dbque.push(depthBuffer.clone());
	
	if (ndb >= FIFO_NUM) {
		sflag = true;
		for (i = 0; i < depthHeight; i++) {
			for (j = 0; j < depthWidth; j++) {
				if ((dbque.front()).at<UINT16>(i, j)) {
					dbcnt.at<uchar>(i, j)--;
					dbsum.at<int>(i, j) -= (int)((dbque.front()).at<UINT16>(i, j));
				}
			}
		}
		dbque.pop();
		ndb--;
	}
	if (sflag) depth_thresh(DEPTH_THRESH);
}