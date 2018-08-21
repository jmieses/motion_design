// motion.h defines all operations for a motion class

#ifndef MOTION_H
#define MOTION_H

#include <vector>

#include "DualQuaternion.h"

class Motion
{
public:
	~Motion();
	void readCtrlPositions(char *fileNameArg);
	void writePositions(char *fileNameArg);
	void drawCtrlPositions(void);
	void screwMotion(void);
		
	// New methods
	DualQuaternion Bezier(float t, Motion m);
	hPoint pointMotion(float t, Motion m, int i);
	double fac(int x);
	DualQuaternion deCasteljau(double t, Motion m);
	hPoint pointMotion(Motion m, int i);
	void affineTransformation(float t);
	Quaternion pointQuaternion(float t, Motion m, int i);
public:
	vector<DualQuaternion> ctrlPos;
	int numberOfPositions;

	// New public memmber variables
	vector<Quaternion> transVector;
	vector<hPoint> point;
	Vector L;
};

#endif
