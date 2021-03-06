#ifndef _GROUP_H_
#define _GROUP_H_
#include <list>
#include "node.h"
#include "Matrix4.h"

class Group : public Node {
protected:
	std::list < Node* > children;
	Matrix4 M;
public:
	Group();
	Group(std::string);
	Group(std::string, Node*);
	void addChild(Node*);
	void deleteChild(Node*);
	Node* getChild(int);
	void setMatrix(Matrix4 m);
	virtual void draw(Matrix4 C, bool,bool, Frustum);
	virtual void update();
};

#endif