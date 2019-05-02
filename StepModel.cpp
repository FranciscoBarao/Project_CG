/*Compile 
g++ a.cpp -lGL -lGLU -lglut -lm -o main*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define M_PI 3.14159265358979323846
//--------------------------------- Definir cores
#define BLACK2 0.0, 0.0, 0.0, 1.0

#define PI 3.14159

//Sistema Coordenadas + objectos
GLint wScreen = 800, hScreen = 600;		 //.. janela (pixeis)
GLfloat xC = 10.0, yC = 10.0, zC = 10.0; //.. Mundo  (unidades mundo)

//Observador
GLfloat obsP[] = {4, 1, 4};
GLfloat obsT[] = {0, 1, -1};
GLfloat aVisao = - (atan(obsP[0]/obsP[2])*360)/(2*M_PI)+270; 
//Dados Casa
GLfloat width = 8, length = 16, height = 4;

//Dados Porta
GLfloat angZoom = 90; 
GLfloat door_angle = 0;
GLfloat window_slide = 0;

//Dados Escada
GLfloat stair_piso = 0, stair_largura = 0, stair_espelho = 0, stair_n_steps = 0, stair_foward = 0, stair_back = 0;

int in_stairs = -1;

//Dados Iluminação Ambiente
GLint   dia=1;
GLfloat intensidadeNoite[4]={0.1,0.1,0.1,1.0};
GLfloat luzGlobalCorAmb[4]={1.0,1.0,0.8,1.0}; 


//Dados Iluminação Foco
GLint   ligaFoco=1;
GLfloat aFoco=aVisao;
GLfloat focoDir[] = { obsT[0], 0, obsT[2]};
GLfloat focoExp   = 10.0;
GLfloat focoCut   = 30.0;

GLfloat focoCorDif[4] ={ 1.0, 1.0, 1.0, 1.0}; 
GLfloat focoCorEsp[4] ={ 1.0, 1.0, 1.0, 1.0}; 

GLfloat focoPosition[4] = {obsP[0],obsP[1],obsP[2],1};

//White
GLfloat whiteAmb  []={ 0.8 ,0.8 ,0.8 };
GLfloat whiteDif  []={ 0.55 ,0.55 ,0.55};
GLfloat whiteSpec []={ 0.870 ,0.870 ,0.870};
GLint 	whiteCoef   = 0.25 *128;


//luz teto propriedades
int light_teto_on = 1;
GLfloat localPos[4] = {width,height*2,length,1};
GLfloat localCorAmb[4] = {0.7,0.7,0.7,1.0};
GLfloat localCorDif[4]= { 0.7, 0.7, 0.7, 1.0};

GLuint texture[10];
RgbImage imag;


void initTexturas(){
	//----------------------------------------- Floor
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("Images/floor.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());


	//----------------------------------------- Wall
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("Images/chair.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	//----------------------------------------- Ceiling
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("Images/ceiling.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());



	//----------------------------------------- table
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	imag.LoadBmpFile("Images/table.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	//----------------------------------------- chair
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	imag.LoadBmpFile("Images/chair.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());


	//----------------------------------------- Red
	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	imag.LoadBmpFile("Images/red.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());

	//----------------------------------------- Red
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	imag.LoadBmpFile("Images/daylight.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
				 imag.GetNumCols(),
				 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
				 imag.ImageData());
}

void initMaterials(){
	glMaterialfv(GL_FRONT,GL_AMBIENT,   whiteAmb);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,   whiteDif);
	glMaterialfv(GL_FRONT,GL_SPECULAR,  whiteSpec);
	glMaterialf (GL_FRONT,GL_SHININESS, whiteCoef);
}

void initLights(void){
	//Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
	//Foco
	glLightfv(GL_LIGHT1, GL_POSITION,      focoPosition);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,focoDir);
	glLightf (GL_LIGHT1, GL_SPOT_EXPONENT ,focoExp);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,   focoCut);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,       focoCorDif);   
	glLightfv(GL_LIGHT1, GL_SPECULAR,      focoCorEsp);
	
	//Luz Tecto
	glLightfv(GL_LIGHT0, GL_POSITION,      localPos );   
	glLightfv(GL_LIGHT0, GL_AMBIENT,       localCorAmb );   
	glLightfv(GL_LIGHT0, GL_DIFFUSE,       localCorDif );
	//glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1);   
	//glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,1);
	//glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,2);


	glEnable(GL_LIGHT0);
}

void inicializa(void){
	glEnable(GL_DEPTH_TEST); //Profundidade
	glEnable(GL_NORMALIZE);                                    
	glShadeModel(GL_SMOOTH); //Interpolacao de cores

	glEnable(GL_LIGHTING);  
	//glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
//	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);    // dos dois lados

	initLights();
	initTexturas();

}

void cube(GLfloat size, int n_texture){
	//Ceiling
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
		initMaterials();
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-size, size, -size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(-size, size, size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(size, size, size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(size, size, -size);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	//Floor
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
	initMaterials();
	glNormal3f(0,1,0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-size, -size, size);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f( size, -size, size);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f( size, -size, -size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Front
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
		initMaterials();
		glNormal3f(0,0,1);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(size, size, size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-size, size, size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(-size, -size, size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(size, -size, size);
		glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	//Back
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
		initMaterials();
		glNormal3f(0,0,-1);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(size, size, -size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(size, -size, -size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(-size, -size, -size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-size, size, -size);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Right
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
		initMaterials();
		glNormal3f(-1,0,0);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(size, size, size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(size, -size, size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(size, -size, -size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(size, size, -size);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Left
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[n_texture]);
		initMaterials();
		glNormal3f(-1,0,0);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-size, size, size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(-size, size, -size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(-size, -size, -size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-size, -size, size);

		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void cubeTest(GLfloat size){
	//Floor
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	initMaterials();
	glNormal3f(0,1,0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0f, 0.0f);   glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 0.0f);	glVertex3f(-size, -size, size);
		glTexCoord2f(1.0f, 1.0f);	glVertex3f( size, -size, size);
		glTexCoord2f(0.0f, 1.0f);	glVertex3f( size, -size, -size);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Back
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		initMaterials();
		glNormal3f(0,0,1);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(size, size, -size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-size, size, -size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(-size, -size, -size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(size, -size, -size);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Right
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		initMaterials();
		glNormal3f(-1,0,0);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);	glVertex3f(size, size, size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(size, size, -size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(size, -size, -size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(size, -size, size);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//Left
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		initMaterials();
		glNormal3f(1,0,0);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); 	glVertex3f(-size, size, size);
			glTexCoord2f(0.0f, 1.0f);	glVertex3f(-size, -size, size);
			glTexCoord2f(1.0f, 1.0f);	glVertex3f(-size, -size, -size);
			glTexCoord2f(1.0f, 0.0f);	glVertex3f(-size, size, -size);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void stair(GLfloat width, GLfloat length, GLfloat height, GLfloat xIni, GLfloat yIni, GLfloat zIni, int nSteps){
	float piso = length;
	float largura = width;
	float espelho = height; //(0.4 - piso) / 2;

	GLfloat Mov_XD = 0, Mov_YD = espelho / 2, Mov_Z = 0;

	int i = 0;

	for (i = 0; i < nSteps; i++)
	{
		if (i != 0)
		{
			Mov_YD += espelho;
			Mov_XD += (2 * piso);
		}
		//primeira barra grande
		if (i == 0)
		{
			glPushMatrix();
			glTranslatef(xIni + 0.1 * piso, yIni + espelho + espelho / 2, zIni + largura * 2 - 0.05 * largura);
			glScalef(0.1 * piso, espelho, 0.05 * largura);
			cube(1, 5);
			glPopMatrix();
		}
		glPushMatrix();
		glTranslatef(Mov_XD + xIni + piso, Mov_YD + yIni, Mov_Z + zIni + largura);
		glScalef(piso, espelho / 2, largura);
		cube(1, 2);
		glPopMatrix();
		//barras pequenas
		if (i != 0 && i < nSteps)
		{
			glPushMatrix();
			glTranslatef(xIni + 0.05 * piso + Mov_XD, yIni + Mov_YD + espelho, zIni + largura * 2 - 0.05 * largura + Mov_Z);
			glScalef(0.05 * piso, espelho, 0.05 * largura);
			cube(1, 5);
			glPopMatrix();
		}
		//capas
		if (i > 0)
		{
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[5]);
			initMaterials();
			glNormal3f(0,0,1);
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(Mov_XD + xIni, Mov_YD + yIni, Mov_Z + zIni + 2 * largura);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, Mov_YD + yIni, Mov_Z + zIni + 2 * largura);
				glTexCoord2f(1.0f, 1.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, 0, Mov_Z + zIni + 2 * largura);
				glTexCoord2f(1.0f, 0.0f);	glVertex3f(Mov_XD + xIni, 0, Mov_Z + zIni + 2 * largura);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
			
		}
		//ultima barra grande
		if (i == (nSteps - 1))
		{
			glPushMatrix();
			glTranslatef(Mov_XD + xIni + piso * 2 - 0.1 * piso, Mov_YD + yIni + espelho + espelho / 2 + espelho / 8, zIni + largura * 2 - 0.05 * largura);
			glScalef(0.1 * piso, espelho + espelho / 8, 0.05 * largura);
			cube(1, 1);
			glPopMatrix();

			glPushMatrix();
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture[5]);
				initMaterials();
				glNormal3f(0,1,0);
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0f, 0.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, Mov_YD + yIni, Mov_Z + zIni);
					glTexCoord2f(0.0f, 1.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, Mov_YD + yIni, Mov_Z + zIni + 2 * largura);
					glTexCoord2f(1.0f, 1.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, 0, Mov_Z + zIni + 2 * largura);
					glTexCoord2f(1.0f, 0.0f);	glVertex3f(Mov_XD + xIni + 2 * piso, 0, Mov_Z + zIni);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			
		}
	}
	GLfloat angle = (atan((((espelho / 2) * nSteps)) / ((piso * (nSteps)))) * 360) / (2 * M_PI);
	GLfloat size = pow((((espelho / 2) * nSteps) + 1) / 2, 2) + pow((piso * (nSteps + 2)), 2);
	size = sqrt(size);
	glPushMatrix();
	glTranslatef(xIni + Mov_XD / 2 + piso, Mov_YD - espelho * 4 - espelho / 8, zIni + largura * 2 - 0.05 * largura);
	glRotatef(angle, 0, 0, 1);
	glScalef(size, espelho / 8, 0.05 * largura);
	cube(1, 1);
	glPopMatrix();
}

void drawWallDoor(GLfloat width, GLfloat length, GLfloat height, GLfloat width_door, GLfloat height_door, GLfloat depth_wall, GLfloat door_xPos){
	glPushMatrix(); //Left wall
	glTranslatef(door_xPos + width_door / 2 + (width - (door_xPos + width_door / 2)) / 2, height / 2, length);
	glScalef(width - (door_xPos + width_door / 2), height, depth_wall);
	cube(0.5, 1);
	glPopMatrix();

	glPushMatrix(); //Right wall
	glTranslatef((door_xPos - width_door / 2) / 2, height / 2, length);
	glScalef(door_xPos - width_door / 2, height, depth_wall);
	cube(0.5, 1);
	glPopMatrix();

	glPushMatrix(); //Middle wall
	glTranslatef(door_xPos, height_door + (height - height_door) / 2, length);
	glScalef(width_door, height - height_door, depth_wall);
	cube(0.5, 1);
	glPopMatrix();

	glPushMatrix(); //Door
	glTranslatef(door_xPos + width_door / 2, height_door / 2, length + depth_wall / 2);
	glRotatef(door_angle, 0, 1, 0);
	glTranslatef(-width_door / 2, 0, 0);

	glScalef(width_door, height_door, depth_wall / 2);
	cube(0.5, 5);
	glPopMatrix();
}
void drawWallWindow(GLfloat width, GLfloat length, GLfloat height, GLfloat width_window, GLfloat height_window, GLfloat depth_wall, GLfloat window_xPos, GLfloat window_yPos){
	glPushMatrix(); //Left wall
	glTranslatef(window_xPos + width_window / 2 + (width - (window_xPos + width_window / 2)) / 2, height / 2, length);
	glScalef(width - (window_xPos + width_window / 2), height, depth_wall);
	cube(0.5, 4);
	glPopMatrix();

	glPushMatrix(); //Right wall
	glTranslatef((window_xPos - width_window / 2) / 2, height / 2, length);
	glScalef(window_xPos - width_window / 2, height, depth_wall);
	cube(0.5, 4);
	glPopMatrix();

	glPushMatrix(); //Middle wall Up
	glTranslatef(window_xPos, window_yPos + height_window / 2 + (height - (window_yPos + height_window / 2)) / 2, length);
	glScalef(width_window, height - (window_yPos + height_window / 2), depth_wall);
	cube(0.5, 4);
	glPopMatrix();

	glPushMatrix(); //Middle wall Down
	glTranslatef(window_xPos, (window_yPos - height_window / 2) / 2, length);
	glScalef(width_window, window_yPos - height_window / 2, depth_wall);
	cube(0.5, 4);
	glPopMatrix();

	glPushMatrix(); //Window
	glTranslatef(window_xPos, window_yPos + window_slide, length + depth_wall / 2);
	glScalef(width_window, height_window - window_slide, depth_wall / 2);
	cube(0.5, 4);
	glPopMatrix();

	glPushMatrix(); //Window
	glTranslatef(window_xPos, window_yPos, length + depth_wall);
	glScalef(width_window, height_window, 0.1);
	cube(0.5, 6);
	glPopMatrix();
}

void drawCeiling(GLfloat width, GLfloat length, GLfloat height, GLfloat width_window, GLfloat height_window, GLfloat depth_wall, GLfloat window_xPos, GLfloat window_yPos){
	//before stair
	glPushMatrix();
	glTranslatef((width / 2 + 2 * stair_piso) / 2, height+0.001, stair_largura);
	glScalef(width / 2 + 2 * stair_piso, 0, stair_largura * 2);
	cube(0.5, 2);
	glPopMatrix();
	//after stair
	glPushMatrix();
	glTranslatef(width - (stair_foward * stair_largura - stair_n_steps * stair_piso) / 2, height+0.001, stair_largura);
	glScalef(((width - stair_foward * stair_piso - stair_n_steps * stair_piso) / 2) / 2, 0, stair_largura * 2);
	cube(0.5, 2);
	glPopMatrix();
	//first floor right ceilling
	glPushMatrix();
	glTranslatef(width / 2, height+0.001, length / 2 + stair_largura);
	glScalef(width, 0, length - stair_largura * 2);
	cube(0.5, 2);
	glPopMatrix();

	//second floor ceilling
	glPushMatrix();
	glTranslatef(width / 2, height * 2, length / 2);
	glScalef(width, 0, length);
	cube(0.5, 2);
	glPopMatrix();
}

void drawTable(GLfloat width, GLfloat length, GLfloat thickness, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat legs_thickness){

	glPushMatrix();
	glTranslatef(xPos, yPos, zPos);
	glScalef(width, thickness, length);
	cube(0.5, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xPos, yPos / 2, zPos);
	glScalef(legs_thickness, yPos, length / 3);
	cube(0.5, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(xPos, yPos / 2, zPos);
	glRotatef(90, 0, 1, 0);
	glScalef(legs_thickness, yPos, width / 3);
	cube(0.5, 3);
	glPopMatrix();
}

void drawChair(GLfloat width, GLfloat length, GLfloat thickness, GLfloat leg_size, GLfloat xPos, GLfloat yPos, GLfloat zPos){
	glPushMatrix();
		glNormal3f(0,1,0);
		glTranslatef(xPos, yPos, zPos);
		glScalef(width, thickness, length);
		cube(0.5, 5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xPos, yPos + width / 2, zPos - length / 2);
		glScalef(length, width * (3 / 2), thickness);
		cube(0.5, 5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xPos + width / 2 - leg_size / 2, yPos / 2, zPos - length / 2 + leg_size / 2);
		glScalef(leg_size, yPos, leg_size);
		cube(0.5, 5 );
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xPos - width / 2 + leg_size / 2, yPos / 2, zPos - length / 2 + leg_size / 2);
		glScalef(leg_size, yPos, leg_size);
		cube(0.5, 5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xPos - width / 2 + leg_size / 2, yPos / 2, zPos + length / 2 - leg_size / 2);
		glScalef(leg_size, yPos, leg_size);
		cube(0.5, 5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(xPos + width / 2 - leg_size / 2, yPos / 2, zPos + length / 2 - leg_size / 2);
		glScalef(leg_size, yPos, leg_size);
		cube(0.5, 5);
	glPopMatrix();
}

void walls(GLfloat width, GLfloat length, GLfloat height, GLfloat width_window, GLfloat height_window, GLfloat depth_wall, GLfloat window_xPos, GLfloat window_yPos){
	glPushMatrix();
	glTranslatef(width / 2, height / 2, length / 2);
	glScalef(width, height, length);
	cubeTest(0.5);
	glPopMatrix();
	drawWallWindow(width, length, height, width_window, height_window, depth_wall, window_xPos, window_yPos);
}

void drawScene(){
	GLfloat depth_wall = 0.1, width_door = 1.5, height_door = 2, door_xPos = 2;
	GLfloat width_window = 1, height_window = 1.5, window_yPos = 2, window_xPos = 5;

	GLfloat table_width = 2, table_length = 2, table_thickness = 0.1, table_legs_thickness = 0.2, table_xPos = 7, table_yPos = 0.8, table_zPos = 15;
	GLfloat chair_width = 0.7, chair_length = 0.7, chair_thickness = 0.1, chair_leg_size = 0.05;

	/*Mudar nome destas merdas para stair_sth*/
	GLfloat largura = 0.6;
	GLfloat nSteps = 16;
	GLfloat foward = 6;
	GLfloat back = 6;
	stair_foward = foward;
	stair_back = back;
	GLfloat espelho = height / (nSteps - 2), piso = ((width - width / back) / 2) / nSteps;
	GLfloat Stair_Ini_X = piso * foward, Stair_Ini_Y = 0, Stair_Ini_Z = 0;
	nSteps = nSteps - (foward / 2);
	stair_espelho = espelho, stair_largura = largura, stair_n_steps = nSteps, stair_piso = piso;
	
	stair(largura, piso, espelho, Stair_Ini_X, Stair_Ini_Y, Stair_Ini_Z, nSteps + 1);
	walls(width, length, 2 * height, width_window, height_window, depth_wall, window_xPos, window_yPos);
	drawWallDoor(width, length / 2, height, width_door, height_door, depth_wall, door_xPos);
	drawTable(table_width, table_length, table_thickness, table_xPos, table_yPos, table_zPos, table_legs_thickness);
	drawChair(chair_width, chair_length, chair_thickness, chair_leg_size, 7, 0.4, 14);
	drawCeiling(width, length, height, width_window, height_window, depth_wall, window_xPos, window_yPos);

	glPopMatrix();
}
void display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Apaga ecra/profundidade

	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	drawScene();

	glutSwapBuffers();
}
bool check_collisions_walls(GLfloat x, GLfloat z){
	//0.2  +/- valor maximo que pode vir a somar.. aproxima do valor real para nao deixar ver dentro das paredes
	return ((x >= 0.2) && (x <= width-0.2) 
		&&  (z >= 0.2) && (z <= length-0.2));

	//return true;
}

