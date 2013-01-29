//================================================================
// File: menu.cpp// Date: 18.01.2008
// Auth: Mounir Elbaz <Mounir.ELbaz@uha.fr>
//
// Exemple pour l'eclairage, l'utilisation de la souris du menu
//================================================================
#include <stdlib.h>
#include <glut.h>
#include <math.h>

#include <iostream>
using namespace std;

enum {
  TRANS_SPHERE,
	TRANS_CUBE,
	TRANS_CONE,
	ROTATION,
	INITIALISATION,
	MENU_EXIT
};
const int LARG=500;
const int HAUT=500;
const float rapport=4.0f;

GLfloat clipHeight;
GLfloat clipWidth;
GLfloat Near = -500.0f;
GLfloat Far = 500.0f;

int objet;

GLint anglex=0, angley=0,  xold,yold;
GLfloat tspherex=0.0,tcubex=0.0, tconex=0.0,tspherey=0.0,tcubey=0.0, tconey=0.0;
GLubyte presse;
void Init_light()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_LIGHTING);									
	glEnable(GL_LIGHT0);									//ca y est.
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	gluLookAt (0.0, -10.0, 0.0 ,0.0 , 0.0, 0.0, 0.0, 0.0, 1.0);
	GLfloat light0_position [] = {100.0f, -100.0f, 100.0f, 0.0f};
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

}	

void  affichage(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushMatrix();
		glRotatef(angley,1.0,0.0,0.0);
		glRotatef(anglex,0.0,1.0,0.0);
	glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f((GLfloat)LARG/rapport,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,(GLfloat)HAUT/rapport,0.0);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,Far);
	glEnd();
		glPushMatrix();
			glTranslatef(tspherex,tspherey,0.0);
			GLfloat vert[] = { 0.0,0.9,0.0,0.0 };
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,vert);	
			glutSolidSphere(20.0,100,100);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(tcubex,tcubey,0.0);
			GLfloat rouge[] = { 0.9,0.0,0.0,0.0 };
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,rouge);	
			glutSolidCube(20.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(tconex,tconey,0.0);
			GLfloat bleu[] = { 0.0,0.0,0.9,0.0 };
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,bleu);	
			glutSolidCone(20.0,40.0,100,100);
		glPopMatrix();
	glPopMatrix();
//	glFlush();
  
	glutSwapBuffers();
}

void Reshape(GLsizei w, GLsizei h)
{

	if (h == 0) h = 1;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLfloat)-LARG/rapport,(GLfloat)LARG/rapport,(GLfloat)-HAUT*h/(rapport*w),(GLfloat)HAUT*h/(rapport*w), Near, Far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
/**************************************************************************/

void mouse(int button, int state, int x, int y)
{
	/* si on appuie sur le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		presse = 1; /* le booleen presse passe a 1 (vrai) */
		xold = x; /* on sauvegarde la position de la souris */
		yold=y;
	}
	/* si on relache le bouton gauche */
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse=0; /* le booleen presse passe a 0 (faux) */
}
void mousemotion(int x,int y)
{
	if (presse) /* si le bouton gauche est presse */
	{
		/* on modifie les angles de rotation de l'objet
		en fonction de la position actuelle de la souris et de la derniere
		position sauvegardee */
		switch (objet) 
		{
		case 1 : tspherex+=2.0*(x-xold)/rapport;tspherey-=2.0*(y-yold)/rapport;
				 break;
		case 2 : tcubex+=2.0*(x-xold)/rapport;tcubey-=2.0*(y-yold)/rapport;
				 break;
		case 3 : tconex+=2.0*(x-xold)/rapport;tconey-=2.0*(y-yold)/rapport;
				 break;
		case 4 : anglex+=(x-xold); if (anglex >= 360) anglex-=360;
				 angley+=(y-yold); if (angley >= 360) angley-=360;
				 break;
		case 5 : tspherex=0;tcubex=0;tconex=0;tspherey=0;tcubey=0;tconey=0;anglex=0;angley=0;
		}
		
		glutPostRedisplay(); /* on demande un rafraichissement de l'affichage */
	}
	xold=x; /* sauvegarde des valeurs courante de le position de la souris */
	yold=y;
}

void SelectFromMenu(int idCommand)
{
	switch (idCommand)
		{
		case TRANS_SPHERE: objet=1;
			break;		 
		case TRANS_CUBE: objet=2;
			break;		 
		case TRANS_CONE: objet=3;
			break;		 
		case ROTATION: objet=4;
			break;		 
		case INITIALISATION: objet=5;
			break;		 

		case MENU_EXIT:
			exit (0);
			break;
		}

	// Almost any menu selection requires a redraw
	glutPostRedisplay();
}
int BuildPopupMenu (void)
{
	int menu;

	menu = glutCreateMenu (SelectFromMenu);
	glutAddMenuEntry ("Translater Sphere", TRANS_SPHERE);
	glutAddMenuEntry ("Translater Cube", TRANS_CUBE);
	glutAddMenuEntry ("Translater Cone", TRANS_CONE);
	glutAddMenuEntry ("Rotation", ROTATION);
	glutAddMenuEntry ("Initialisation", INITIALISATION);
	glutAddMenuEntry ("Exit demo\tEsc", MENU_EXIT);

	return menu;
}

/**************************************************************************/
int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(LARG,HAUT);
	glutInitWindowPosition(200,200);
	glutCreateWindow("Eclairage");
	Init_light();
	glutDisplayFunc(affichage);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	BuildPopupMenu ();
	glutAttachMenu (GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}
