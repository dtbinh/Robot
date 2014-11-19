
//Based on the files in lightouse
#ifndef _PLANE_
#define _PLANE_

#include "Vector3.h"

class Plane
{

public:

	Vector3 normal, point;
	double d;


	Plane::Plane(Vector3 , Vector3, Vector3 );
	Plane::Plane();

	void set3Points(Vector3 , Vector3 , Vector3 );
	void setNormalAndPoint(Vector3 , Vector3) ;
	double distance(Vector3);
	void print();

};


#endif