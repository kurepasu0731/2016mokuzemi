#pragma once

#include <iostream>

// openCV
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include <direct.h> // create dir

#include <Kinect.h>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp> // SURF

#include <Windows.h>
//#include <thread>
//#include <mutex>

#include <atlbase.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define COLOR_FORMAT ColorImageFormat::ColorImageFormat_Bgra

// flagで管理することにする
// 32: 32f, 16: 16f, 8: 8f, 0: 1f
//#define MEDIAN_DEPTH 0

#define DIR_CIRCLE 20

// 以下，色マーカー識別用パラメータ
// number, hueRange(0~179), saturation(0~255), value(0~255)の順 
#define NUM_COLOR 3 //手動で書き直すこと

#define _GAUSSIAN 1

#define _GOMA 1
#define GOMA0 5 // 削る
#define GOMA1 4 // 膨張

#define MIN_BLOCK 250


// setHSVparamsに手動で追加すること・・・

// BLUE
#define BLUE 0
#define BLUE_H_MIN 110
#define BLUE_H_MAX 120
#define BLUE_S_MIN 205
#define BLUE_S_MAX 0
#define BLUE_V_MIN 0
#define BLUE_V_MAX 0

// GREEN
#define GREEN 1
#define GREEN_H_MIN 60
#define GREEN_H_MAX 80
#define GREEN_S_MIN 205
#define GREEN_S_MAX 0
#define GREEN_V_MIN 0
#define GREEN_V_MAX 0

// RED
#define RED 2
#define RED_H_MIN 170
#define RED_H_MAX 10
#define RED_S_MIN 160
#define RED_S_MAX 0
#define RED_V_MIN 0
#define RED_V_MAX 0

#define _MKMASKIMG 0

#include "kinectApp.h"
