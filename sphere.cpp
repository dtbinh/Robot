
#include "Matrix4.h"
#include "node.h"
#include <string>
#include "geode.h"
#include "sphere.h"

Sphere::Sphere() {

}
Sphere::Sphere(double s) {
	size = s;
	m.identity();
}
void Sphere::render() {
	glutSolidSphere(size,30,30);
}

void Sphere::setMatrix(Matrix4 temp) {
	m = temp;
}