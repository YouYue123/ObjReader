#include"ObjReader.h"
using namespace std;
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
ObjReader::ObjReader()
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
                 ViewMinX = 65535;
                 ViewMinY = 65535;
                 ViewMinZ = 65535;
                 ViewMaxX = -65535;
                 ViewMaxY = -65535;
                 ViewMaxZ = -65535;
           
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
                    if(content[1]=='t')//��ͼ����� 
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
                    if(content[1]=='n')//���㷨����Ϣ 
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
     printf("��%d����:\nKa%d---%f %f %f\n",FaceNumber+1,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Is_Ka,Mtl[Group[FaceSet[i].GroupIndex].MtlIndex].Ka[0]
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
           glVertex3fv(VSet[FaceSet[i].V[j]-1]);         
      }
     glEnd(); 
	glDisable(GL_BLEND);//��ֹ���
    /*
    if(FaceSet[i].VNumber==3)
    {
      glBegin(GL_TRIANGLES);
         if(Is_Vn)
         glNormal3fv(VnSet[FaceSet[i].Vn[0]-1]);
         glVertex3fv(VSet[FaceSet[i].V[0]-1]);
         if(Is_Vn)
        glNormal3fv(VnSet[FaceSet[i].Vn[1]-1]);
         glVertex3fv(VSet[FaceSet[i].V[1]-1]);
         if(Is_Vn)
        glNormal3fv(VnSet[FaceSet[i].Vn[2]-1]);
         glVertex3fv(VSet[FaceSet[i].V[2]-1]);
         #ifdef TestVn
         printf("%d %d %d\n",FaceSet[i].Vn[0],FaceSet[i].Vn[1],FaceSet[i].Vn[2]);
        printf("%f %f %f \n",VnSet[FaceSet[i].Vn[0]-1][0],VnSet[FaceSet[i].Vn[0]-1][1],VnSet[FaceSet[i].Vn[0]-1][2]);
         #endif
      glEnd();
    } 
    if(FaceSet[i].VNumber==4)
    {
      glBegin(GL_POLYGON);
         if(Is_Vn)
        glNormal3fv(VnSet[FaceSet[i].Vn[0]-1]);
         glVertex3fv(VSet[FaceSet[i].V[0]-1]);
         if(Is_Vn)
         glNormal3fv(VnSet[FaceSet[i].Vn[1]-1]);
         glVertex3fv(VSet[FaceSet[i].V[1]-1]);
         if(Is_Vn)
         glNormal3fv(VnSet[FaceSet[i].Vn[2]-1]);
         glVertex3fv(VSet[FaceSet[i].V[2]-1]);
         if(Is_Vn)
         glNormal3fv(VnSet[FaceSet[i].Vn[3]-1]);
         glVertex3fv(VSet[FaceSet[i].V[3]-1]);
         #ifdef TestVn
         printf("%d %d %d %d\n",FaceSet[i].Vn[0],FaceSet[i].Vn[1],FaceSet[i].Vn[2],FaceSet[i].Vn[3]);
         printf("%f %f %f %f\n",VnSet[FaceSet[i].Vn[0]-1][0],VnSet[FaceSet[i].Vn[0]-1][1],VnSet[FaceSet[i].Vn[0]-1][2],VnSet[FaceSet[i].Vn[0]-1][3]);
         #endif
      glEnd();
    }
    */ 
  glFlush();
  glPopMatrix();
  }
  glutSwapBuffers();
    //return;
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
             case 'i':
                    if(!Is_Find)
                       break;
                    strtok(content," ");
                    Mtl[TempIndex].illum = atoi(strtok(NULL," "));
                    Mtl[TempIndex].Is_illum =true;
                    break;
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