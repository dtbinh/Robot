#include <list>
#include "node.h"
#include "Matrix4.h"
#include "group.h"


Group::Group(){
	M.identity();
}

Group::Group(std::string n) {
	Name = n;
}

Group::Group(std::string n, Node* p) {
	Name = n;
	parent = p;
}
void Group::addChild(Node* temp){
	children.push_back(temp);
}
void Group::deleteChild(Node* temp){
	children.remove(temp);
}
void Group::draw(Matrix4 C, bool bound,bool cull, Frustum frum) {
	Matrix4 tempM = M * C;
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		Node* temp = *it;
		temp->draw(tempM, bound,cull, frum);
	}
}

Node* Group::getChild(int i) {
	std::list<Node*>::iterator it = children.begin();
	Node* temp = *it + i;
	return temp;
}

void Group::setMatrix(Matrix4 temp) {
	M = temp;
}
void Group::update() {
	for (std::list<Node*>::iterator it = children.begin(); it != children.end(); ++it) {
		Node* tempNode = *it;
		tempNode->update();
	}
}