
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
	center.set(0, 0, 0, 1);
	scale.set(s, s, s, 0);
}
void Sphere::render() {
	glutSolidSphere(size,30,30);
}

void Sphere::setMatrix(Matrix4 temp) {
	m = temp;
}
void Sphere::update() {

}
