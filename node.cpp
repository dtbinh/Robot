#include "node.h"
Node::Node() {}
Node::Node(std::string n, Node* p) {
	Name = n;
	parent = p;
}