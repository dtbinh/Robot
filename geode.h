#ifndef _GEODE_H_
#define _GEODE_H_

#include "Matrix4.h"
#include "node.h"
#include <string>

class Geode : public Node {
protected:
	double size;
	double R, G, B;
	Matrix4 m;
public:
	Geode();
	void draw(Matrix4 C, bool,bool, Frustum);
	virtual void render() = 0;
	void setColor(double, double, double);
	virtual void setMatrix(Matrix4) = 0;
	void update();
};

#endif