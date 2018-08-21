#include <iostream>
#include <fstream>
#include <vector>
#include <GL\glut.h>

#include "motion.h"
#include "Quaternion.h"
#include "hMatrix.h"
#include "hPoint.h"

using namespace std;

//-2.70000   2.00000   0.00000	0.00
//- 1.00000   0.90000   1.50000	0.00
//1.50000   1.50000   1.00000	0.00
//1.50000   3.00000 - 1.50000	0.00
// homogeneous points
hPoint p1(-2.7,2.,0.);
hPoint p2(-1.,0.9,1.5);
hPoint p3(1.5,1.5,1.);
hPoint p4(1.5,3.,-1.5);

void Motion::readCtrlPositions(char *fileNameArg)
{
	ifstream inFile(fileNameArg, ios::in);
	
	if (!inFile)
	{
		cerr<<"File" << fileNameArg << "could not be opened" << endl;
		exit(1);
	}

	int i;
	
	inFile >> numberOfPositions;
	
	Quaternion *RotationQuaternion = new Quaternion[numberOfPositions];
	Vector *TranslationVector = new Vector[numberOfPositions];

   	for(i=0; i<numberOfPositions; i++)
			inFile >> RotationQuaternion[i];
    	
    for(i=0; i<numberOfPositions; i++)
			inFile >> TranslationVector[i];
	
	// New line
	ifstream inFile2("input2.txt", ios::in);

	/*inFile2 >> numberOfPositions;
	double *skip = new double[4 * numberOfPositions];
	for (i = 0; i < 4 * numberOfPositions; i++)
		inFile2 >> skip[i];*/

	hPoint *p = new hPoint[numberOfPositions];
	for (i = 0; i < numberOfPositions; i++){
		inFile2 >> p[i];
		point.push_back(p[i]);
		
	}

	/*for (i = 0; i < numberOfPositions; i++)
		transVector.push_back(RotationQuaternion[i]);*/
	////////////////////////////////////////////////////
    for(i=0; i<numberOfPositions; i++)
    {
	   DualQuaternion dQ(RotationQuaternion[i], TranslationVector[i]);
	   ctrlPos.push_back(dQ);
	   cout<<"first position from input: "<<ctrlPos[i]<<endl;
    }
	// delete new variables.
	//delete[] skip; 
	delete[] p;
	//
    delete[] RotationQuaternion;
    delete[] TranslationVector;
    
}

void Motion::writePositions(char *fileNameArg)
{
	ofstream outFile(fileNameArg, ios::out);
	
	if (!outFile)
	{
		cerr<<"File" << fileNameArg << "could not be opened for writing" << endl;
		exit(1);
	}

	int i;
	
	outFile << numberOfPositions << endl;
	
	for(i=0; i<numberOfPositions; i++)
		outFile<<ctrlPos[i]<<endl;
}


void Motion::drawCtrlPositions(void)
{
	vector <hMatrix> homogeneousMatricesForCtrlPositions;
	for (int i=0; i<numberOfPositions; i++)
	{
		homogeneousMatricesForCtrlPositions.push_back(ctrlPos[i].dualQuaternionToHomogeneousMatrix().transpose());
		double MatrixforOpenGLStack[16];

		for (int i1=0; i1<4; i1++)
			for (int i2=0; i2<4; i2++)
				MatrixforOpenGLStack[4*i1+i2] =  homogeneousMatricesForCtrlPositions.at(i).m[i1][i2];
			
		::glPushMatrix();
		::glMultMatrixd(MatrixforOpenGLStack);
		glutSolidTeapot(0.15);
		::glPopMatrix();
	}

}



double Motion::fac(int x)
{
	if (x == 0)
		return 1;

	else
		return (x*fac(x - 1));
}

