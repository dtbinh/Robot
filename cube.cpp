
#include "Matrix4.h"
#include "node.h"
#include <string>
#include "geode.h"
#include "cube.h"


Cube::Cube() {

}
Cube::Cube(double s) {
	size = s;
	m.identity();
}
void Cube::render() {
	glutSolidCube(size);
}

void Cube::setMatrix(Matrix4 temp) {
	m = temp;
}