//================================================================
// File: tp02.c
// Date: 18.01.2005
// Auth: Mounir Elbaz <Mounir.ELbaz@uha.fr>
//
// Objectif : afficher a l'ecran un polygone en couleur 
//================================================================

// inclusion des fichiers d'en-tete Glut  			

#include <iostream>
using namespace std;

#include <glut.h>

const int LARG=500;
const int HAUT=500;


void affichage()
{
// effacement de l'image avec la couleur de fond 
glClear(GL_COLOR_BUFFER_BIT);
// Dessin du polygone 
glBegin(GL_POLYGON);
	glColor3f(1.0,1.0,0.0);
	glVertex2f(0.0,-0.75);
	glColor3f(0.0,1.0,0.0);
	glVertex2f(0.5,-0.5);
	glColor3f(0.0,1.0,1.0);
	glVertex2f(0.75,0.0);
	glColor3f(0.0,0.0,1.0);
	glVertex2f(0.5,0.5);
	glColor3f(1.0,0.0,1.0);
	glVertex2f(0.0,0.75);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(-0.5,0.5);
	glColor3f(0.0,0.0,0.0);
	glVertex2f(-0.75,0.0);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(-0.5,-0.5);
glEnd();
// on force l'affichage du resultat 
glFlush();
}

void clavier(unsigned char touche,int x,int y)
{
switch (touche)
{
	case 'p': // affichage du carre plein 
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glutPostRedisplay();
	break;
	case 'f': // affichage en mode fil de fer 
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glutPostRedisplay();
	break;
	case 's' : // Affichage en mode sommets seuls 
	glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	glutPostRedisplay();
	break;
	case 'q' : exit (0); //la touche 'q' permet de quitter le programme 
}
}
int main(int argc,char **argv)
{

	cout << "        p: affichage du polygone plein " << endl;
	cout <<"        f: affichage en mode fil de fer " << endl;
	cout <<"        s: affichage en mode sommets seuls " << endl;
	cout <<"        q: quitter le programme " << endl;

// initialisation de glut et creation de la fenetre 

glutInit(&argc,argv);
glutInitDisplayMode(GLUT_RGB);
glutInitWindowPosition(400,200);
glutInitWindowSize(LARG,HAUT);
glutCreateWindow("Test des touche du clavier");

// Initialisation d'OpenGL 

glClearColor(0.0,0.0,0.0,0.0);
glColor3f(1.0,1.0,1.0);
glPointSize(2.0);

// enregistrement des fonctions de rappel 

glutDisplayFunc(affichage);
glutKeyboardFunc(clavier);

// Entree dans la boucle principale glut 

glutMainLoop();
return 0;
}
