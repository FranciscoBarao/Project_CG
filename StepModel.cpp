/*Compile 
g++ a.cpp -lGL -lGLU -lglut -lm -o main*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0

#define PI		 3.14159


//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen=800, hScreen=600;		//.. janela (pixeis)
GLfloat		xC=10.0, yC=10.0, zC=10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador 
GLfloat  aVisao = PI*1.5; //Porque PI*1.5?  aproximadamente 5
GLfloat  obsP[] = {0, 1, 5};
GLfloat  obsT[] = {0,1,0};
GLfloat  angZoom=90;

GLboolean   frenteVisivel=1;
GLboolean   visivel = 1;



void inicializa(void){
	glClearColor(BLACK);		//Apagar
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//Faces visiveis
	glCullFace(GL_BACK);		//Mostrar so as da frente		
}



void drawEixos(){	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(RED);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i(20, 0, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0); 
		glVertex3i(0, 20, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i( 0, 0, 20); 
	glEnd();

}	

void drawRectangle(GLfloat width, GLfloat length, GLfloat height, GLfloat r, GLfloat g, GLfloat b){
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex3f(-width, height, -length);
		glVertex3f(-width, height, length);
		glVertex3f(width, height, length);
		glVertex3f(width, height, -length);
	glEnd();
}

void stair(GLfloat width, GLfloat length,GLfloat xIni,GLfloat yIni,GLfloat zIni, int nSteps){
	float piso = length;
	float largura = width;
	float espelho = (0.4 - piso) / 2;

	GLfloat Mov_XU = 0, Mov_XD=0, Mov_YD=0, Mov_YU=0, Mov_Z=0;

	if (espelho < 0)
	{
		espelho = -espelho;
	}
	int i=0;
	Mov_YU = espelho/2;
	Mov_XU = largura;
	for (i=0;i<nSteps;i++)
	{
		//down
		glPushMatrix();
		glTranslatef(Mov_XD+xIni, Mov_YD+yIni, Mov_Z+zIni);
		drawRectangle(largura, piso, 0, 1, 0, 0);
		glPopMatrix();
		
		
		//up
		glPushMatrix();
		glTranslatef(Mov_XU+xIni, Mov_YU+yIni, Mov_Z+zIni);
		glRotatef(90, 0, 0, 1);
		drawRectangle(largura/5, piso,0, 0, 1, 0);
		glPopMatrix();

		Mov_YU += espelho;
		Mov_YD += espelho;
		Mov_XD += (2*largura);
		Mov_XU += 2*largura;
	}
}



void walls(GLfloat width, GLfloat length, GLfloat height){
	glPushMatrix();
		//Wall x/y
		glTranslatef(0,height,width);
		glRotatef(90, 0, 0, 1);
		drawRectangle(height,width,0,1,0,1);
	glPopMatrix();

	glPushMatrix();
		//Wall x/y on z
		glTranslatef(2*length,height,width);
		glRotatef(90, 0, 0, 1);
		drawRectangle(height,width,0,1,0,1);
	glPopMatrix();

	glPushMatrix();
		//Wall z/y
		glTranslatef(length,height,0);
		glRotatef(90, 1, 0, 0);
		drawRectangle(length,height,0,0,1,1);
	glPopMatrix();

	glPushMatrix();
		//Wall z/y on x
		glTranslatef(length,height,2*width);
		glRotatef(90, 1, 0, 0);
		drawRectangle(length,height,0,0,1,1);
	glPopMatrix();

	glPushMatrix();
		//Floor
		glTranslatef(length,0,width);
		drawRectangle(length,width,0,1,1,0);
	glPopMatrix();

	glPushMatrix();
		//Ceiling
		glTranslatef(length,2*height,width);
		drawRectangle(length,width,0,0,1,0);
	glPopMatrix();

}
void drawWallDoor(GLfloat width, GLfloat length, GLfloat height, GLfloat lengthD, GLfloat heightD){
	glPushMatrix();
		glTranslatef(length+lengthD,0,width);
		walls(0.1,length/2 - lengthD/2 ,height);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,0, width);
		walls(0.1,length/2-lengthD/2, height);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(lengthD, height, width);
		walls(0.1,lengthD, height-heightD);
	glPopMatrix();
}

void drawScene(){
	
    if (frenteVisivel)
	    glCullFace(GL_BACK);  //glFrontFace(GL_CW);
	else
	    glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);

	glPushMatrix();	
		//stair(0.25, 0.2,-1,-1,-1,4);
		//walls(4,2,1);
		
		drawRectangle(2,2,2, 1,1,1);
		//drawWallDoor(4,2,1,1,0.75);
		glColor4f(YELLOW);
		glutWireTeapot(1);		 
	glPopMatrix();
}

void display(void){
	//Apaga ecra/profundidade
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//================================================================= NAO MOFIFICAR
	glViewport (0, 0, wScreen, hScreen);								
	glMatrixMode(GL_PROJECTION);										// ESQUECER PoR AGORA
	glLoadIdentity();													// ESQUECER PoR AGORA
	gluPerspective(angZoom, (float)wScreen/hScreen, 0.1, 3*zC);			
	glMatrixMode(GL_MODELVIEW);											// ESQUECER PoR AGORA	
	glLoadIdentity();													// ESQUECER PoR AGORA
	//================================================================= NAO MOFIFICAR

	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0],obsT[1],obsT[2], 0, 1, 0);

	drawEixos(); 
	drawScene(); 	
	
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y){
	
	switch (key) {
	case 'f':
	case 'F':
		frenteVisivel=!frenteVisivel;
		glutPostRedisplay();
		break;

	case 'V':
	case 'v':
		if(visivel) glDisable(GL_CULL_FACE);
		else glEnable(GL_CULL_FACE);
		visivel = !visivel;
		glutPostRedisplay();
	break;

	case 'S':
	case 's':
		glutPostRedisplay();
	break;

	case 'e':
	case 'E':
		glutPostRedisplay();
	break;

	case 'd':
	case 'D':
		glutPostRedisplay();
	break;
	case 27:
		exit(0);
		break;	
  }

}



void teclasNotAscii(int key, int x,int y){
		if(key == GLUT_KEY_UP){
			obsP[0] += 0.2 * cos(aVisao);
			obsP[2] += 0.2 * sin(aVisao);	
		}
		if(key == GLUT_KEY_DOWN){
			obsP[0] -= 0.2 * cos(aVisao);
			obsP[2] -= 0.2 * sin(aVisao);
		}
		if(key == GLUT_KEY_LEFT){
			aVisao -= 0.2;
		}
		if(key == GLUT_KEY_RIGHT){
			aVisao += 0.2;
		}
		obsT[0] = obsP[0] + 2*cos(aVisao);
		obsT[2] = obsP[2] + 2*sin(aVisao);
		/*
		Virar o User
		A = X + r*Cos(Alpha+/- 0.1)	
		B = B
		C = Y + r*Cos(Alpha+/- 0.1)
		Frente/Tras do user
		x = x + passo * cos alpha
		z = z + passo * sin alpha
		Atualizar variaveis da circunferencia
		A = A + passo*cosAlpha
		C = C + passo*sinAlpha
		*/
	glutPostRedisplay();	
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen); 
	glutInitWindowPosition (300, 100); 
	glutCreateWindow ("|FaceVisivel:'f'|      |Observador:'SETAS'|        |Enable/Disable Face-'v'|");
  
	inicializa();
	
	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	return 0;
}