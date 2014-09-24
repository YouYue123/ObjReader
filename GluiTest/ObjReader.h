#ifndef ObjReader_H
#define ObjReader_H
#include<string.h>
#include"global.h"
#include"Trim.h"
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
struct FaceInfo//面信息 
{
   int V[16];//顶点索引 
   int Vn[16];//法线索引
   int Vt[16];//纹理索引
   int GroupIndex;//组信息
   int VNumber;//记录每个面用到的顶点个数
};
struct GroupInfo//组信息 
{
   char GroupName[32];//组名 
   int MtlIndex;//材质索引 
};
struct MtlInfo//材质信息
{
  bool Is_Ka,Is_Kd,Is_Ks,Is_illum,Is_d,Is_Ns,Is_Ke;//开关 
  char MtlName[64];//材质名称 
  float Ka[4];   //环境反射 
  float Kd[4];  //漫反射 
  float Ks[4]; //镜面反射
  float Ke[4];//自发光
  int illum;  //材质的光照模型 
  float d;    //渐隐指数 
  float Ns;  //反射指数 
};
/*******读取Obj文件类************** 
 ****功能：负责读取obj模型并画出 **
 ***********************************/ 
class ObjReader
{
     public:
            ObjReader();//初始化 
            void ReadObjFile(char *FilePath);//读取obj文件并处理 
            void AnalyseMtl(); //读取obj文件对应的材质文件
            void DrawModel();//根据读取的信息画出模型 
     private:
          bool Is_Vt;//标志是否含有贴图顶点信息
          bool Is_Vn;//标志是否含有顶点法线信息
          char MtlName[MtlFileLength];//存储材质文件路径 
          int VNumber;//读取到的模型的几何体顶点的数量
          int VnNumber;//读取到的模型的法线向量的数量
          int VtNumber;//读取到的模型的纹理坐标的数量
          int MtlNumber;//材质种类个数 
          int GroupNumber;//组个数
          GroupInfo Group[MaxGroupNumber];
          MtlInfo Mtl[MaxMtlNumber];//材质信息 
          int FaceNumber;//读取到的模型的面的数量
          float VSet [MaxPointNumber][3]; //存储顶点信息（三维）
          float VnSet[MaxPointNumber][3];//存储法线向量信息 
          float VtSet[MaxPointNumber][3];//存储纹理坐标信息
          FaceInfo FaceSet[MaxFaceNumber];//存储读取到的面信息 
          float ViewMinX,ViewMinY,ViewMinZ,ViewMaxX,ViewMaxY,ViewMaxZ;//记录模型的坐标范围 
          int VFace[MaxPointNumber][MaxCommonFace];//记录点所对应的面信息
};
#endif