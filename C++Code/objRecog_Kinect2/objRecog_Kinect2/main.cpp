#include "Header.h"

KinectApp app;

// 0~3キーで時平滑化のフレーム数が指定できる、という機能は廃止
// ノイズ除去処理を削減した結果，時平滑化は避けられなかった

void main()
{
	try {	
		app.initialize();
		// FIFO_NUMフィルタ数，深度エラー閾値，物体面積範囲，矩形の辺範囲
		//app.setEnvParams(64, 35, 5, 100000, 5, 10000);
		// 物体ID，高さ範囲，色相範囲，彩度範囲，明度範囲，矩形辺範囲，物体面積範囲，追加パラメータ
		//app.setObjParams(KinectApp::BANANA, 0, 400, 25, 35, 180, 255, 180, 255, 0, 1000, 0, 2000, 0.f, 0.f, 0.f, 0.f);
		app.objRecog();
	}
	catch (std::exception& ex){
		std::cout << ex.what() << std::endl;
	}
}