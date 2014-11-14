#ifndef _NODE_H_
#define _NODE_H_

#include "Matrix4.h"
#include "window.h"
#include "main.h"
#include "frustum.h"
#include <string>
#include <GL/glut.h>

class Node {
protected:
	std::string Name;
	Node* parent;

public:
	Vector4 center;
	Vector4 scale;
	Node();
	Node(std::string, Node*);
	virtual void draw(Matrix4, bool,bool, Frustum) = 0;
	virtual void update() = 0;
};

#endif

