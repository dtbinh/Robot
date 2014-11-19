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
#include "camera.h"
#include "frustum.h"
#include <time.h>
#include "parser.h"

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
static double degree = 1;
bool forw = true;
bool walk = false;
bool bound = false;
bool cull = false;
bool robot = true;

//Dimensions
double sTorso = 4;
double sHead = 2;
double sEye = 0.5;
double sMouth = 0.5;
double wMouth = 2;
double sArm = 1;
double sLeg = 1;
double wArm = 4;
double wLeg = 4;
//Parser
Parser *bunny = new Parser("bunny.obj", double(Window::width) / double(Window::height));
//Camera
Camera camera;
Camera pc;

//Frustum
Frustum frustum;

//Main Group
Group *world = new Group();

MatrixTransform *Control = new MatrixTransform();
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
Cube *Torso = new Cube(sTorso);
Cube *Head = new Cube(sHead);
Cube *Eye = new Cube(sEye);
Cube *Mouth = new Cube(sMouth);
Cube *Arm = new Cube(sArm);
Cube *Leg = new Cube(sArm);


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
  frustum.setCamInternals(60.0, double(width) / double(height), 1.0, 1000.0);
  frustum.setCamDef(Vector3(0.0, 0.0, 20.0), Vector3(0, 0, 0), Vector3(0, 1, 0));
  //glTranslatef(0, -7, -30);    // walk camera back 20 units so that it looks at the origin (or else it's in the origin)
  glMatrixMode(GL_MODELVIEW);
}

void Window::loadRobot() {
	Matrix4 temp;
	Matrix4 altTemp;
	camera.set(Vector3(0, 5, 20.0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	pc.set(Vector3(0, 0, 20.0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Control->set(Globals::object.getMatrix());

	//Torso
	Torso->setColor(1, 1, 1);
	Control->addChild(Torso);

	//Head
	altTemp.makeTranslate(0, (sTorso+sHead)/2, 0);
	mtHead->set(altTemp);
	Head->setColor(1, 0, 0);
	mtHead->addChild(Head);
	//Eyes
	altTemp.makeTranslate(-0.6*(sHead/2), 0.25*(sHead/2), (sHead/2));
	mtLEye->set(altTemp);
	Eye->setColor(1, 1, 0);
	mtLEye->addChild(Eye);
	mtHead->addChild(mtLEye);
	altTemp.makeTranslate(0.6*(sHead / 2), 0.25*(sHead / 2),(sHead / 2));
	mtREye->set(altTemp);
	mtREye->addChild(Eye);
	mtHead->addChild(mtREye);
	//Mouth
	altTemp.makeTranslate(0, -0.5*(sHead / 2), (sHead / 2));
	mtMouth->set(altTemp);
	altTemp.makeScale(wMouth, 1, 1);
	Mouth->setMatrix(altTemp);
	Mouth->setColor(1, 1, 0);
	mtMouth->addChild(Mouth);
	mtHead->addChild(mtMouth);
	Control->addChild(mtHead);

	//Left Arm
	altTemp.makeTranslate(-(sTorso+sArm)/2, 0, 0);
	mtLArm->set(altTemp);
	Arm->setColor(0, 1, 0);
	altTemp.makeScale(1, wArm, 1);
	Arm->setMatrix(altTemp);
	LArmRot->addChild(Arm);
	mtLArm->addChild(LArmRot);
	Control->addChild(mtLArm);

	//Right Arm
	altTemp.makeTranslate((sTorso + sArm) / 2, 0, 0);
	mtRArm->set(altTemp);
	RArmRot->addChild(Arm);
	mtRArm->addChild(RArmRot);
	Control->addChild(mtRArm);

	//Left Leg
	altTemp.makeTranslate(-(0.25*sTorso), -(sTorso+wLeg)/2, 0);
	mtLLeg->set(altTemp);
	Leg->setColor(0, 0, 1);
	altTemp.makeScale(1, wLeg, 1);
	Leg->setMatrix(altTemp);
	LLegRot->addChild(Leg);
	mtLLeg->addChild(LLegRot);
	Control->addChild(mtLLeg);

	//Right Leg
	altTemp.makeTranslate((0.25*sTorso), -(sTorso + wLeg) / 2, 0);
	mtRLeg->set(altTemp);
	RLegRot->addChild(Leg);
	mtRLeg->addChild(RLegRot);
	Control->addChild(mtRLeg);
	
	for (int x = -50; x < 50; x = x + 7) {
		for (int z = -50; z < 50; z = z + 5) {
			temp.makeTranslate(x, 0, z);
			MatrixTransform *tempmat = new MatrixTransform(temp);
			tempmat->addChild(Control);
			world->addChild(tempmat);
		}
	}
	
	bunny->load();
	bunny->setMatrix(Globals::object.getMatrix());
	//world->addChild(Control);
}
//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	if (robot) {
		clock_t begin = clock();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
		glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
		// Tell OpenGL what ModelView matrix to use:
		Matrix4 altTemp;
		Matrix4 temp;
		Matrix4 altTemp1;
		Control->set(Globals::object.getMatrix());
		if (walk) {
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
		}
		else {
			altTemp.identity();
			altTemp1.identity();
		}
		temp = altTemp;
		altTemp.makeTranslate(0, -wArm / 2, 0);
		altTemp1 = altTemp1 * altTemp;
		temp = temp * altTemp;
		altTemp.makeTranslate(0, wArm / 2, 0);
		temp = altTemp * temp;
		altTemp1 = altTemp * altTemp1;
		LArmRot->set(temp);
		RLegRot->set(temp);
		RArmRot->set(altTemp1);
		LLegRot->set(altTemp1);
		temp.identity();
		world->draw(camera.getInverse(), bound, cull, frustum);
		world->update();


		glFlush();
		glutSwapBuffers();
		clock_t end = clock();
		double sec = double(end - begin) / CLOCKS_PER_SEC;
		double fps = 1 / sec;
		cout << "FPS: " << fps << endl;
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffers
		glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode
		Matrix4 temp;
		temp = Globals::object.getMatrix();
		bunny->setMatrix(Globals::object.getMatrix());
		bunny->draw(camera.getInverse(), false, false, frustum);
		glFlush();
		glutSwapBuffers();
	}
}
//----------------------------------------------------------------------------
// Callback method called by GLUT when users press specific keys on the keyboard
void Window::keyboardCallback(unsigned char key, int x, int y) {
	switch (key) {
		// toggles direction of spin between clockwise and counterclockwise
	case 't':
		Globals::object.flipRotation();
		break;
		// moves object left
	case 'X':
		Globals::object.move(1, 0, 0);
		break;
		// moves object right
	case 'x':
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
		Globals::object.scale(10.0 / 11.0);
		break;
		// scales object up
	case 's':
		Globals::object.scale(11.0 / 10.0);
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
	case'k':
		walk = !walk;
		break;
	case 'b':
		bound = !bound;
		break;
	case 'c':
		cull = !cull;
		cout << "CULL" << endl;
		break;
	case'R':
		robot = !robot;
		break;
	}
}
