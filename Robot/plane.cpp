

//Based on the files in lightouse
#include "Plane.h"
#include <iostream>

Plane::Plane(Vector3 v1, Vector3 v2, Vector3 v3) {

	set3Points(v1, v2, v3);
}


Plane::Plane() {}
void Plane::set3Points(Vector3 v1, Vector3 v2, Vector3 v3) {


	Vector3 temp;
	Vector3 altTemp;

	temp = v1 - v2;
	altTemp = v3 - v2;

	normal = temp.cross(altTemp,temp);
	normal.normalize();
	point = v2;
	d = -(normal.dot(point, normal));
}

void Plane::setNormalAndPoint(Vector3 n, Vector3 p) {

	normal = n;
	point = p;
	d = -(normal.dot(point, normal));
}


double Plane::distance(Vector3 p) {

	return (d + normal.dot(p, normal));
}
void Plane::print() {

	printf("Plane("); normal.print(""); printf("# %f)", d);
	std::cout << std::endl;
}
