
#include "Matrix4.h"
#include "node.h"
#include <string>
#include "geode.h"
#include <iostream>

Geode::Geode() {
}

void Geode::draw(Matrix4 C, bool bound,bool cull, Frustum frust){
	Matrix4 Ctemp = C * m;
	Vector4 tempcenter = Ctemp * center;
	Vector4 tempscale = Ctemp * scale;
	glMatrixMode(GL_MODELVIEW);
	Matrix4 glmatrix; 
	glColor3d(R, G, B);
	glmatrix = Ctemp;
	glmatrix.transpose();
	glLoadMatrixd(glmatrix.getPointer());
	if (!cull) {
		render();
		if (bound) {
			glMatrixMode(GL_MODELVIEW);
			Matrix4 glMatrix;
			Matrix4 translate;
			double radius = tempscale.length();
			translate.identity();
			translate.makeTranslate(tempcenter.get(0), tempcenter.get(1), tempcenter.get(2));
			glMatrix = translate;
			glMatrix.transpose();
			glLoadMatrixd(glMatrix.getPointer());
			glutWireSphere(radius, 20, 20);
		}
	}
	else {
		if (frust.sphereInFrustum(tempcenter, tempscale.length()) != -1){
			render();
			if (bound) {
				glMatrixMode(GL_MODELVIEW);
				Matrix4 glMatrix;
				Matrix4 translate;
				double radius = tempscale.length();
				translate.identity();
				translate.makeTranslate(tempcenter.get(0), tempcenter.get(1), tempcenter.get(2));
				glMatrix = translate;
				glMatrix.transpose();
				glLoadMatrixd(glMatrix.getPointer());
				glutWireSphere(radius, 20, 20);
			}
		}
	}

}

void Geode::setColor(double r, double g, double b) {
	R = r;
	G = g;
	B = b;
}

void Geode::update() {
}
Matrix4 Geode::getMatrix() {
	return m;
}