#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include "Window.h"
#include "object.h"
#include "Matrix4.h"
#include "main.h"
#include "node.h"
#include "sphere.h"
#include "cube.h"
#include "matrixTransform.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
static double degree = 1;
static bool forw = true;
bool create = true;

//Main Group
Group *world = new Group();

//Rotation Matrix Transforms
MatrixTransform *LArmRot = new MatrixTransform();
MatrixTransform *RArmRot = new MatrixTransform();
MatrixTransform *LLegRot = new MatrixTransform();
MatrixTransform *RLegRot = new MatrixTransform();

//Placement Matrix Transform
MatrixTransform *mtHead = new MatrixTransform();
MatrixTransform *mtLEye = new MatrixTransform();
MatrixTransform *mtREye = new MatrixTransform();
MatrixTransform *mtMouth = new MatrixTransform();
MatrixTransform *mtLArm = new MatrixTransform();
MatrixTransform *mtRArm = new MatrixTransform();
MatrixTransform *mtLLeg = new MatrixTransform();
MatrixTransform *mtRLeg = new MatrixTransform();

//Body Parts
Cube *Torso = new Cube(4);
Cube *Head = new Cube(2);
Cube *Eye = new Cube(0.5);
Cube *Mouth = new Cube(0.5);
Cube *Arm = new Cube(1);
Cube *Leg = new Cube(1);


//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
  displayCallback();         // call display routine to show the object
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
  cerr << "Window::reshapeCallback called" << endl;
  width = w;
  height = h;
  glViewport(0, 0, w, h);  // set new viewport size
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); // set perspective projection viewing frustum
  glTranslatef(0, 0, -20);    // move camera back 20 units so that it looks at the origin (or else it's in the origin)
  glMatrixMode(GL_MODELVIEW);
}

void Window::load() {
	Matrix4 temp;
	Matrix4 altTemp;

	//Torso
	Torso->setColor(1, 1, 1);
	world->addChild(Torso);

	//Head
	altTemp.makeTranslate(0, 3, 0);
	mtHead->set(altTemp);
	Head->setColor(1, 0, 0);
	mtHead->addChild(Head);
	//Eyes
	altTemp.makeTranslate(-0.6, 0.25, 2);
	mtLEye->set(altTemp);
	Eye->setColor(1, 1, 0);
	mtLEye->addChild(Eye);
	mtHead->addChild(mtLEye);
	altTemp.makeTranslate(0.6, 0.25, 2);
	mtREye->set(altTemp);
	mtREye->addChild(Eye);
	mtHead->addChild(mtREye);
	//Mouth
	altTemp.makeTranslate(0, -0.5, 2);
	mtMouth->set(altTemp);
	altTemp.makeScale(2, 1, 1);
	Mouth->setMatrix(altTemp);
	Mouth->setColor(1, 1, 0);
	mtMouth->addChild(Mouth);
	mtHead->addChild(mtMouth);
	world->addChild(mtHead);

	//Left Arm
	altTemp.makeTranslate(-2.5, 0, 0);
	/*
	if (forw) {
	altTemp.makeRotate(degree, Vector3(0,1,0));
	temp = temp *altTemp;
	degree = degree + 1;
	if (degree == 40)
	forw = !forw;
	}
	if (!forw && degree > -45) {
	altTemp.makeRotateX(-degree);
	temp = temp *altTemp;
	degree = degree - 1;
	if (degree == -40)
	forw = !forw;
	}
	*/
	mtLArm->set(altTemp);
	Arm->setColor(0, 1, 0);
	altTemp.makeScale(1, 4, 1);
	Arm->setMatrix(altTemp);
	LArmRot->addChild(Arm);
	mtLArm->addChild(LArmRot);
	world->addChild(mtLArm);

	//Right Arm
	altTemp.makeTranslate(2.5, 0, 0);
	mtRArm->set(altTemp);
	RArmRot->addChild(Arm);
	mtRArm->addChild(RArmRot);
	world->addChild(mtRArm);

	//Left Leg
	altTemp.makeTranslate(-1, -4, 0);
	mtLLeg->set(altTemp);
	Leg->setColor(0, 0, 1);
	altTemp.makeScale(1, 4, 1);
	Leg->setMatrix(altTemp);
	LLegRot->addChild(Leg);
	mtLLeg->addChild(LLegRot);
	world->addChild(mtLLeg);

	//Right Leg
	altTemp.makeTranslate(1, -4, 0);
	mtRLeg->set(altTemp);
	RLegRot->addChild(Leg);
	mtRLeg->addChild(RLegRot);
	world->addChild(mtRLeg);

}
//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
  // Tell OpenGL what ModelView matrix to use:
  Matrix4 altTemp;
  Matrix4 temp;
  Matrix4 altTemp1;
  if (forw) {
	  altTemp.makeRotateX(degree);
	  altTemp1.makeRotateX(-degree);
	  degree = degree + 1;
	  if (degree == 20)
		  forw = !forw;
  }
  else {
	  altTemp.makeRotateX(degree);
	  altTemp1.makeRotateX(-degree);
	  degree = degree - 1;
	  if (degree == -20)
		  forw = !forw;
  }
  temp = altTemp;
  altTemp.makeTranslate(0, -2, 0);
  altTemp1 = altTemp1 * altTemp;
  temp = temp * altTemp;
  altTemp.makeTranslate(0, 2, 0);
  temp = altTemp * temp;
  altTemp1 = altTemp * altTemp1;
  LArmRot->set(temp);
  RLegRot->set(temp);
  RArmRot->set(altTemp1);
  LLegRot->set(altTemp1);
  world->draw(Globals::object.getMatrix());
  glFlush();  
  glutSwapBuffers();
}
//----------------------------------------------------------------------------
// Callback method called by GLUT when users press specific keys on the keyboard
void Window::keyboardCallback(unsigned char key, int x, int y) {
	switch (key) {
	  // toggles direction of spin between clockwise and counterclockwise
	  case 't' :
		  Globals::object.flipRotation();
		  break;
      // moves object left
	  case 'X' :
		  Globals::object.move(1, 0, 0);
		  break;
	  // moves object right
	  case 'x' :
		  Globals::object.move(-1, 0, 0);
		  break;
	  // moves object down
	  case 'Y':
		  Globals::object.move(0, 1, 0);
		  break;
	  // moves object up
	  case 'y':
		  Globals::object.move(0, -1, 0);
		  break;
	  // moves object towards screen
	  case 'Z':
		  Globals::object.move(0, 0, -1);
		  break;
	  // moves object away from screen
	  case 'z':
		  Globals::object.move(0, 0, 1);
		  break;
	  // resets object position
	  case 'r':
		  Globals::object.reset();
		  break;
	  // orbits object counterclockwise
	  case 'O':
		  Globals::object.orbit(true);
		  break;
	  // orbits object clockwise
	  case 'o':
		  Globals::object.orbit(false);
		  break;
	  // scales object down
	  case 'S':
		  Globals::object.scale(10.0/11.0);
		  break;
	  // scales object up
	  case 's':
		  Globals::object.scale(11.0/10.0);
		  break;
	  case 'q':
		  Globals::object.spin(5.0);
		  break;
	  case 'Q':
		  Globals::object.spin(-5.0);
		  break;
	  case 'w':
		  Globals::object.spinX(5.0);
		  break;
	  case 'W':
		  Globals::object.spinX(-5.0);
		  break;
	}
}
void Window::funcKeyboardCallback(int key, int x, int y) {
	switch (key) {

	}
}
