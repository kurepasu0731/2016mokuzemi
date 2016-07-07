#include "KinectApp.h"

// ゴリラのように・・・書こう！
void KinectApp::setHSVparams() {
	// BLUE
	hsv_thresh[BLUE][0] = BLUE_H_MIN;
	hsv_thresh[BLUE][1] = BLUE_H_MAX;
	hsv_thresh[BLUE][2] = BLUE_S_MIN;
	hsv_thresh[BLUE][3] = BLUE_S_MAX;
	hsv_thresh[BLUE][4] = BLUE_V_MIN;
	hsv_thresh[BLUE][5] = BLUE_V_MAX;

	// GREEN
	hsv_thresh[GREEN][0] = GREEN_H_MIN;
	hsv_thresh[GREEN][1] = GREEN_H_MAX;
	hsv_thresh[GREEN][2] = GREEN_S_MIN;
	hsv_thresh[GREEN][3] = GREEN_S_MAX;
	hsv_thresh[GREEN][4] = GREEN_V_MIN;
	hsv_thresh[GREEN][5] = GREEN_V_MAX;

	// RED
	hsv_thresh[RED][0] = RED_H_MIN;
	hsv_thresh[RED][1] = RED_H_MAX;
	hsv_thresh[RED][2] = RED_S_MIN;
	hsv_thresh[RED][3] = RED_S_MAX;
	hsv_thresh[RED][4] = RED_V_MIN;
	hsv_thresh[RED][5] = RED_V_MAX;
}