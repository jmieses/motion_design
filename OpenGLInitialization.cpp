#include <windows.h>
#include <GL/glut.h>
#include <iostream>
//#include <gl/glu.h>

#include "OpenGLInitialization.h"


using namespace std;


void reshape (int w, int h)
{
//Setup Viewing Transform: This calculates windows coordinates
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   
//Setup Projection: Device coordinates
   ::glMatrixMode (GL_PROJECTION);
   ::glLoadIdentity ();
   gluPerspective(30.0f, (double)w/(double)h, 0.1f, 1000.0f);

//Set up Modeling and Viewing transform: Get Eye coordinates
   ::glMatrixMode (GL_MODELVIEW);
   ::glLoadIdentity ();
   ::glTranslatef(0.0f, 0.0f, -10.0f);
   
}


void init(void) 
{

	::glClearColor (1.0, 1.0, 1.0, 1.0);

   	GLUquadricObj *qobj = gluNewQuadric();	
	::gluQuadricDrawStyle(qobj,GLU_FILL);
	::gluQuadricNormals(qobj,GLU_SMOOTH);	


	//::glClearColor (0.0, 0.0, 0.0, 1.0);
	::glEnable(GL_DEPTH_TEST);
	::glShadeModel(GL_SMOOTH);
		// specify the back of the buffer as clear depth
	::glClearDepth( 1.0f );
	
	double eqn[] = {0.01f,0.0f,0.01f,-1.0f};
	// enable clip plane	
	::glClipPlane(GL_CLIP_PLANE0,eqn);

}


void setupLight(void)
{
	
	float LightAmbient[]	= { 1.0f, 1.0f, 1.0f, 1.0};
	float LightDiffuse[]	= { 0.7f, 0.7f, 0.7f, 1.0f};
	float LightSpecular[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
	float LightPosition[]	= { 3.0f, 3.0f, 3.0f, 1.0f };

	float RedSurface[]   = { 1.0f, 0.0f, 0.0f, 1.0f};
	float GreenSurface[] = { 0.0f, 1.0f, 0.0f, 1.0f};
	float BlueSurface[]  = { 0.0f, 0.0f, 1.0f, 1.0f};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);	

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	float no_mat[]={0.0f,0.0f,0.0f,1.0f};
    float mat_ambient[]={0.7f,0.7f,0.7f,1.0f};
    float mat_ambient_color[]={0.8f,0.3f,0.7f,1.0f};
    float mat_diffuse[]={0.7f,0.7f,0.7f,1.0f};
    
    float mat_specular[]={0.7f,0.7f,0.7f,1.0f};

    float no_shininess[]={0.0};
    float low_shininess[]={5.0};
    float high_shininess[]={100.0};
    float mat_emission[]={0.2f,0.1f,0.1f,0.0f};

	glMaterialfv(GL_FRONT,GL_AMBIENT,no_mat);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,high_shininess);
    glMaterialfv(GL_FRONT,GL_EMISSION,mat_emission); 
	//glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);

}
