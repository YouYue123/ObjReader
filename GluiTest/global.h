#ifndef GLOBAL_H
#define GLOBAL_H
#include<GL/glut.h>
#include<gl/glui.h>
/******设置参数*************/
#define MaxPointNumber 32768//最大容纳点个数
#define MaxFaceNumber 32768//最大容纳面个数
#define MaxMtlNumber 1024//最大容纳材质个数
#define MaxGroupNumber 128//最大容纳组个数
#define MtlFileLength 256//材质文件名最大长度
#define PI 3.14159265359
#define MoveSpeed 1.1//相机移动速度
#define SpinSpeed PI/18//旋转速度
#define ScaleSpeed 0.05//缩放速度
#define MaxCommonFace 16
#define checkImageWidth 64
#define checkImageHeight 64
/********** UI控件所用到的ID ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
/********* UI所用到的控件指针*******/
static GLUI *glui, *glui2;
static GLUI_Spinner    *light0_spinner, *light1_spinner;
static GLUI_RadioGroup *radio;
static GLUI_Panel      *obj_panel;
static GLUI_FileBrowser *fb,*ft;
static GLUI_EditText *camera_info[9];
static GLUI_EditText *obj_info[9];
static GLUI_Listbox *list;
/********** 全局灯光信息 **********/
static GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };//环境光反射 	
static GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	//漫反射
static GLfloat LightSpecular[] = {1.0f,1.0f,1.0f,1.0f};//镜面反射
static GLfloat LightPosition[]= { 0.0f, 0.0f, 1000.0f, 1.0f };//灯光位置

static GLfloat Light1Ambient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 	
static GLfloat Light1Diffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat Light1Specular[] = {1.0f,1.0f,1.0f,1.0f};
static GLfloat Light1Position[] = { 0.0f, 0.0f, -1000.0f, 1.0f };
/********鼠标信息全局变量********/
static GLboolean MouseLDown = GL_FALSE;
static GLboolean MouseRDown = GL_FALSE;
static GLboolean MouseMDown = GL_FALSE;
static GLint mousex = 0, mousey = 0;//鼠标位置
static int LastP[2];//记录鼠标之前的位置
/********相机位置信息*************/
static GLfloat center[3] = {0.0f, 0.0f, 0.0f};/// 看向的点 
static GLfloat eye[3];//位置 
static GLfloat HeadDirection[3]={0.0f,1.0f,0.0f};//头顶朝向
/********球坐标信息*************/
static GLfloat theta = 0.5 * PI;
static GLfloat phi = 0.0;
static GLfloat r = 100.0;
/********物体变换信息**********/
static GLfloat ScaleX=1.0f,ScaleY=1.0f,ScaleZ=1.0f;//缩放变换
static float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };//旋转变换
static float obj_pos[] = { 0.0, 0.0, 0.0 };//位置变换
/**********定义四种材质*****************/
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
/********窗口信息*****/
static int   width = 1366,height = 768;//初始窗口长宽
static float xy_aspect;//窗口比例
static int main_window;//主窗口ID
static int window_x = (glutGet (GLUT_SCREEN_WIDTH) - width)/2;//窗口中心
static int window_y = (glutGet (GLUT_SCREEN_HEIGHT) - height)/2;//窗口中心
/********UI所用到的开关量******/
static int   FillMode = 0;//填充模式开关
static int   selfMaterial = 0;//自身材质开关
static int   normal = 0;//法线计算开关
static int   texture = 0;
static int   light0_enabled = 1;//光源开关
static int   light1_enabled = 1;
static float light0_intensity = 1.0;//光源强度
static float light1_intensity = 1.0;
static float scale = 1.0;//缩放比例
static float smooth = 1.0;//平滑程度
static int UseOtherMtl = 0;//使用系统模型开关
static char FileName[128];//文件名
static int shade_type=1;
/***************Texture**************/
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;
#endif