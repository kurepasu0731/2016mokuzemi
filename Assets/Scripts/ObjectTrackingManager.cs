using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;


public class ObjectTrackingManager : MonoBehaviour {

    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr openKinectApp();
    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void initialize(IntPtr kinectApp);
    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern bool calParams(IntPtr kinectApp, int id, int[] position);
    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void update(IntPtr kinectApp);
    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void draw(IntPtr kinectApp);
    [DllImport("objRecog_Kinect_DLL", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    private static extern void destroyAllWindows();

    private IntPtr kinectApp; //ネイティブクラスへのポインタ


    public int[] id_1 = new int[3];
    public int[] id_2 = new int[3];
    public int[] id_3 = new int[3];

	// Use this for initialization
	void Start () {
        kinectApp = openKinectApp();
        initialize(kinectApp);
	
	}
	
	// Update is called once per frame
	void Update () {
        update(kinectApp);

        calParams(kinectApp, 0, id_1);
        calParams(kinectApp, 1, id_2);
        calParams(kinectApp, 2, id_3);

        draw(kinectApp);
	}

    //終了処理
    void OnApplicationQuit()
    {
        destroyAllWindows();
    }
}