DualQuaternion Motion::Bezier(float t, Motion m){
	int r;
	
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

DualQuaternion Motion::deCasteljau(double t, Motion m)
{
	int n = m.ctrlPos.size();
	Motion v;
	v.ctrlPos = m.ctrlPos;
	for (int i = 0; i < n - 1; i++)
	{
		for (int r = 0; r < (n - 1) - i; r++)
		{
			v.ctrlPos[r] = ((1 - t)*v.ctrlPos[r]) + (t*v.ctrlPos[r+1]);
			//C->T[r].y = ((1 - t)*C->T[r].y) + (t*C->T[r + 1].y);
		}
	}
	return v.ctrlPos[0];
}

hPoint Motion::pointMotion(float t, Motion m, int i){
	DualQuaternion Q = deCasteljau(t, m);
	//hPoint p(m.transVector[i].coord[0], m.transVector[i].coord[1], m.transVector[i].coord[2]);
	//hPoint p(m.transVector[i].q[0], m.transVector[i].q[1], m.transVector[i].q[2], m.transVector[i].q[3]);
	//hPoint p(1., 2., 3.);
	hPoint p;
	switch (i){
	case 0:
		return p1.PointTransform(Q);
		break;
	case 1:
		return p2.PointTransform(Q);
		break;
	case 2:
		return p3.PointTransform(Q);
		break;
	case 3:
		return p4.PointTransform(Q);
		break;
	}
	/*if (point.size() > 0)
		return point[i].PointTransform(Q);
	else
		exit(1);*/
}

Quaternion Motion::pointQuaternion(float t, Motion m, int i){
	DualQuaternion Q = deCasteljau(t, m);
	//hPoint p(m.transVector[i].coord[0], m.transVector[i].coord[1], m.transVector[i].coord[2]);
	//hPoint p(m.transVector[i].q[0], m.transVector[i].q[1], m.transVector[i].q[2], m.transVector[i].q[3]);
	//hPoint p(1., 2., 3.);
	//hPoint p;
	double re[4] = { 1, 0, 0, 0 };
	DualQuaternion Pn;

	double du1[4] = { 0, p1.coord[0], p1.coord[1], p1.coord[2] };
	DualQuaternion P1(re, du1);
	double du2[4] = { 0, p2.coord[0], p2.coord[1], p2.coord[2] };
	DualQuaternion P2(re, du2);
	double du3[4] = { 0, p3.coord[0], p3.coord[1], p3.coord[2] };
	DualQuaternion P3(re, du3);
	double du4[4] = { 0, p4.coord[0], p4.coord[1], p4.coord[2] };
	DualQuaternion P4(re, du4);

	switch (i){
	case 0:
		Pn = Q*P1*Q.Conjugate();
		return  Pn.GetDual();
		break;
	case 1:
		Pn = Q*P2*Q.Conjugate();
		return  Pn.GetDual();
		break;
	case 2:
		Pn = Q*P3*Q.Conjugate();
		return  Pn.GetDual(); 
		break;
	case 3:
		Pn = Q*P4*Q.Conjugate();
		return  Pn.GetDual(); 
		break;
	}
	/*if (point.size() > 0)
	return point[i].PointTransform(Q);
	else
	exit(1);*/
}

hPoint Motion::pointMotion(Motion m, int i){
	//hPoint p(m.transVector[i].coord[0], m.transVector[i].coord[1], m.transVector[i].coord[2]);
	//hPoint p(m.transVector[i].q[0], m.transVector[i].q[1], m.transVector[i].q[2], m.transVector[i].q[3]);
	switch (i){
	case 0:
		return p1.PointTransform(m.ctrlPos[i]);
		break;
	case 1:
		return p2.PointTransform(m.ctrlPos[i]);
		break;
	case 2:
		return p3.PointTransform(m.ctrlPos[i]);
		break;
	case 3:
		return p4.PointTransform(m.ctrlPos[i]);
		break;
	}

	/*if (point.size() > 0)
		return point[i].PointTransform(m.ctrlPos[i]);
	else
		exit(1);*/
}

void Motion::affineTransformation(float t){
	vector <hMatrix> homogeneousMatricesForCtrlPositions;
	for (int i = 0; i < ctrlPos.size(); i++)
	{
		homogeneousMatricesForCtrlPositions.push_back(ctrlPos[i].dualQuaternionToHomogeneousMatrix().transpose());
	}
		int n = ctrlPos.size();
		Motion v;
		v.ctrlPos = ctrlPos;
		for (int i = 0; i < n - 1; i++)
		{
			for (int r = 0; r < (n - 1) - i; r++)
			{
				homogeneousMatricesForCtrlPositions.at(r) = ((1 - t)*homogeneousMatricesForCtrlPositions.at(r)) + (t*homogeneousMatricesForCtrlPositions.at(r+1));

			}
		}
		
		for (int i = 0; i < numberOfPositions; i++){
			hPoint p;
			switch (i){
				
			case 0:
				 p = homogeneousMatricesForCtrlPositions.at(0)*p1;
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_LINE);
				glVertex3f(p1.coord[0], p2.coord[1], p3.coord[2]);
				glVertex3f(p.coord[0], p.coord[1], p.coord[2]);
					glEnd();
					glFlush();
				break;
			case 1:
				 p = homogeneousMatricesForCtrlPositions.at(0)*p2;
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_LINE);
				glVertex3f(p2.coord[0], p2.coord[1], p2.coord[2]);
				glVertex3f(p.coord[0], p.coord[1], p.coord[2]);
				glEnd();
				glFlush();
				break;
			case 2:
				 p = homogeneousMatricesForCtrlPositions.at(0)*p3;
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_LINE);
				glVertex3f(p3.coord[0], p3.coord[1], p3.coord[2]);
				glVertex3f(p.coord[0], p.coord[1], p.coord[2]);
				glFlush();
				glEnd();
				break;
			case 3:
				 p = homogeneousMatricesForCtrlPositions.at(0)*p4;
				glColor3f(1.0, 0.0, 1.0);
				glBegin(GL_LINE);
				glVertex3f(p4.coord[0], p4.coord[1], p4.coord[2]);
				glVertex3f(p.coord[0], p.coord[1], p.coord[2]);
				glFlush();
				glEnd();
				break;
			}
		}
	
}

//void Motion::screwMotion(void){
//	ifstream inFile("input.txt", ios::in);
//
//	if (!inFile)
//	{
//		cerr << "File could not be opened" << endl;
//		exit(1);
//	}
//
//	inFile >> numberOfPositions;
//	for (int i = 0; i < numberOfPositions; i++)
//		inFile >> L.coord[i];
//
//
//
//	double qo = ctrlPos[0].GetReal().q[0];
//	double angle = acos(qo);
//	double wr = cos(angle / 2.);
//
//	for (int i = 0; i < numberOfPositions; i++)
//		L = L*sin(angle / 2.);
//	
//	Quaternion P(cos(angle / 2.), L.coord[0],L.coord[1],L.coord[2]);
//	double d = (2 * Q*P.Conjugate())*L;
//}
Motion::~Motion()
{
	
}