#include "camera.h"
#include "Matrix4.h"
#include "Vector3.h"

Camera::Camera() {
	e = new Vector3(0.0, 0.0, 0.0);
	up = new Vector3(0.0, 0.0, 0.0);
	d = new Vector3(0.0, 0.0, 0.0);
	world2camera.identity();
}

void Camera::set(Vector3& a, Vector3& b, Vector3& c) {
	e = &a;
	d = &b;
	up = &c;
	Vector3 x;
	Vector3 y;
	Vector3 z;
	z = *e - *d;
	z.normalize();
	x = x.cross(*up, z);
	x.normalize();
	y = y.cross(z, x);
	y.normalize();
	for (int i = 0; i < 3; i++) {
		world2camera.set(i, 0, x.get(i));
		world2camera.set(i, 1, y.get(i));
		world2camera.set(i, 2, z.get(i));
	}
	makeInverse();
}

void Camera::makeInverse() {
	e->negate();
	Matrix4 translation;
	Matrix4 rotation;
	translation.identity();
	rotation.identity();
	translation.makeTranslate(e->getX(), e->getY(), e->getZ());
	for (int i = 0; i < 3; i++) {
		rotation.set(i, 0, world2camera.get(i, 0));
		rotation.set(i, 1, world2camera.get(i, 1));
		rotation.set(i, 2, world2camera.get(i, 2));
	}
	rotation.transpose();
	Inverted = rotation * translation;
}
Matrix4& Camera::getMatrix() {
	return world2camera;
}

Matrix4& Camera::getInverse() {
	return Inverted;
}

void Camera::move(double x, double y, double z) {
	Matrix4 mat;
	mat.makeTranslate(x, y, z);
	world2camera = mat * world2camera;
	makeInverse();
}