void keyboard(unsigned char key, int x, int y){
	switch (key)
	{
	case 'f':
	case 'F':
		ligaFoco = !ligaFoco;
		if (ligaFoco) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case 'O':
	case 'o':
		if (door_angle > -90)
		{
			door_angle -= 5;
			window_slide += 0.1;
		}
		glutPostRedisplay();
		break;
	case 'C':
	case 'c':
		if (door_angle < 0)
		{
			door_angle += 5;
			window_slide -= 0.1;
		}
		glutPostRedisplay();
		break;
	case 'L':
	case 'l':
		in_stairs = in_stairs * -1;
		if (in_stairs < 0)
		{
			if(obsP[1] >= height){
				obsP[0] = stair_piso * 4;
				obsP[1] = height+1;
				obsP[2] = stair_largura;
				obsT[0] = 10;
				obsT[1] = height+1;
				obsT[2] = 0;
				aVisao = 0;
				//printf("angulo = %f",aVisao);
			}
			else{
				obsP[0] = 4;
				obsP[1] = 1;
				obsP[2] = 4;
				obsT[0] = 0;
				obsT[1] = 1;
				obsT[2] = 0;
				aVisao =-(atan(obsP[0]/obsP[2])*360)/(2*M_PI) + 270 ;

				//printf("angulo = %f",aVisao);
			}
		}
		else
		{
			if(obsP[1]>= height){
				obsP[0] = stair_n_steps*stair_largura;
				obsP[1] = height+2;
				obsP[2] = stair_largura;
				obsT[0] = -10;
				obsT[1] = height+2;
				obsT[2] = 0;
				aVisao = 180;
			}
			else{
				obsP[0] = stair_piso * 4;
				obsP[1] = 1;
				obsP[2] = stair_largura;
				obsT[0] = 10;
				obsT[1] = 1;
				obsT[2] = 0;
				aVisao = 0;
				//printf("angulo = %f",aVisao);
			}
		}
		glutPostRedisplay();
		break;
	case 'T':
	case 't':
		if(light_teto_on){
			glDisable(GL_LIGHT0);
			light_teto_on = 0;
		}
		else
		{
			light_teto_on = 1;
			glEnable(GL_LIGHT0);
		}
		glutPostRedisplay();
		break;
	case 'N':
	case 'n':
		if(dia){
			dia = 0;
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, intensidadeNoite);
			glutPostRedisplay();
		}
		else{
			dia = 1;
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
			glutPostRedisplay();
		}
		break;
	case 27:
		exit(0);
		break;
	}
}

