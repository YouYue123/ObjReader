#include"Trim.h"
#include"global.h"
#include"Debug.h"
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
using namespace std;
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
            ObjReader()//��ʼ�� 
            {
				 int i;
                 Is_Vt = false;
                 Is_Vn = false;
                 memset(MtlName,0,sizeof(MtlName));
                 memset(Mtl,0,sizeof(Mtl));
                 memset(Group,0,sizeof(Group));
				 for(i=0;i<MaxMtlNumber;i++)
				 {
					 Mtl[i].Ka[3] = 1.0f;
					 Mtl[i].Kd[3] = 1.0f;
					 Mtl[i].Ks[3] = 1.0f;
				 }
                 VNumber = 0;
                 VnNumber = 0;
                 VtNumber = 0;
                 FaceNumber = 0;
                 MtlNumber = 0;
                 GroupNumber = 0;
                 memset(VSet,0,sizeof(VSet));
                 memset(VnSet,0,sizeof(VnSet));
                 memset(VtSet,0,sizeof(VtSet));
                 memset(FaceSet,0,sizeof(FaceSet));
				 memset(VFace,0,sizeof(VFace));
				 memset(VFaceAccount,0,sizeof(VFaceAccount));
                 ViewMinX = 65535;
                 ViewMinY = 65535;
                 ViewMinZ = 65535;
                 ViewMaxX = -65535;
                 ViewMaxY = -65535;
                 ViewMaxZ = -65535;
            }
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
		 int VFaceAccount[MaxPointNumber];
};
ObjReader *Reader1;
void control_cb( int control)
{
  if(control == 4)
  {
	  switch(radio->get_int_val())
	  {
	  case 0:
		  glShadeModel(GL_FLAT);//����Shade�ķ�ʽGL_SMOOTH����GL_FLAT;
		  break;
	  case 1:
		  glShadeModel(GL_SMOOTH);//����Shade�ķ�ʽGL_SMOOTH����GL_FLAT
		  break;
	  }
	  glutPostRedisplay();
	  return;
  }
  if(control == 7)
  {
	  if(!strstr(fb->get_file(),".obj"))
	{
		printf("File Type Error!\n");
		return;
	}
	else
	{
	   strcpy(FileName,fb->get_file());
	   if(Reader1)
		  delete(Reader1);
	    Reader1 = new ObjReader;	
        Reader1->ReadObjFile(FileName);
        Reader1->AnalyseMtl();
		glutPostRedisplay();
		 return;
	}
  }
  if(control == 8)
  {
	     ScaleX = scale;
         ScaleY = scale;
         ScaleZ = scale;
         glutPostRedisplay();
		 return;
  }
  if(control == 9)
  {
	   glutPostRedisplay();
		 return;
  }
  if(control == 10)
  {      
	     if(selfMaterial ==1)
			 return;
		 UseOtherMtl = list->get_int_val();
	     glutPostRedisplay();
		 return;  
  }
  if(control==16)
  {
	    glutPostRedisplay();
		return;
  }
  if(control==21)
  {
	  glutPostRedisplay();
	  return;
  }
  if ( control == LIGHT0_ENABLED_ID ) 
  {
    if ( light0_enabled ) 
	{
      glEnable( GL_LIGHT0 );
      light0_spinner->enable();
    }
    else 
	{
      glDisable( GL_LIGHT0 ); 
      light0_spinner->disable();
    }
  }
  else if ( control == LIGHT1_ENABLED_ID ) 
  {
    if ( light1_enabled ) 
	{
      glEnable( GL_LIGHT1 );
      light1_spinner->enable();
    }
    else 
	{
      glDisable( GL_LIGHT1 ); 
      light1_spinner->disable();
    }
  }
  else if ( control == LIGHT0_INTENSITY_ID ) 
  {
    float v[] = 
	{ 
      LightDiffuse[0],  LightDiffuse[1],
      LightDiffuse[2],  LightDiffuse[3] 
	};
    
    v[0] *= light0_intensity;
    v[1] *= light0_intensity;
    v[2] *= light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v );
  }
  else if ( control == LIGHT1_INTENSITY_ID ) 
  {
    float v[] = 
	{ 
      Light1Diffuse[0],   Light1Diffuse[1],
       Light1Diffuse[2],   Light1Diffuse[3] 
	};
    
    v[0] *= light1_intensity;
    v[1] *= light1_intensity;
    v[2] *= light1_intensity;

    glLightfv(GL_LIGHT1, GL_DIFFUSE, v );
  }
  glutPostRedisplay();
  return;
}
void draw_axes()
{
  glDisable( GL_LIGHTING );

  glPushMatrix();
  //glScalef( scale, scale, scale );

  glBegin( GL_LINES );
 
  glColor3f( 1.0, 0.0, 0.0 );
  glVertex3f( 80.0f,5.0f, 0.0 );  glVertex3f( 100.0, 25.0f, 0.0 ); /* Letter X */
  glVertex3f( 80.0f, 25.0f, 0.0 );  glVertex3f( 100.0f, 5.0f, 0.0 );
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 100.0, 0.0, 0.0 ); /* X axis      */

  glColor3f( 0.0, 1.0, 0.0 );
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 100.0, 0.0 ); /* Y axis      */

  glColor3f( 0.0, 0.0, 1.0 );
  glVertex3f( 0.0, 0.0, 0.0 );  glVertex3f( 0.0, 0.0, 100.0 ); /* Z axis    */
  glEnd();

  glPopMatrix();

  glEnable( GL_LIGHTING );
}
/**********obj�ļ���ȡ����************
 *******�������ļ�·��****************
 **���ܣ���Objģ���ļ��ж�ȡ����*****
 *******������Ӧ�Ĵ洢****************/ 
