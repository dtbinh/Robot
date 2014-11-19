#ifndef _CUBE_H_
#define _CUBE_H_

#include "Matrix4.h"
#include "node.h"
#include "geode.h"
#include <string>

class Cube : public Geode {
public:
	Cube();
	Cube(double);
	void render();
	void setMatrix(Matrix4);
};

#endif