void teclasNotAscii(int key, int x, int y){
	if (in_stairs == -1)
	{
		if (key == GLUT_KEY_UP && check_collisions_walls(obsP[0] + 0.2 * cos(aVisao), obsP[2] + 0.2 * sin(aVisao)))
		{
			obsP[0] += 0.2 * cos((aVisao*2*PI)/360);
			obsP[2] += 0.2 * sin((aVisao*2*PI)/360);
		}
		if (key == GLUT_KEY_DOWN && check_collisions_walls(obsP[0] - 0.2 * cos(aVisao), obsP[2] - 0.2 * sin(aVisao)))
		{
			obsP[0] -= 0.2 * cos((aVisao*2*PI)/360);
			obsP[2] -= 0.2 * sin((aVisao*2*PI)/360);
		}
		if (key == GLUT_KEY_LEFT)
		{
			aVisao -= 5;
		}
		if (key == GLUT_KEY_RIGHT)
		{
			aVisao += 5;
		}
		obsT[0] = obsP[0] + 2 * cos((aVisao*2*PI)/360);
		obsT[2] = obsP[2] + 2 * sin((aVisao*2*PI)/360);

		aFoco = aVisao;
		focoDir[0] = obsT[0] - obsP[0];
		focoDir[2] = obsT[2] - obsP[2];
		focoPosition[0] = obsP[0];
		focoPosition[2] = obsP[2];
		}
	else
	{
		if (key == GLUT_KEY_UP && check_collisions_walls(obsP[0] + stair_piso * 2, obsP[1] + stair_espelho))
		{
			obsP[0] += stair_piso * 2;
			obsP[1] += stair_espelho;
		}
		if (key == GLUT_KEY_DOWN && check_collisions_walls(obsP[0] - stair_piso * 2, obsP[1] - stair_espelho))
		{
			obsP[0] -= stair_piso * 2;
			obsP[1] -= stair_espelho;
		}

		if (key == GLUT_KEY_LEFT)
		{
			aVisao -= 5;
		}
		if (key == GLUT_KEY_RIGHT)
		{
			aVisao += 5;
		}
		obsT[0] = obsP[0] + 2 * cos((aVisao*2*PI)/360);
		obsT[2] = obsP[2] + 2 * sin((aVisao*2*PI)/360);
		obsT[1] = obsP[1];
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("|Foco:'f'| |Spectate:'Arrows'|  |Open/Close door - 'o|c'|");

	inicializa();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}