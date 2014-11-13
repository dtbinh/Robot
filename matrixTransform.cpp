#include "node.h"
#include "Matrix4.h"
#include "group.h"
#include "matrixTransform.h"


MatrixTransform::MatrixTransform() {}

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
void MatrixTransform::draw(Matrix4 temp) {
	Matrix4 C = M * temp;
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		Node* tempNode = *it;
		tempNode->draw(C);
	}
}