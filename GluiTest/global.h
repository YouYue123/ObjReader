#ifndef GLOBAL_H
#define GLOBAL_H
#include<GL/glut.h>
#include<gl/glui.h>
/******���ò���*************/
#define MaxPointNumber 32768//������ɵ����
#define MaxFaceNumber 32768//������������
#define MaxMtlNumber 1024//������ɲ��ʸ���
#define MaxGroupNumber 128//������������
#define MtlFileLength 256//�����ļ�����󳤶�
#define PI 3.14159265359
#define MoveSpeed 1.1//����ƶ��ٶ�
#define SpinSpeed PI/18//��ת�ٶ�
#define ScaleSpeed 0.05//�����ٶ�
#define MaxCommonFace 16
#define checkImageWidth 64
#define checkImageHeight 64
/********** UI�ؼ����õ���ID ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
/********* UI���õ��Ŀؼ�ָ��*******/
static GLUI *glui, *glui2;
static GLUI_Spinner    *light0_spinner, *light1_spinner;
static GLUI_RadioGroup *radio;
static GLUI_Panel      *obj_panel;
static GLUI_FileBrowser *fb,*ft;
static GLUI_EditText *camera_info[9];
static GLUI_EditText *obj_info[9];
static GLUI_Listbox *list;
/********** ȫ�ֵƹ���Ϣ **********/
static GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };//�����ⷴ�� 	
static GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	//������
static GLfloat LightSpecular[] = {1.0f,1.0f,1.0f,1.0f};//���淴��
static GLfloat LightPosition[]= { 0.0f, 0.0f, 1000.0f, 1.0f };//�ƹ�λ��

static GLfloat Light1Ambient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 	
static GLfloat Light1Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat Light1Specular[] = {1.0f,1.0f,1.0f,1.0f};
static GLfloat Light1Position[] = { 0.0f, 0.0f, -1000.0f, 1.0f };
/********�����Ϣȫ�ֱ���********/
static GLboolean MouseLDown = GL_FALSE;
static GLboolean MouseRDown = GL_FALSE;
static GLboolean MouseMDown = GL_FALSE;
static GLint mousex = 0, mousey = 0;//���λ��
static int LastP[2];//��¼���֮ǰ��λ��
/********���λ����Ϣ*************/
static GLfloat center[3] = {0.0f, 0.0f, 0.0f};/// ����ĵ� 
static GLfloat eye[3];//λ�� 
static GLfloat HeadDirection[3]={0.0f,1.0f,0.0f};//ͷ������
/********��������Ϣ*************/
static GLfloat theta = 0.5 * PI;
static GLfloat phi = 0.0;
static GLfloat r = 100.0;
/********����任��Ϣ**********/
static GLfloat ScaleX=1.0f,ScaleY=1.0f,ScaleZ=1.0f;//���ű任
static float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };//��ת�任
static float obj_pos[] = { 0.0, 0.0, 0.0 };//λ�ñ任
/**********�������ֲ���*****************/
static GLfloat BrassA[] = {0.329412f, 0.223529f, 0.027451f, 1.000000f};
static GLfloat BrassD[] = {0.780392f, 0.568627f, 0.113725f, 1.000000f};
static GLfloat BrassS[] = {0.992157f, 0.941176f, 0.807843f, 1.000000f};
static GLfloat BrassNs = 27.897400f;


static GLfloat GoldA[] = {0.247250f, 0.199500f, 0.074500f, 1.000000f};
static GLfloat GoldD[] = {0.751640f, 0.606480f, 0.226480f, 1.000000f};
static GLfloat GoldS[] = {0.628281f, 0.555802f, 0.366065f, 1.000000f};
static GLfloat GoldNs = 51.200001f;

static GLfloat EmeraldA[] = {0.021500f, 0.174500f, 0.021500f, 0.550000f};
static GLfloat EmeraldD[] = {0.075680f, 0.614240f, 0.075680f, 0.550000f};
static GLfloat EmeraldS[] = {0.633000f, 0.727811f, 0.633000f, 0.550000f};
static GLfloat EmeraldNs = 76.800003f;

static GLfloat VioletA[] = {0.110000f, 0.060000f, 0.090000f, 1.000000f};
static GLfloat VioletD[] = {0.430000f, 0.470000f, 0.540000f, 1.000000f};
static GLfloat VioletS[] = {0.330000f, 0.330000f, 0.520000f, 1.000000f};
static GLfloat VioletNs = 22.0000f;
/********������Ϣ*****/
static int   width = 1366,height = 768;//��ʼ���ڳ���
static float xy_aspect;//���ڱ���
static int main_window;//������ID
static int window_x = (glutGet (GLUT_SCREEN_WIDTH) - width)/2;//��������
static int window_y = (glutGet (GLUT_SCREEN_HEIGHT) - height)/2;//��������
/********UI���õ��Ŀ�����******/
static int   FillMode = 0;//���ģʽ����
static int   selfMaterial = 0;//������ʿ���
static int   normal = 0;//���߼��㿪��
static int   texture = 0;
static int   light0_enabled = 1;//��Դ����
static int   light1_enabled = 1;
static float light0_intensity = 1.0;//��Դǿ��
static float light1_intensity = 1.0;
static float scale = 1.0;//���ű���
static float smooth = 1.0;//ƽ���̶�
static int UseOtherMtl = 0;//ʹ��ϵͳģ�Ϳ���
static char FileName[128];//�ļ���
static int shade_type=1;
/***************Texture**************/
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;
#endif