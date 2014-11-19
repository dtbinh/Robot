#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Matrix4.h"
#include "Vector3.h"

class Camera
{
protected:
	Matrix4 world2camera;            // model matrix (transforms model coordinates to world coordinates)
	Matrix4 Inverted;


public:
	Camera();   // Constructor
	Matrix4& getMatrix();	
	Vector3*  e;
	Vector3* d;
	Vector3* up;
	void set(Vector3&, Vector3&, Vector3&);
	Matrix4& getInverse();
	void move(double, double, double);
	void makeInverse();
	void setMatrix(Matrix4);
};

#endif

