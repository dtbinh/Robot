#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Matrix4.h"
#include "node.h"
#include "geode.h"
#include <string>

class Sphere : public Geode {
public:
	Sphere();
	Sphere(double);
	void render();
	void setMatrix(Matrix4);
};

#endif