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
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define PI		 3.14159


//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen=800, hScreen=600;		//.. janela (pixeis)
GLfloat		xC=10.0, yC=10.0, zC=10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador 
GLfloat  rVisao=10, aVisao=0.5*PI, incVisao=0.05;
GLfloat  obsP[] ={rVisao*cos(aVisao), 1.0, rVisao*sin(aVisao)};
GLfloat  obsT[] ={0,0,0};
GLfloat  angZoom=90;
GLfloat  incZoom=3;


//=========================================================== FACES DA MESA
GLboolean   frenteVisivel=1;



void inicializa(void)
{
	glClearColor(BLACK);		//Apagar
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//Faces visiveis
	glCullFace(GL_BACK);		//Mostrar so as da frente		
}



void drawEixos()
{	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(RED);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i(10, 0, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0); 
		glVertex3i(0, 10, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i( 0, 0,10); 
	glEnd();

}	

void drawRectangleFlat(GLfloat width, GLfloat length, GLfloat height, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);	                  
		glVertex3f(-width , 0 , -length); 
		glVertex3f(-width , 0 , length);
		glVertex3f(width , 0 , length);
		glVertex3f(width , 0 , -length);               
	glEnd();
}
void drawRectangleUP(GLfloat width, GLfloat length, GLfloat height, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
		glVertex3f(-width , 1 , -length); 
		glVertex3f(-width , 1 , length);
		glVertex3f(width , 1 , length);
		glVertex3f(width , 1 , -length);
	glEnd();
}


void step(){
		drawRectangleFlat(2,4,0,1,0,0);

		glPushMatrix();
			glTranslatef(0,0,0);
			glRotatef(-90,0,0,1);
			drawRectangleUP(0.5,4,0, 0,1,0);
		glPopMatrix();
		}	


void drawScene(){
	
    if (frenteVisivel)
	    glCullFace(GL_BACK);  //glFrontFace(GL_CW);
	else
	    glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);

	int i;
	glPushMatrix();	
		step();

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

	//Objectos/modelos
	drawEixos(); 
	drawScene(); 	
	
	glutSwapBuffers();
}


//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){
	
	switch (key) {
	case 'f':
	case 'F':
		frenteVisivel=!frenteVisivel;
		glutPostRedisplay();
		break;

	case 'A':
	case 'a':
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
	glutCreateWindow ("{jh,pjmm}@dei.uc.pt|       |FaceVisivel:'f'|      |Observador:'SETAS'|        |Andar-'a/s'|        |Rodar -'e/d'| ");
  
	inicializa();
	
	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	return 0;
}