void ObjReader::ReadObjFile(char *FilePath)
{
      int temp,type,i,j,k,TempIndex,TempGroupIndex;
      bool FindMtl,FindGroup;
      char *T;
      char K[128],content[1024],tempMtl[64];
      ifstream F(FilePath);
      if(!F.is_open())
       {
          cout<<"Error in opening ObjFile\n";
          exit(1);
       }
      while(!F.eof())
      {
         memset(content,0,sizeof(content));
         F.getline(content,1024);
         temp = 0;
         type  = 0;
         FindMtl = false; 
         FindGroup = false;
         mytrimF(content);
         mytrimE(content);
         #ifdef DEBUG
          printf("%s\n",content);
         #endif
         switch(content[0])
            {
              case '#' ://ע����Ϣ 
                   break; 
              case 'g' ://����Ϣ
                      if(content[3]=='\0')
                          break;
                       memset(tempMtl,0,sizeof(tempMtl));
                       strtok(content," ");
                       strcpy(tempMtl,strtok(NULL," "));
                       for(i=0;i<GroupNumber;i++)
                        {
                            if(strstr(Group[i].GroupName,tempMtl))//�ҵ�֮ǰ�洢���Ĳ���
                            {
                                 TempGroupIndex = i;
                                 FindGroup = true;
                                 break;
                            }                       
                        }
                        if(!FindGroup)
                        {
                            strcpy(Group[GroupNumber].GroupName,tempMtl);
                            TempGroupIndex = GroupNumber;
                            GroupNumber++;       
                        }
                        break;
              case 'u' ://����Ӧ����Ϣ
                         memset(tempMtl,0,sizeof(tempMtl));
                         strtok(content," ");
                         strcpy(tempMtl,strtok(NULL," "));
                         for(i=0;i<MtlNumber;i++)
                          {
                                if(strstr(Mtl[i].MtlName,tempMtl))//�ҵ�֮ǰ�洢���Ĳ���
                                 {
                                       TempIndex = i;
                                       FindMtl = true;
                                       break;
                                 }         
                          }
                          if(!FindMtl)//���֮ǰû�г��ָò��� 
                           {
                                strcpy(Mtl[MtlNumber].MtlName,tempMtl);
                                TempIndex = MtlNumber;
                                MtlNumber++;
                           }
                          Group[TempGroupIndex].MtlIndex = TempIndex; 
                         break;
              case 'm' ://�����ļ����� 
                     strtok(content," ");
                     strcpy(MtlName,strtok(NULL," "));
                     break;
              case 'v' :
                    if(content[1]==' ')//Ϊ������Ϣ
                    { 
                      strtok(content," ");
                      for(i=0;i<3;i++)
                      {
                          VSet[VNumber][i] = atof(strtok(NULL," "));
                         switch(i)
                          {
                              case 0 :
                                   if(VSet[VNumber][i]<ViewMinX)
                                        ViewMinX =  VSet[VNumber][i];
                                    if(VSet[VNumber][i]>ViewMaxX)
                                        ViewMaxX =  VSet[VNumber][i];
                                    break;
                              case 1 :
                                  if(VSet[VNumber][i]<ViewMinY)
                                        ViewMinY =  VSet[VNumber][i];
                                    if(VSet[VNumber][i]>ViewMaxY)
                                        ViewMaxY =  VSet[VNumber][i];
                                    break;
                              case 2:
                                   if(VSet[VNumber][i]<ViewMinZ)
                                        ViewMinZ =  VSet[VNumber][i];
                                   if(VSet[VNumber][i]>ViewMaxZ)
                                        ViewMaxZ =  VSet[VNumber][i];
                                   break;
                              default:
                                   break;
                          }
                      }
                      VNumber++;
                      break;
                    }
                    else if(content[1]=='t')//��ͼ����� 
                    {
                      Is_Vt = true;
                      strtok(content," ");
                      for(i=0;i<2;i++)
                      {
                         VtSet[VtNumber][i] = atof(strtok(NULL," "));
                      }
                      VtNumber++;
                      break;
                    }
					else if(content[1]=='n')//���㷨����Ϣ 
                    {
                      Is_Vn = true;
                      strtok(content," ");
                      #ifdef TestVnInput
                      printf("��%d��������:",VnNumber+1);
                      #endif
                      for(i=0;i<3;i++)
                      {
                         VnSet[VnNumber][i] = atof(strtok(NULL," "));
                         #ifdef TestVnInput
                         printf("%f ",VnSet[VnNumber][i]);
                         #endif
                      }
                      #ifdef TestVnInput
                      printf("\n");
                      #endif
                      VnNumber++;
                      break; 
                    }
          case 'f' :
               
                  if(!Is_Vn && !Is_Vt)//û�ж��㷨�ߺ���ͼ����� 
                   {
                     for(i=0;i<strlen(content);i++)
                      {
                         if(content[i]==' ')
                               temp++;
                      }
                      FaceSet[FaceNumber].VNumber = temp;
                      FaceSet[FaceNumber].GroupIndex = TempGroupIndex;
                      strtok(content," ");
                      for(i=0;i<temp;i++)
                      {
                        FaceSet[FaceNumber].V[i] = atoi(strtok(NULL," "));
						VFace[FaceSet[FaceNumber].V[i]-1][VFaceAccount[FaceSet[FaceNumber].V[i]-1]++]=FaceNumber;//��¼������Ӧ������Ϣ
                      }
                      FaceNumber++;
                     break;
                   }
                   
                  else//�ж��㷨����Ϣ����ͼ�������Ϣ 
                   {
                     for(i=0;i<strlen(content);i++)
                      {
                         if(content[i]==' ')
                               temp++;
                      }
                      strtok(content," ");
                      FaceSet[FaceNumber].VNumber = temp;
                      FaceSet[FaceNumber].GroupIndex = TempGroupIndex;
                      for(i=0;i<temp;i++) 
                      {
                         T = strtok(NULL," ");//��ȡһ�� 
                         type = 0;
                         k = 0;
                         memset(K,0,sizeof(K));
                         for(j=0;j<strlen(T);j++)//��ͷ��βɨ�� 
                          {
                              if(T[j]=='/'|| j==(strlen(T)-1))
                                 {    
                                    switch(type)
                                    {
                                       case 0:
                                          FaceSet[FaceNumber].V[i] = atoi(K);
										  VFace[FaceSet[FaceNumber].V[i]-1][VFaceAccount[FaceSet[FaceNumber].V[i]-1]++]=FaceNumber;//��¼������Ӧ������Ϣ
                                          k=0;
                                          type++;
                                         
                                          break;
                                       case 1:
                                          FaceSet[FaceNumber].Vt[i] = atoi(K);
                                          k=0;
                                          type++;
                                          break;
                                       case 2:
                                          K[k] = T[j];
                                          FaceSet[FaceNumber].Vn[i] = atoi(K);
                                          k=0;
                                          type++;
                                          break;
                                       default:
                                          break;    
                                    }
                                    memset(K,0,sizeof(K)); 
                                 }
                              else
                               {
                                    K[k] = T[j];
                                    k++;
                               }
                          }
                      }
                      FaceNumber++;
                      break;
                   }
          default :
                   break;
            
            }
      }
      #ifdef DEBUG
      printf("�������:%d\n������������:%d\n��ͼ�������:%d\n",VNumber,VnNumber,VtNumber);
      printf("�����:%d\n",FaceNumber);
      printf("���ʸ���:%d\n",MtlNumber);
      printf("�����:%d\n",GroupNumber);
      #endif
	  
	  float x1,x2,x3,y1,y2,y3,z1,z2,z3,tempx1,tempx2,tempy1,tempy2,tempz1,tempz2,resultx,resulty,resultz,L;
	  if(!Is_Vn)//����ļ��ڲ�����������
	  {
		  for(i=0;i<VNumber;i++)//����ÿ����
		  {
			  for(j=0;j<VFaceAccount[i];j++)//�����е����ڵ���
			  {
				  for(k=0;k<3;k++)//ȡ���������е�������
				  {
					if(k==0)
					{
					 x1 = VSet[FaceSet[VFace[i][j]].V[k]-1][0];
					 y1 = VSet[FaceSet[VFace[i][j]].V[k]-1][1];
					 z1 = VSet[FaceSet[VFace[i][j]].V[k]-1][2];
					//printf("%d:%f %f %f\n",FaceSet[VFace[i][j]].V[k]-1,x1,y1,z1);
					}
					 if(k==1)//�ӵڶ����㿪ʼ
					 {
					   x2 = VSet[FaceSet[VFace[i][j]].V[k]-1][0];
					   y2 = VSet[FaceSet[VFace[i][j]].V[k]-1][1];
					   z2 = VSet[FaceSet[VFace[i][j]].V[k]-1][2];
					   tempx1 = x2 - x1;          //����������� ���������ɵ�����
					   tempy1 = y2 - y1;
					   tempz1 = z2 - z1;
					 }
					 if(k==2)
					 {
					   x3 = VSet[FaceSet[VFace[i][j]].V[k]-1][0];
					   y3 = VSet[FaceSet[VFace[i][j]].V[k]-1][1];
					   z3 = VSet[FaceSet[VFace[i][j]].V[k]-1][2];
					   tempx2 = x3 - x2;
					   tempy2 = y3 - y2;
					   tempz2 = z3 - z2; //��������ε���������
					   resultx = tempy1 * tempz2 - tempz1 * tempy2;//������������
					   resulty = tempz1 * tempx2 - tempx1 * tempz2;
					   resultz = tempx1 * tempy2 - tempy1 * tempx2;
					   L = sqrt(resultx* resultx+ resulty* resulty+ resultz * resultz);//��һ��
					   resultx /= L; resulty /= L;resultz /= L;
					 }
				  }
				     //����ǰ�����η������ӵ��������㷨������
                   VnSet[i][0] += resultx; VnSet[i][1] += resulty; VnSet[i][2] += resultz;
			  }
			       
			       L = sqrt(VnSet[i][0]* VnSet[i][0]+ VnSet[i][1]* VnSet[i][1]+ VnSet[i][2]* VnSet[i][2]);
				   VnSet[i][0] /= L; VnSet[i][1] /= L;VnSet[i][2] /= L;
				   //printf("%f\n", VnSet[i][0]);
		  }
	  }
	 
      F.close();
      return;
}
/***��ͼ����*******/
void ObjReader::DrawModel()
{   
     int i,j;
     char temp[32];
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
     glColor3f(1.0,1.0,1.0);
     glViewport(window_x,window_y,(GLsizei)width,(GLsizei)height); 
     //glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     eye[0] = r*sin(theta)*sin(phi);
     eye[1] = r*cos(theta);
     eye[2] = r*sin(theta)*cos(phi);
     //HeadDirection[0] =  sin(phi);
     HeadDirection[1] = sin(theta);
	 //HeadDirection[2] = 1/sin(phi);
     gluLookAt(eye[0],eye[1],eye[2],
               center[0],center[1],center[2],
               HeadDirection[0],HeadDirection[1],HeadDirection[2]);//��ͼ�任
     #ifdef TestLookAt
     printf("�۾���λ��: %f %f %f\n",eye[0],eye[1],eye[2]);
     printf("�۲������:%f %f %f\n",center[0],center[1],center[2]);
     printf("ͷ���ĳ���:%f %f %f\n",HeadDirection[0],HeadDirection[1],HeadDirection[2]);
     #endif

	 sprintf(temp,"%lf",eye[0]);camera_info[0]->text = temp;camera_info[0]->redraw();
	 sprintf(temp,"%lf",eye[1]);camera_info[1]->text = temp;camera_info[1]->redraw();
	 sprintf(temp,"%lf",eye[2]);camera_info[2]->text = temp;camera_info[2]->redraw();
	 sprintf(temp,"%lf",center[0]);camera_info[3]->text = temp;camera_info[3]->redraw();
	 sprintf(temp,"%lf",center[1]);camera_info[4]->text = temp;camera_info[4]->redraw();
	 sprintf(temp,"%lf",center[2]);camera_info[5]->text = temp;camera_info[5]->redraw();
	 sprintf(temp,"%lf",HeadDirection[0]);camera_info[6]->text = temp;camera_info[6]->redraw();
	 sprintf(temp,"%lf",HeadDirection[1]);camera_info[7]->text = temp;camera_info[7]->redraw();
	 sprintf(temp,"%lf",HeadDirection[2]);camera_info[8]->text = temp;camera_info[8]->redraw();

	 sprintf(temp,"%lf",obj_pos[0]);obj_info[0]->text = temp;obj_info[0]->redraw();
	 sprintf(temp,"%lf",obj_pos[1]);obj_info[1]->text = temp;obj_info[1]->redraw();
	 sprintf(temp,"%lf",-obj_pos[2]);obj_info[2]->text = temp;obj_info[2]->redraw();
     
	 glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(60.0,xy_aspect,0.3,10000.0);
      glMatrixMode(GL_MODELVIEW);
	  draw_axes();
	 if(FillMode==0)
	{
	  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);	
	}
	 else
	 {
	   glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);	
	 }
 for(i=0;i<FaceNumber;i++)
  { 
     glPushMatrix();
     //glTranslated(MoveX,MoveY,MoveZ);
	 glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
     glScaled(ScaleX,ScaleY,ScaleZ);
	 glMultMatrixf( view_rotate );
#ifdef DEBUG
     printf("��%d����:\nKa%d---%f %f %f\n",i+1,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ka,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ka[0]
                                ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ka[1]
                                ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ka[2]);
     printf("Kd%d-----%f %f %f\n",Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Kd,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Kd[0]
                               ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Kd[1]
                               ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Kd[2]);
     printf("Ks%d-----%f %f %f\n",Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ks,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ks[0]
                               ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ks[1]
                               ,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ks[2]);
      printf("Ns%d-----%f\n",Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ns,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ns);
#endif
    if(selfMaterial)
	{
	     
	 glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,BrassNs);//�߹�(���Ĳ�֪��Ϊʲô����������򲻶�)
     if(Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ka)
     glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ka);//��������
     if(Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Kd)
     glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Kd);//������ 
     if(Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ks)
     glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ks);//���淴�� 
     if(Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ns)
     glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ns);//�߹�
	 if(Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ke)
	 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ke);//�Է���
	}
	else
	{
		switch(UseOtherMtl)
		{
		case 0 :
			 break;
		case 1:;
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,BrassA);//��������
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, BrassD);//������
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,BrassS);//���淴��
			glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,BrassNs);//�߹�
			UseOtherMtl = 0;
			break;
		case 2:
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,GoldA);//��������
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, GoldD);//������
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,GoldS);//���淴��
			glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,GoldNs);//�߹�
			UseOtherMtl = 0;
			break;
		case 3:
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,EmeraldA);//��������
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, EmeraldD);//������
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,EmeraldS);//���淴��
			glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,EmeraldNs);//�߹�
			UseOtherMtl = 0;
			break;
		case 4:
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,VioletA);//��������
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, VioletD);//������
			glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,VioletS);//���淴��
			glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,VioletNs);//�߹�
			UseOtherMtl = 0;
			break;
		default:
			UseOtherMtl = 0;
			 break;
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//���û�Ϸ�ʽ
    glEnable(GL_BLEND);//���û��
    glBegin(GL_POLYGON);
    for(j=0;j<FaceSet[i].VNumber;j++)
      {
           if(j>24)
             break;
           if(Is_Vn)
           glNormal3fv(VnSet[FaceSet[i].Vn[j]-1]);
		   if(!Is_Vn && normal == 1)
		   glNormal3fv(VnSet[FaceSet[i].V[j]-1]);
           glVertex3fv(VSet[FaceSet[i].V[j]-1]);         
      }
     glEnd(); 
	glDisable(GL_BLEND);//��ֹ���
  glPopMatrix();
  }
  glFlush();
  glutSwapBuffers();
  
    return;
}
void ObjReader::AnalyseMtl()
{
      bool Is_Find=false;
      int i;
      char content[1024],TempMtl[64];
      int TempIndex=0;
      ifstream F(MtlName);
      if(!F.is_open())
       {
          cout<<"Error in opening ObjFile\n";
          exit(1);
       }
       while(!F.eof())
       {
         memset(content,0,sizeof(content));
         F.getline(content,1024);
         mytrimF(content);
         mytrimE(content);
         switch(content[0])
          {
             case '#'://ע��
                    break; 
             case 'n'://��������
                    memset(TempMtl,0,sizeof(TempMtl));
                    strtok(content," ");
                    strcpy(TempMtl,strtok(NULL," "));
                    Is_Find = false;
                    for(i=0;i<MtlNumber;i++)
                     {
                       if(strstr(Mtl[i].MtlName,TempMtl))
                              {
                                TempIndex = i;
                                Is_Find =true;
                                break;
                              }                               
                     } 
                     break;
             case 'K':// Ks/Kd/Ka/Ke
                     if(!Is_Find)
                       break;
                     strtok(content," ");
                     switch(content[1])
                      {
                         case 'a' ://Ka
                               //printf("%s\n",content);
                               Mtl[TempIndex].Ka[0] = atof(strtok(NULL," "));
                               //printf("%s\n",strtok(content," "));
                               Mtl[TempIndex].Ka[1] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Ka[2] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Is_Ka =true;
                               //printf("%d %f\n",TempIndex,Mtl[TempIndex].Ka[0]);
                               break;
                         case 'd' :
                               Mtl[TempIndex].Kd[0] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Kd[1] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Kd[2] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Is_Kd =true;
                               break;
                         case 's' :
                               Mtl[TempIndex].Ks[0] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Ks[1] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Ks[2] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Is_Ks =true;
                               break;
						 case 'e':
							   Mtl[TempIndex].Ke[0] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Ke[1] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Ke[2] = atof(strtok(NULL," "));
                               Mtl[TempIndex].Is_Ke =true;
                               break;
                         default:
                               break;
                      } 
                      break;
             case 'd'://͸���� 
                     if(!Is_Find)
                       break;
                     strtok(content," ");
                     Mtl[TempIndex].Ka[3] = atof(strtok(NULL," "));
					 Mtl[TempIndex].Kd[3] =  Mtl[TempIndex].Ka[3];
					 Mtl[TempIndex].Ks[3]=  Mtl[TempIndex].Ka[3];
                     Mtl[TempIndex].Is_d =true;
                     break; 
             //case 'T'://͸����
             case 'N'://shiness
                     if(!Is_Find)
                       break;
                     strtok(content," ");
                     Mtl[TempIndex].Ns = atof(strtok(NULL," "));
                     Mtl[TempIndex].Is_Ns =true;
                     break;
			/*
             case 'i':
                    if(!Is_Find)
                       break;
                    strtok(content," ");
                    Mtl[TempIndex].illum = atoi(strtok(NULL," "));
                    Mtl[TempIndex].Is_illum =true;
                    break;
					*/
             //case 'm' 
             default:
                    break;     
          }
       }
      #ifdef DEBUG
      printf("��%s�ж�ȡ�Ĳ���\n",MtlName);
      #endif 
	  F.close();
	  return;
}
void myGlutReshape( int x, int y )
{
  int tx, ty, tw, th;

  GLUI_Master.reshape();
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );

  window_x = tx;window_y = ty;
  width = tw;height = th;

  xy_aspect = (float)tw / (float)th;
  glutPostRedisplay();
}
void makeCheckImage(void)
{
	int i,j,c;
	for(i=0;i<checkImageHeight;i++)
		for(j=0;j<checkImageWidth;j++)
		{
			 c =((i&0x8)==0)^((j&0x8)==0) *255;
			 checkImage[i][j][0] = (GLubyte)c;
			 checkImage[i][j][1] = (GLubyte)c;
			 checkImage[i][j][2] = (GLubyte)c;
			 checkImage[i][j][3] = (GLubyte)255;
		}
}
void Init()
{
   glClearColor (0.0, 0.0, 0.0, 0.0); //���ñ���
    glClearDepth(1.0f);	//������Ȼ����ֵ
    //���ù���
    glLightfv(GL_LIGHT1, GL_AMBIENT, Light1Ambient); //���û�������ɫ	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light1Diffuse); //�������������ɫ
	glLightfv(GL_LIGHT1,GL_SPECULAR,Light1Specular);
	glLightfv(GL_LIGHT1, GL_POSITION,Light1Position);//���ù�Դλ��

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient); //���û�������ɫ	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse); //�������������ɫ
	glLightfv(GL_LIGHT0,GL_SPECULAR,LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);//���ù�Դλ��

	glEnable(GL_LIGHT1);//���ù���LIGHT1
	glEnable(GL_LIGHT0);//���ù���LIGHT2
    glEnable(GL_LIGHTING);//���ù���

    glShadeModel(GL_SMOOTH);//����Shade�ķ�ʽGL_SMOOTH����GL_FLAT
    glDepthFunc(GL_LEQUAL);//������Ȳ��Ժ���
    glEnable(GL_DEPTH_TEST);//������Ȳ���
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
/*
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1,&texName);
    glBindTexture(GL_TEXTURE_2D,texName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,3,Bit.Width,Bit.Height,0,GL_RGB,GL_UNSIGNED_BYTE,Pixels);
	*/
}
void InputKey(unsigned char key, int x, int y)
{
   float x1,x2,y1,y2,z1,z2;
   x1 = eye[0];y1 = eye[1];z1 = eye[2];
   x2 = center[0];y2 = center[1]; z2 = center[2];
   if(key=='s'||key=='S')
    {
        r *= MoveSpeed;
    }
   if(key=='w'||key=='W')
    {
        r /= MoveSpeed;  
    }
   if(key=='d'||key=='D')
    {
         phi += SpinSpeed;
    }
   if(key=='a'||key=='A')
    {
         phi -= SpinSpeed;
    } 
   if(key=='=')
    {
         ScaleX += ScaleSpeed;
         ScaleY += ScaleSpeed;
         ScaleZ += ScaleSpeed;
         glutPostRedisplay();   
		 return;
    } 
   if(key=='-')
   {
         ScaleX -= ScaleSpeed;
         ScaleY -= ScaleSpeed;
         ScaleZ -= ScaleSpeed;
         glutPostRedisplay();
		 return;
   }
   glutPostRedisplay();
   return;
} 
void MouseClick(int button,int state,int x,int y)
{
    if(button == GLUT_RIGHT_BUTTON)
           MouseRDown = !MouseRDown;
    if(button == GLUT_LEFT_BUTTON)
           MouseLDown = !MouseRDown;
    if(MouseRDown)
     {
       LastP[0] = x;
       LastP[1] = y;
     }
    if(MouseLDown)
    {
      LastP[0] = x;
      LastP[1] = y;             
    }     
}
void MouseMove(int x,int y)
{
     if(MouseRDown)
       {
           phi   += (LastP[0]-x)*PI/720;
           theta += (LastP[1]-y)*PI/720;
           LastP[0] = x;
           LastP[1] = y;
           glutPostRedisplay();
       }
     if(MouseLDown)
      {
         obj_pos[0] += cos(phi) *  (x - LastP[0])/10;
         obj_pos[1] += sin(theta)* (LastP[1] - y)/10;
         obj_pos[2] -= cos(theta)*(LastP[1] - y)/5 + sin(phi) * (LastP[0] - x)/5;
         LastP[0] = x;
         LastP[1] = y;
         glutPostRedisplay();
      }
}
void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  

  //glutPostRedisplay();
}
void Display()
{
	Reader1->DrawModel();
}
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowPosition(window_x,window_y);
    glutInitWindowSize(width,height);
    main_window = glutCreateWindow("OpenGL Obj Reader");
	
	Init();

	GLUI_Master.set_glutReshapeFunc( myGlutReshape ); 
    GLUI_Master.set_glutSpecialFunc( NULL );

    /****************************************/
    /*         Here's the GLUI code         */
    /****************************************/

	glui = GLUI_Master.create_glui_subwindow( main_window,GLUI_SUBWINDOW_RIGHT );
	obj_panel = new GLUI_Rollout(glui, "Properties", true);
    new GLUI_Checkbox( obj_panel, "FillMode", &FillMode,1,control_cb);//�߿�ģʽcheckbox
	new GLUI_Checkbox( obj_panel, "SelfMaterial", &selfMaterial );//�����������checkbox
	new GLUI_Checkbox( obj_panel, "Normalization", &normal,16,control_cb);//����ģʽcheckbox
	new GLUI_Checkbox( obj_panel, "Texture", &texture,21,control_cb);//��ͼģʽcheckbox
    new GLUI_Separator( obj_panel);
	
	GLUI_Panel *type_panel = new GLUI_Panel( obj_panel, "Render Mode" );
    radio = new GLUI_RadioGroup(type_panel,&shade_type,4,control_cb);
    new GLUI_RadioButton( radio, "Flat" );
    new GLUI_RadioButton( radio, "Smooth" );


	new GLUI_Separator( obj_panel);

	list =new GLUI_Listbox(obj_panel,"Material",(int*)0,10,control_cb);  
	list->add_item(1,"Brass");
	list->add_item(2,"Gold");
	list->add_item(3,"Emerald");
	list->add_item(4,"Violet");
	list->set_alignment(GLUI_ALIGN_RIGHT);

	new GLUI_Separator( obj_panel);

    GLUI_Spinner *scale_spinner = new GLUI_Spinner( obj_panel, "Scale:", &scale,8,control_cb);
    scale_spinner->set_float_limits( .2f, 4.0 );
    scale_spinner->set_alignment( GLUI_ALIGN_RIGHT);
	
	new GLUI_Separator(glui);
    GLUI_Panel *control_panel = new GLUI_Rollout(glui, "Control", true );
	GLUI_Rotation *view_rot = new GLUI_Rotation(control_panel, "Rotate Objects", view_rotate,9,control_cb);
    view_rot->set_spin( 1.0 ); 
	GLUI_Translation *trans_xy = new GLUI_Translation(control_panel, "Objects XY", GLUI_TRANSLATION_XY, obj_pos );
	new GLUI_Column(control_panel,false);
	 GLUI_Translation *trans_x = new GLUI_Translation(control_panel, "Objects X", GLUI_TRANSLATION_X, obj_pos );
     trans_x->set_speed( .05 );
    GLUI_Translation *trans_y = new GLUI_Translation( control_panel, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .05 );
    GLUI_Translation *trans_z = new GLUI_Translation( control_panel, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .05 );
	
	new GLUI_Separator(glui);

    GLUI_Panel *camera_panel = new GLUI_Rollout( glui, "Camera Information" );
	camera_info[0] = new GLUI_EditText(camera_panel,"PositionX: ");
	camera_info[1] = new GLUI_EditText(camera_panel,"PositionY: ");
	camera_info[2] = new GLUI_EditText(camera_panel,"PositionZ: ");
	new GLUI_Separator(camera_panel);
	camera_info[3] = new GLUI_EditText(camera_panel,"LookAtX: ");
	camera_info[4] = new GLUI_EditText(camera_panel,"LookAtY: ");
	camera_info[5] = new GLUI_EditText(camera_panel,"LookAtZ: ");
	new GLUI_Separator(camera_panel);
	camera_info[6] = new GLUI_EditText(camera_panel,"HeadX: ");
	camera_info[7] = new GLUI_EditText(camera_panel,"HeadY: ");
	camera_info[8] = new GLUI_EditText(camera_panel,"HeadZ: ");
	new GLUI_Separator(camera_panel);

    new GLUI_Column( glui, false );
    GLUI_Rollout *roll_lights = new GLUI_Rollout(glui, "Lights", true );

    GLUI_Panel *light0 = new GLUI_Panel( roll_lights, "Light 1" );
    GLUI_Panel *light1 = new GLUI_Panel( roll_lights, "Light 2" );

  new GLUI_Checkbox( light0, "Enabled", &light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb );
  light0_spinner = 
    new GLUI_Spinner( light0, "Intensity:", 
                      &light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb );
  light0_spinner->set_float_limits( 0.0, 1.0 );
  GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar( light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &LightDiffuse[0],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &LightDiffuse[1],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &LightDiffuse[2],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  new GLUI_Checkbox( light1, "Enabled", &light1_enabled,
                     LIGHT1_ENABLED_ID, control_cb );
  light1_spinner = 
    new GLUI_Spinner( light1, "Intensity:",
                      &light1_intensity, LIGHT1_INTENSITY_ID,
                      control_cb );
  light1_spinner->set_float_limits( 0.0, 1.0 );
  sb = new GLUI_Scrollbar( light1, "Red",GLUI_SCROLL_HORIZONTAL,
                           &Light1Diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Green",GLUI_SCROLL_HORIZONTAL,
                           &Light1Diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar( light1, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &Light1Diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  trans_xy->set_speed( .05 );
  
   fb = new GLUI_FileBrowser(glui, "",1, 7, control_cb);
   fb->set_h(150);
    new GLUI_Separator(glui);
    GLUI_Panel *object_panel = new GLUI_Rollout( glui, "Object Information");
	obj_info[0] = new GLUI_EditText(object_panel,"PositionX: ");
	obj_info[1] = new GLUI_EditText(object_panel,"PositionY: ");
	obj_info[2] = new GLUI_EditText(object_panel,"PositionZ: ");
	new GLUI_Separator(glui);

    new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );


   /**** Link windows to GLUI, and register idle callback ******/
  
    glui->set_main_gfx_window( main_window );

    /* We register the idle callback with GLUI, *not* with GLUT */
    GLUI_Master.set_glutIdleFunc( myGlutIdle ); 
   /*End of GLUI code*************************/
    glutKeyboardFunc(InputKey);//WASD����λ�� 
    glutMouseFunc(MouseClick);//�Ҽ����ó����������ģ��λ�� 
    glutMotionFunc(MouseMove);//����ƶ� 
    glutDisplayFunc(&Display);
	glutReshapeFunc(myGlutReshape);
	//glutLeaveMainLoop();
    glutMainLoop();
    return 0;
}