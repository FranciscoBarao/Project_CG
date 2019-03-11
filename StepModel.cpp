/*Compile 
g++ a.cpp -lGL -lGLU -lglut -lm -o main*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

//--------------------------------- Definir cores
#define BLUE2     0.0, 0.0, 1.0, 1.0
#define RED2		 1.0, 0.0, 0.0, 1.0
#define GREEN2    0.0, 1.0, 0.0, 1.0
#define BLACK2    0.0, 0.0, 0.0, 1.0
struct Color{
	GLfloat r,g,b;
};
Color RED,GREEN,BLUE,YELLOW,PURPLE,CYAN,BLACK;

#define PI		 3.14159

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen=800, hScreen=600;		//.. janela (pixeis)
GLfloat		xC=10.0, yC=10.0, zC=10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador 
GLfloat  aVisao = PI*1.5; //Porque PI*1.5?  aproximadamente 5
GLfloat  obsP[] = {0, 1, 5};
GLfloat  obsT[] = {0,1,0};

GLfloat  angZoom=90;
GLfloat  door_angle=0;

GLboolean   frenteVisivel=1;
GLboolean   visivel = 1;



void inicializa(void){
	glClearColor(BLACK2);		//Apagar
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//Faces visiveis
	glCullFace(GL_BACK);		//Mostrar so as da frente		
}

void drawEixos(){	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(RED2);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i(20, 0, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(GREEN2);
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0); 
		glVertex3i(0, 20, 0); 
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(BLUE2);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0); 
		glVertex3i( 0, 0, 20); 
	glEnd();

}	

void drawSquare(GLfloat size, Color color){
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POLYGON);
		glVertex3f(-size, 0, -size);
		glVertex3f(-size, 0, size);
		glVertex3f(size, 0, size);
		glVertex3f(size, 0, -size);
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
		glScalef(largura,piso,1);
		drawSquare(1, RED);
		glPopMatrix();
		
		
		//up
		glPushMatrix();
		glTranslatef(Mov_XU+xIni, Mov_YU+yIni, Mov_Z+zIni);
		glRotatef(90, 0, 0, 1);
		glScalef(largura/5,piso,1);
		drawSquare(1, GREEN);
		glPopMatrix();

		Mov_YU += espelho;
		Mov_YD += espelho;
		Mov_XD += (2*largura);
		Mov_XU += 2*largura;
	}
}

void walls(GLfloat width, GLfloat length, GLfloat height, Color colors[]){
	glPushMatrix();
		//Wall x/y
		glTranslatef(0,height,width);
		glRotatef(90, 0, 0, 1);
		glScalef(height,1,width);
		drawSquare(1,colors[0]);
	glPopMatrix();

	glPushMatrix();
		//Wall x/y on 2z
		glTranslatef(2*length,height,width);
		glRotatef(90, 0, 0, 1);
		glScalef(height,1,width);
		drawSquare(1,colors[1]);
	glPopMatrix();

	glPushMatrix();
		//Wall z/y
		glTranslatef(length,height,0);
		glRotatef(90, 1, 0, 0);
		glScalef(length,1,height);
		drawSquare(1,colors[2]);
	glPopMatrix();

	glPushMatrix();
		//Wall z/y on 2x
		glTranslatef(length,height,2*width);
		glRotatef(90, 1, 0, 0);
		glScalef(length,1,height);		
		drawSquare(1,colors[3]);
	glPopMatrix();

	glPushMatrix();
		//Floor
		glTranslatef(length,0,width);
		glScalef(length,1,width);		
		drawSquare(1,colors[4]);
	glPopMatrix();

	glPushMatrix();
		//Ceiling
		glTranslatef(length,2*height,width);
		glScalef(length,1,width);		
		drawSquare(1,colors[5]);
	glPopMatrix();
}

void drawWallDoor(GLfloat width, GLfloat length, GLfloat height, GLfloat length_door, GLfloat height_door,GLfloat depth_wall, Color colors[]){
	glPushMatrix();
		glTranslatef(0,0, width);
		walls(depth_wall,length/2-length_door/2, height, colors);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(length + length_door,0,width);
		walls(depth_wall,length/2 - length_door/2 ,height, colors);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(length - length_door, height-height_door , width);
		walls(0.1,length_door, height - height_door, colors);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(length+length_door , 0, width+depth_wall/2);
		glRotatef(door_angle,0,1,0);
		glTranslatef(-2*length_door , 0, 0);
		walls(0.05,length_door, height_door, colors);

	glPopMatrix();
}

void drawTable(GLfloat width, GLfloat length, GLfloat thickness, GLfloat xPos,GLfloat yPos,GLfloat zPos, Color colors[]){
	glPushMatrix();
		glTranslatef(zPos,0.8,xPos);
		walls(width,length,thickness,colors);
	glPopMatrix();

	Color x[6] = {BLACK,BLACK,YELLOW,YELLOW,BLACK,BLACK};

	glPushMatrix();
		glTranslatef(zPos+width-0.3,0,xPos+length);
		glPushMatrix();
			walls(0.1,0.3,0.4,x);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.2, 0, 0.4);
			glRotatef(90,0,1,0);
			walls(0.1,0.3,0.4,x);
		glPopMatrix();
	glPopMatrix();


}

void drawScene(){

	GLfloat width=10, length=5, height=3;
	GLfloat depth_wall=0.1,length_door=1, height_door=1;
	GLfloat table_width=1,table_length=1,table_thickness=0.05,table_xPos=17,table_yPos=1,table_zPos=7;
	Color room_colors[6]= {CYAN,CYAN,GREEN,GREEN,PURPLE,YELLOW};
	Color wall_colors[6] = {YELLOW,CYAN,PURPLE,CYAN,RED,CYAN};
	Color table_colors[6] = {RED,RED,RED,RED,RED,RED};

    if (frenteVisivel)
	    glCullFace(GL_BACK);  //glFrontFace(GL_CW);
	else
	    glCullFace(GL_FRONT);  //glFrontFace(GL_CCW);

	glPushMatrix();	
		//stair(0.25, 0.2,0,0,0,10);
		walls(width,length,height,room_colors);
		drawWallDoor(width+2,length,height, length_door,height_door,depth_wall,wall_colors);
		drawTable(table_width, table_length, table_thickness,table_xPos,table_yPos,table_zPos,table_colors);

		glColor4f(BLUE2);
		glutWireTeapot(1);		 
	glPopMatrix();
}

void display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//Apaga ecra/profundidade

	glViewport (0, 0, wScreen, hScreen);								
	glMatrixMode(GL_PROJECTION);										
	glLoadIdentity();													
	gluPerspective(angZoom, (float)wScreen/hScreen, 0.1, 3*zC);			
	glMatrixMode(GL_MODELVIEW);											
	glLoadIdentity();													

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

	case 'O':
	case 'o':
		if(door_angle>-90) door_angle -= 5;
		glutPostRedisplay();
		break;
	case 'C':
	case 'c':
		if(door_angle<0) door_angle += 5;
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
	glutCreateWindow ("|FaceVisivel:'f'| |Spectate:'Arrows'| |Enable/Disable Face-'v'| |Open/Close door - 'o|c'|");
  

	/*Define Pre defined Colors*/
	RED    = {1,0,0};
	GREEN  = {0,1,0};
	BLUE   = {0,0,1};
	YELLOW = {1,1,0}; 
	PURPLE = {1,0,1}; 
	CYAN   = {0,1,1};
	BLACK  = {0,0,0};
	inicializa();
	
	glutSpecialFunc(teclasNotAscii); 
	glutDisplayFunc(display); 
	glutKeyboardFunc(keyboard);
	
	glutMainLoop();

	return 0;
}