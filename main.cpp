#include <iostream>
#include <windows.h>
#include <vector>
#include <GL\glut.h>

#include "motion.h"
#include "OpenGLInitialization.h"
#include "Interaction.h"
#include "color.h"
#include <math.h>
//#include "hPoint.h"

using namespace std;

void draw(int, Motion);
void crossProduct(double vect_A[], double vect_B[], double cross_P[]);
DualQuaternion setDualQuaternion(double angle, vector<double> translation);
DualQuaternion Bezier(double t, Motion m);
double fac(int x);


void display(void)
{
	static Motion m;
	m.readCtrlPositions("input.txt");

	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	::glPushMatrix();
	::glTranslated(Interaction::m_xTrans, Interaction::m_yTrans, Interaction::m_zTrans);
	::glRotated(Interaction::m_xRotate, 1.0, 0.0, 0.0);
	::glRotated(Interaction::m_yRotate, 0.0, 1.0, 0.0);
	::glRotated(Interaction::m_zRotate, 0.0, 0.0, 1.0);

	::glMaterialfv(GL_FRONT, GL_DIFFUSE, BROWN);
	m.drawCtrlPositions();
	::glMaterialfv(GL_FRONT, GL_DIFFUSE, BLUE);
	draw(GL_RENDER,m);
	
	::glPopMatrix();

	::glutSwapBuffers();
}

void draw(int mode, Motion m){
	//glClear(GL_COLOR_BUFFER_BIT);
	/*glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(0.2, 0.2, 0.5);
		glVertex3f(0.8, 0.2, 1.0);
		glVertex3f(0.2, 0.5, 0.7);
		glVertex3f(0.8, 0.5, 0.0);
		glVertex3f(0.2, 0.8, 0.0);
		glVertex3f(0.8, 0.8, 0.0);
	glEnd();*/

	double t;
	Motion vv;
	hPoint p(-2.7, 2., 0.);
	//hPoint p(m.transVector[0].coord[0], m.transVector[0].coord[1], m.transVector[0].coord[2]);
	hPoint pp;

	for (int i = 0; i < m.numberOfPositions; i++){
		for (t = 0; t <= 1; t = t + 0.01)
		{
			pp = vv.pointMotion(t,m, i);	
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(p.coord[0], p.coord[1], p.coord[2]);
				glVertex3f(pp.coord[0], pp.coord[1], pp.coord[2]);
				//glVertex3f(1. + 10 * t, 2. + 10 * t, 3. + 10 * t);
				p = pp;
				glEnd();
		}
	}
	Quaternion q(0,-2.7, 2., 0.);
	Quaternion qo;
	Motion vo;
	for (int i = 0; i < m.numberOfPositions; i++){
		for (t = 0; t <= 1; t = t + 0.01)
		{
			qo = vo.pointQuaternion(t, m, i);
			glColor3f(1.0, 0.0, 1.0);
			glBegin(GL_LINES);
			glVertex3f(q.q[1], q.q[2], q.q[3]);
			glVertex3f(qo.q[1], qo.q[2], qo.q[3]);

			//glVertex3f(1. + 10 * t, 2. + 10 * t, 3. + 10 * t);
			q = qo;
			glEnd();
		}
	}

	/*for (t = 0; t <= 1; t = t + 0.01){
		m.affineTransformation(t);

	}*/
}




int main (int argc, char *argv[])
{
	
	//Initialize Default OpenGL window 
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
   glutInitWindowSize (800, 600); 
   glutInitWindowPosition (0, 0);
   int win_id = glutCreateWindow("Motion Design Skeleton Code");
   
   init();
   setupLight();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(Interaction::mouseButtonEvents);
   glutMotionFunc(Interaction::mouseMotionEvents);
   glutKeyboardFunc(Interaction::keyboard);

   glutMainLoop();

	return 0;
}

//void setDualQuaternion(double angle, double l[2], double p[2]){
//	vector<double> vr, vd;
//	double m[2];
//	double s = sin(angle / 2);
//	double c = cos(angle / 2);
//	crossProduct(p, l, m);
//	// screw parameters to dual quaternion
//	double wr = c;  // real part of a dual quaternion
//	for (int i = 0; i < 3; i++)
//		vr.push_back(l[i] * s);
//
//
//	double wd = -(d / 2.)*s;
//	for (int i = 0; i < 3; i++)
//		vd.push_back(s*m[i] + (d / 2.)*c*l[i]);
//
//}

// The cross product of two vector arrays
void crossProduct(double vect_A[], double vect_B[], double cross_P[])
{
	cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
	cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
	cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

DualQuaternion setDualQuaternion(double angle, vector<double> translation){
	double g = 0;
	Quaternion qreal(cos(angle / 2.), sin(angle / 2.), g*sin(angle / 2.), g*sin(angle / 2.));
	Quaternion qdual(0, translation.at(0), translation.at(1), translation.at(2));
	DualQuaternion Q(qreal, qdual);
	return Q;
}

DualQuaternion Bezier(double t, Motion m){
	int r;
	vector<double> transVector = { Interaction::m_xTrans, Interaction::m_yTrans, Interaction::m_zTrans };
	DualQuaternion temp;

	//temp = setDualQuaternion(Interaction::m_xRotate, transVector);
	int n = m.ctrlPos.size() - 1;
	for (r = 0; r <= n; r++)
	{
		temp = temp + m.ctrlPos[r] * (fac(n) / (fac(r) * fac(n - r)))*(pow((1 - t), (n - r)))*pow(t, r);
		//temp.y += C->B[r].y * (fac(n) / (fac(r) * fac(n - r)))*(pow((1 - t), (n - r)))*pow(t, r);
	}
	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////
double fac(int x)
{
	if (x == 0)
		return 1;

	else
		return (x*fac(x - 1));
}
