
#include "Matrix4.h"
#include "node.h"
#include <string>
#include "geode.h"
#include "cube.h"


Cube::Cube() {

}
Cube::Cube(double s) {
	center.set(0, 0, 0, 1);
	scale.set(s*0.5, s*0.5, s*0.5,0);
	size = s;
	m.identity();
}
void Cube::render() {
	glutSolidCube(size);
}

void Cube::setMatrix(Matrix4 temp) {
	m = temp;
}
