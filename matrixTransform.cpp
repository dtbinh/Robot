#include "node.h"
#include "Matrix4.h"
#include "group.h"
#include "matrixTransform.h"


MatrixTransform::MatrixTransform() {
	M.identity();
}

MatrixTransform::MatrixTransform(std::string n,Node* p, Matrix4 temp) {
		M = temp;
		Name = n;
		parent = p;
}
MatrixTransform::MatrixTransform( Matrix4 temp) {
	M = temp;
}

void MatrixTransform::set(Matrix4 temp) {
	M = temp;
}

Matrix4 MatrixTransform::get() {
	return M;
}

void MatrixTransform::draw(Matrix4 temp, bool wire,bool cull, Frustum frum) {
	Matrix4 C = temp* M;
	double x = 0;
	double y = 0;
	double z = 0;
	double radius = 0;
	int number = 0;
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		Node* tempNode = *it;
		number++;
		tempNode->draw(C, wire,cull, frum);
		if (radius < tempNode->scale.length()) {
			radius = tempNode->scale.length();
		}
		x = x + tempNode->center.get(0);
		y = y + tempNode->center.get(1);
		z = z + tempNode->center.get(2);
	}
	x = x / number;
	y = y / number;
	z = z / number;
	/*
	if (wire) {
		glMatrixMode(GL_MODELVIEW);
		Matrix4 glMatrix;
		Matrix4 translate;
		translate.identity();
		translate.makeTranslate(x,y,z);
		glMatrix = translate;
		glMatrix.transpose();
		glLoadMatrixd(glMatrix.getPointer());
		glutWireSphere(radius, 20, 20);
	}*/
}
void MatrixTransform::update() {
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		Node* tempNode = *it;
		tempNode->update();
	}
}
