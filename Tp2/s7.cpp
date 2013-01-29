#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#define PI 3.14159265

// CF, see `glOrtho` pour voir l'ortho de la scène & projection


int angle=45;
float Sin[360],Cos[360];
GLfloat L0pos[]={ 0.0,2.0,-1.0};
//GLfloat L0dif[]={ 0.3,0.3,0.8};
GLfloat L0dif[]={ 1.0,0.0,0.0};
GLfloat L1pos[]={ 2.0,2.0,2.0};
GLfloat L1dif[]={ 0.0,0.0,1.0};
GLfloat Mspec[]={0.5,0.5,0.5};
GLfloat Mshiny=50;

void affichage(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(4.5*Cos[angle],2.0,4.5*Sin[angle],0.0,0.0,0.0,0.0,1.0,0.0);
	glLightfv(GL_LIGHT0,GL_POSITION,L0pos);
	glLightfv(GL_LIGHT1,GL_POSITION,L1pos);
	glutSolidSphere(1.0,50,50);
	glutSwapBuffers();
}
void reshape(int x,int y)
{
	if (x<y)
		glViewport(0,(y-x)/2,x,x);
	else
		glViewport((x-y)/2,0,y,y);
}
void clavier(unsigned char touche,int x,int y)
{
	switch (touche)
	{
	case 'z' : /* increment de l'angle de position */
		angle+=2;
		if (angle>=360)
			angle-=360;
		glutPostRedisplay();
		break;
	case 'a' : /* decrement de l'angle de position */
		angle-=2;
		if (angle<0)
			angle+=360;
		glutPostRedisplay();
		break;
	case 'w' : /* Lampe 0 on */
		glEnable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'x' : /* Lampe 0 off */
		glDisable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'c': /* lampe 1 on */
		glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case 'v': /* lampe 1 off */
		glDisable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case 'm': /* increment reflexion speculaire */
		Mspec[0]+=0.1;
		if (Mspec[0]>1)
			Mspec[0]=1;
		Mspec[1]+=0.1;
		if (Mspec[1]>1)
			Mspec[1]=1;
		Mspec[2]+=0.1;
		if (Mspec[2]>1)
			Mspec[2]=1;
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec);
		glutPostRedisplay();
		break;
	case 'l': /* decrement reflexion speculaire */
		Mspec[0]-=0.1;
		if (Mspec[0]<0)
			Mspec[0]=0;
		Mspec[1]-=0.1;
		if (Mspec[1]<0)
			Mspec[1]=0;
		Mspec[2]-=0.1;
		if (Mspec[2]<0)
			Mspec[2]=0;
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec);
		glutPostRedisplay();
		break;
	case 'j': /* increment de la brillance */
		Mshiny-=1;
		if (Mshiny<0)
			Mshiny=0;
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mshiny);
		glutPostRedisplay();
		break;
	case 'k': /* decrement de la brillance */
		Mshiny+=1;
		if (Mshiny>128)
			Mshiny=128;
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mshiny);
		glutPostRedisplay();
		break;
	case 'f' :
			glutFullScreen();
		glutPostRedisplay();
	break;
	case 27 :
		exit(0);

	}
}

void calcTableCosSin()
{
	/* calcul du tableau des sinus et cosinus */
	int i;
	for (i=0;i<360;i++) {
		Cos[i]=cos(((float)i)/90.0*PI);
		Sin[i]=sin(((float)i)/180.0*PI);
	}
}
int main(int argc,char **argv)
{
	
	printf("z : increment de l'angle de position \n");
	printf("a : decrement de l'angle de position  \n");
	printf("w : Lampe 0 on \n");
	printf("x : Lampe 0 off \n");
	printf("c : lampe 1 on \n");
	printf("v : lampe 1 off \n");
	printf("m : increment reflexion speculaire \n");
	printf("l : decrement reflexion speculaire \n");
	printf("j : increment de la brillance \n");
	printf("k : decrement de la brillance \n");	
	printf("f : plein ecran \n");
	
	/* initialisation de glut et creation
	de la fenetre */
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(400,400);
	glutCreateWindow("light1");
	/* pr?calcul de la table des sinus et cosinus */
	calcTableCosSin();
	/* Initialisation d'OpenGL */
	glClearColor(0.0,0.0,0.0,0.0);
	glColor3f(1.0,0.0,00);
	glEnable(GL_DEPTH_TEST);
	/* Param?trage des lumi?res */
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,L0dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,L0dif);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,L1dif);
	glLightfv(GL_LIGHT1,GL_SPECULAR,L1dif);
	/* Param?trage du mat?riau */
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Mspec);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Mshiny);
	/* Mise en place de la perspective */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,1.0,0.1,10.0);
	glMatrixMode(GL_MODELVIEW);
	/* Mise en place des fonctions de rappel */

	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	/* Entree dans la boucle principale */
	glutMainLoop();
	return 0;
}