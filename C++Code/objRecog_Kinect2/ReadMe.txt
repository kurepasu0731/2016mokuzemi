藤沢先輩へ
main.cppにKinectAppクラスにおける必要な操作が記述されています．
各パラメータがどこで使われているか確認したい場合は，KinectApp.hを覗いてください






物体特定関数作成もしくは微調整を試みる人へ
・parameter.ini
各パラメータの吟味及び追加

・kinectApp.h
ObjectInfo, ObjectRecValに必要なパラメータ吟味
setEnvParams(), setObjParams()で弄れるパラメータ吟味

・kinectApp_ini.cpp
EnvParamsにあたる部分の初期化

・ObjRecog.cpp
物体の種類特定及び向き特定のための関数を記述すること

・kinectApp_upd.cpp
ObjRecog.cppでは補えない問題が発生する場合はこのソースの条件を弄る必要がある

・main.cpp
setEnvParams(), setObjParams()を弄ったら当然ここも弄る必要がある