#ifndef ObjReader_H
#define ObjReader_H
#include<string.h>
#include"global.h"
#include"Trim.h"
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
struct FaceInfo//����Ϣ 
{
   int V[16];//�������� 
   int Vn[16];//��������
   int Vt[16];//��������
   int GroupIndex;//����Ϣ
   int VNumber;//��¼ÿ�����õ��Ķ������
};
struct GroupInfo//����Ϣ 
{
   char GroupName[32];//���� 
   int MtlIndex;//�������� 
};
struct MtlInfo//������Ϣ
{
  bool Is_Ka,Is_Kd,Is_Ks,Is_illum,Is_d,Is_Ns,Is_Ke;//���� 
  char MtlName[64];//�������� 
  float Ka[4];   //�������� 
  float Kd[4];  //������ 
  float Ks[4]; //���淴��
  float Ke[4];//�Է���
  int illum;  //���ʵĹ���ģ�� 
  float d;    //����ָ�� 
  float Ns;  //����ָ�� 
};
/*******��ȡObj�ļ���************** 
 ****���ܣ������ȡobjģ�Ͳ����� **
 ***********************************/ 
class ObjReader
{
     public:
            ObjReader();//��ʼ�� 
            void ReadObjFile(char *FilePath);//��ȡobj�ļ������� 
            void AnalyseMtl(); //��ȡobj�ļ���Ӧ�Ĳ����ļ�
            void DrawModel();//���ݶ�ȡ����Ϣ����ģ�� 
     private:
          bool Is_Vt;//��־�Ƿ�����ͼ������Ϣ
          bool Is_Vn;//��־�Ƿ��ж��㷨����Ϣ
          char MtlName[MtlFileLength];//�洢�����ļ�·�� 
          int VNumber;//��ȡ����ģ�͵ļ����嶥�������
          int VnNumber;//��ȡ����ģ�͵ķ�������������
          int VtNumber;//��ȡ����ģ�͵��������������
          int MtlNumber;//����������� 
          int GroupNumber;//�����
          GroupInfo Group[MaxGroupNumber];
          MtlInfo Mtl[MaxMtlNumber];//������Ϣ 
          int FaceNumber;//��ȡ����ģ�͵��������
          float VSet [MaxPointNumber][3]; //�洢������Ϣ����ά��
          float VnSet[MaxPointNumber][3];//�洢����������Ϣ 
          float VtSet[MaxPointNumber][3];//�洢����������Ϣ
          FaceInfo FaceSet[MaxFaceNumber];//�洢��ȡ��������Ϣ 
          float ViewMinX,ViewMinY,ViewMinZ,ViewMaxX,ViewMaxY,ViewMaxZ;//��¼ģ�͵����귶Χ 
          int VFace[MaxPointNumber][MaxCommonFace];//��¼������Ӧ������Ϣ
};
#endif