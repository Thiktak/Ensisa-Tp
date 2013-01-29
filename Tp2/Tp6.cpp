//================================================================
// File: tp05.c
// Date: 18.01.2005
// Auth: Mounir Elbaz <Mounir.ELbaz@uha.fr>
//
// Exemple pour l'�clairage  
//================================================================
#include <stdlib.h>
#include <glut.h>
#include <math.h>

#include <iostream>
using namespace std;


const int LARG=500;
const int HAUT=500;
int id_sphere;

GLfloat clipHeight;
GLfloat clipWidth;

void Init_light()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_LIGHTING);									
	glEnable(GL_LIGHT0); //ca y est.
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT); //GL_FLAT, GL_SMOOTH
//	gluLookAt (0.0, -10.0, 0.0 ,0.0 , 0.0, 0.0, 0.0, 0.0, 1.0);
	GLfloat light0_position [] = {100.0f, -50.0f, 200.0f, 0.0f}; // position de la source à "l'infini"
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

}					

void  affichage(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLfloat rouge[] = { 1.0, 0.0, 0.0, 0.0 };
	GLfloat blue[]  = { 0.0, 1.0, 0.0, 0.0 };
	GLfloat blanc[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
	//glutSolidSphere(50.0,50,50);
	//glTranslatef(10, 50, 10); // déplacement

	
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
		//glScalef(10.0, 10.0, 10.0);
		//glCallList(id_sphere);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
		glRotatef(20, 1, 0, 0);
		glTranslatef(100, 0, 80);
		glutSolidTeapot(10);
	glPopMatrix();
	
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, blanc);
	
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rouge); // GL_FRONT_AND_BACK
//		glTranslatef(0, 0, 0);
		glScalef(2.0, 2.0, 1.0);
		
		glCallList(id_sphere);
	glPopMatrix();

	// change 100->50 glutSolidSphere(50.0, 100, 100);
  	glutSwapBuffers();
}

void Reshape(GLsizei w, GLsizei h)
{
	GLfloat Near = -100.0f;
	GLfloat Far = 100.0f;

	if (h == 0) h = 1;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLfloat)-LARG/4.0,(GLfloat)LARG/4.0,(GLfloat)-HAUT*h/(4.0*w),(GLfloat)HAUT*h/(4.0*w), Near, Far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int creer_sphere() {
	id_sphere = glGenLists(1);
	glNewList(id_sphere, GL_COMPILE);
		glutSolidSphere(5.0, 10, 10);
	glEndList();

	return id_sphere;
}
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(LARG,HAUT);
	glutInitWindowPosition(200,200);
	glutCreateWindow("Eclairage");
	Init_light(); // activation de l'éclairage
	creer_sphere();

	glutDisplayFunc(affichage);
	glutReshapeFunc(Reshape);
	glutMainLoop();


	return 0;
}
