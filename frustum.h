#ifndef _FRUSTUM_
#define _FRUSTUM_

#include "Vector4.h"
#include "plane.h"
#include "camera.h"
class Frustum {

private:
	//Top = 0
	//Bottom = 1
	//Left = 2
	//Right = 3
	//Near = 4
	//Far = 5

	//Inside = 1
	//Intersetc = 0
	//Outside = -1

public:
	Frustum();
	Plane pl[6];
	Vector3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	double nearD, farD, ratio, angle;
	double nw, nh, fw, fh;

	void setCamInternals(double angle, double ratio, double nearD, double farD);
	void setCamDef(Vector3 p, Vector3 l, Vector3 u);
	int sphereInFrustum(Vector4 , double);
};


#endif