
#include "Matrix4.h"
#include "node.h"
#include <string>
#include "geode.h"


Geode::Geode() {
}

void Geode::draw(Matrix4 C){
	C = C * m;
	glMatrixMode(GL_MODELVIEW);
	Matrix4 glmatrix; 
	glColor3d(R, G, B);
	glmatrix = C;
	glmatrix.transpose();
	glLoadMatrixd(glmatrix.getPointer());
	render();
}

void Geode::setColor(double r, double g, double b) {
	R = r;
	G = g;
	B = b;
}