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
Group *world = new Group();



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

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
  // Tell OpenGL what ModelView matrix to use:
  if (create) {
	  Matrix4 temp;
	  Matrix4 altTemp;

	  //Torso
	  Cube *Torso = new Cube(4);
	  Torso->setColor(1, 1, 1);
	  world->addChild(Torso);

	  //Head
	  altTemp.makeTranslate(0, 3, 0);
	  temp.identity();
	  temp = temp * altTemp;
	  MatrixTransform *mtHead = new MatrixTransform(temp);
	  Cube *Head = new Cube(2);
	  Head->setColor(1, 0, 0);
	  mtHead->addChild(Head);
	  altTemp.makeTranslate(-0.6, 0.25, 2);
	  MatrixTransform *mtLEye = new MatrixTransform(altTemp);
	  Cube *Eye = new Cube(0.5);
	  Eye->setColor(1, 1, 0);
	  mtLEye->addChild(Eye);
	  mtHead->addChild(mtLEye);
	  altTemp.makeTranslate(0.6, 0.25, 2);
	  MatrixTransform *mtREye = new MatrixTransform(altTemp);
	  mtREye->addChild(Eye);
	  mtHead->addChild(mtREye);
	  altTemp.makeTranslate(0, -0.5, 2);
	  MatrixTransform *mtMouth = new MatrixTransform(altTemp);
	  Cube *Mouth = new Cube(0.5);
	  altTemp.makeScale(2, 1, 1);
	  Mouth->setMatrix(altTemp);
	  Mouth->setColor(1, 1, 0);
	  mtMouth->addChild(Mouth);
	  mtHead->addChild(mtMouth);
	  world->addChild(mtHead);

	  //Left Arm
	  //Left Arm
	  temp.identity();
	  altTemp.makeTranslate(-2.5, 0, 0);
	  temp = temp * altTemp;
	  /*
	  if (forw) {
	  altTemp.makeRotateX(10);
	  temp = temp *altTemp;
	  degree = degree + 10;
	  if (degree == 40)
	  forw = !forw;
	  }
	  if (!forw && degree > -45) {
	  altTemp.makeRotateX(-10);
	  temp = temp *altTemp;
	  degree = degree - 10;
	  if (degree == -40)
	  forw = !forw;
	  }
	  */
	  MatrixTransform *mtLArm = new MatrixTransform(temp);
	  Cube *Arm = new Cube(1);
	  Arm->setColor(0, 1, 0);
	  altTemp.makeScale(1, 4, 1);
	  Arm->setMatrix(altTemp);
	  mtLArm->addChild(Arm);
	  //Hand
	  /*temp.identity();
	  altTemp.makeTranslate(0, -2.5, 0);
	  temp = temp * altTemp;
	  MatrixTransform *mtLHand = new MatrixTransform(temp);
	  Sphere *Hand = new Sphere(0.5);
	  Hand->setColor(0, 0, 1);
	  mtLHand->addChild(Hand);
	  mtLArm->addChild(mtLHand);
	  */
	  world->addChild(mtLArm);

	  //Right Arm
	  temp.identity();
	  altTemp.makeTranslate(2.5, 0, 0);
	  temp = temp * altTemp;
	  MatrixTransform *mtRArm = new MatrixTransform(temp);
	  mtRArm->addChild(Arm);
	  world->addChild(mtRArm);

	  //Left Leg
	  temp.identity();
	  altTemp.makeTranslate(-1, -4, 0);
	  temp = temp * altTemp;
	  MatrixTransform *mtLLeg = new MatrixTransform(temp);
	  Cube *Leg = new Cube(1);
	  Leg->setColor(0, 0, 1);
	  altTemp.makeScale(1, 4, 1);
	  Leg->setMatrix(altTemp);
	  mtLLeg->addChild(Leg);
	  world->addChild(mtLLeg);

	  //Right Leg
	  temp.identity();
	  altTemp.makeTranslate(1, -4, 0);
	  temp = temp * altTemp;
	  MatrixTransform *mtRLeg = new MatrixTransform(temp);
	  mtRLeg->addChild(Leg);
	  world->addChild(mtRLeg);
	  create = false;
  }

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
