using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

public class ObjectTrackingManger2 : MonoBehaviour
{

    //オブジェクトの情報をxmlから読み取る版

    //Public Variables
    public string dirname = "ObjectRecognize";    //Output Directory
    public string fname = "objRecog.xml"; //file name

    //Private Variables
    private XmlDocument xmlDoc;

    public int objNum = 0; //認識された物体の個数

    //IDわりふり
    public GameObject ID_0;
    public GameObject ID_1;
    public GameObject ID_2;
    public GameObject ID_3;
    public GameObject ID_4;
    public GameObject ID_5;

    //それぞれの位置、角度
    public Vector2[] Positions;
    public int[] Angles;

    public bool isReadFile = false;

    //Public Functions

    // Initialization
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {


    }

    void OnGUI()
    {
        if (GUI.Button(new Rect(20, 20, 120, 50), "Read ObjectFile"))
        {
            xmlDoc = ReadXML();
            ParseXML(xmlDoc);
            TransformObject();
        }
    }


    private XmlDocument ReadXML()   //XMLファイルを読み込む
    {
        //
        string dlm = "/";
        string path;
        if (dirname != "")
        {
            path = dlm + dirname + dlm;
        }
        else
        {
            path = dlm;
        }
        string fullpath;
        if (Application.isEditor)
        {
            fullpath = Application.dataPath + path + fname; //エディタの場合, Application.dataPathは'Asset'フォルダ
        }
        else
        {
            fullpath = Application.dataPath + dlm + ".." + path + fname;    //PC/Macの場合,Application.dataPathは、'実行ファイル_data'フォルダ

        }
        XmlDocument xmlDoc = new XmlDocument(); //XmlDocumentクラス
        xmlDoc.Load(fullpath); // load the file.    //XMLデータをロード

        return xmlDoc;  //読み込んだXmlDocumentを返す
    }



    private void ParseXML(XmlDocument xmlDoc)
    {
        Vector2 pos= new Vector2(); //物体の座標を格納
        int angle = 0; //角度

        foreach (XmlElement element in xmlDoc.DocumentElement)
        {
            //string text = element.InnerText;                         // 要素の内容
            //string attribute = element.GetAttribute("numObjects");  // 指定した属性
            string log = element.Name + ": " + element.InnerText;
            Debug.Log(log);

            if (element.Name.Equals("numObjects"))
            {
                objNum = int.Parse(element.InnerText);
                Positions = new Vector2[objNum];
                Angles = new int[objNum];
                continue;
            }
            

            //ID番号ごとにみる
            if (element.Name.Contains("0"))
            {
                switch (element.Name)
                {
                    case "id0": continue;
                    case "x0":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y0":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir0":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[0] = pos;
                Angles[0] = angle;
            }

            if (element.Name.Contains("1"))
            {
                switch (element.Name)
                {
                    case "id1": continue;
                    case "x1":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y1":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir1":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[1] = pos;
                Angles[1] = angle;
            }

            if (element.Name.Contains("2"))
            {
                switch (element.Name)
                {
                    case "id2": continue;
                    case "x2":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y2":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir2":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[2] = pos;
                Angles[2] = angle;
            }

            if (element.Name.Contains("3"))
            {
                switch (element.Name)
                {
                    case "id3": continue;
                    case "x3":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y3":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir3":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[3] = pos;
                Angles[3] = angle;
            }

            if (element.Name.Contains("4"))
            {
                switch (element.Name)
                {
                    case "id4": continue;
                    case "x4":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y4":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir4":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[4] = pos;
                Angles[4] = angle;
            }

            if (element.Name.Contains("5"))
            {
                switch (element.Name)
                {
                    case "id5": continue;
                    case "x5":
                        {
                            pos.x = int.Parse(element.InnerText);
                            continue;
                        }
                    case "y5":
                        {
                            pos.y = int.Parse(element.InnerText);
                            continue;
                        }
                    case "dir5":
                        {
                            int vec = int.Parse(element.InnerText);
                            //vec->angleへ変換
                            angle = vec;
                            break;
                        }
                    default: continue;
                }

                Positions[5] = pos;
                Angles[5] = angle;
            }

        }
    }

    private void TransformObject()
    {
        //Position(zはそのまま)
        ID_0.transform.position = new Vector3(Positions[0].x, Positions[0].y, ID_0.transform.position.z);
        ID_1.transform.position = new Vector3(Positions[1].x, Positions[1].y, ID_1.transform.position.z);
        ID_2.transform.position = new Vector3(Positions[2].x, Positions[2].y, ID_2.transform.position.z);
        ID_3.transform.position = new Vector3(Positions[3].x, Positions[3].y, ID_3.transform.position.z);
        ID_4.transform.position = new Vector3(Positions[4].x, Positions[4].y, ID_4.transform.position.z);
        ID_5.transform.position = new Vector3(Positions[5].x, Positions[5].y, ID_5.transform.position.z);
        //Angle
    }
}
