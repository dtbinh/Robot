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
#include "light.h"
#include "material.h"
#include "shader.h"
using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here
Vector3 mouse;
Group *B = new Group();
Group *Be = new Group();
Group *D = new Group();
Group *T = new Group();

Matrix4 id;
Matrix4 rotation;
light *Light = new light();
material *Material = new material();
material *blank = new material();
static double degree = 1;
//Flags
bool controlLight = false;
bool forw = true;
bool walk = false;
bool bound = false;
bool cull = false;
bool robot = false;
bool scale = false;
bool rot = false;
bool mat= true;
bool shad = false;
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
double lightAngle = 45;

int show = 0;

//Parser
Parser *bunny = new Parser("bunny.obj", double(Window::width) / double(Window::height));
Parser *bear = new Parser("bear.obj", double(Window::width) / double(Window::height));
Parser *dragon = new Parser("dragon.obj", double(Window::width) / double(Window::height));
//Camera
Camera camera;
Camera pc;

//Frustum
Frustum frustum;

//Main Group
Group *world = new Group();

MatrixTransform *CubeControl = new MatrixTransform();
MatrixTransform *DragonControl = new MatrixTransform();
MatrixTransform *BearControl = new MatrixTransform();
MatrixTransform *BunnyControl = new MatrixTransform();
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
Cube *Test = new Cube(4);
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
  glTranslatef(0, 0, -20);    // walk camera back 20 units so that it looks at the origin (or else it's in the origin)
  glMatrixMode(GL_MODELVIEW);
}

void Window::loadRobot() {
	Matrix4 temp;
	Matrix4 altTemp;
	id.identity();
	//Setting material
	Material->setAmbient(0.8, 0.8, 0.2, 1.0);
	Material->setDiffuse(0.1, 0.5, 0.8, 1.0);
	Material->setSpecular(1.0, 1.0, 1.0, 1.0);
	Material->setShininess(5.0);
	Material->setEmission(0.3, 0.2, 0.2, 0.0);

	//Seting Point Light
	Light->setAmbient(0, 0, 0, 1.0, 1);
	Light->setDiffuse(0, 0, 1, 0, 1);
	Light->setSpecular(0, 1.0, 1.0, 0, 1);
	Light->setPosition(0, 10, 0, 0 , 1);
	Light->setType(0, 0);
	Light->setAttenuation(0, 1.0, 0, 0);

	//Setting Spot Light
	Light->setAmbient(1, 1, 0, 0, 1.0);
	Light->setDiffuse(1, 1.0, 0.0, 1.0, 1.0);
	Light->setSpecular(1, .0, 1,.0, 1.0);
	Light->setPosition(1, -10, 0, 0, 1);
	Light->setAttenuation(1, 1.0, 0, 0.);
	Light->setSpotCutoff(1, 45);
	Light->setDirection(1, 1, 0, 0);
	Light->setSpotExponent(1, 2);
	Light->setType(1, 1);

	//Setting Cameras
	camera.set(Vector3(0, 5, 0.0), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Constructing Robots
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
	
	//Loading BUnny
	bunny->load();
	dragon->load();
	bear->load();
	Test->setColor(.5, .5, .5);
	temp.identity();
	rotation.identity();
	Test->setMatrix(temp);
	bunny->setMatrix(temp);
	dragon->setMatrix(temp);
	bear->setMatrix(temp);
	CubeControl->set(Globals::object.getMatrix());
	BunnyControl->set(Globals::object.getMatrix());
	CubeControl->addChild(Test);
	BunnyControl->addChild(bunny);
	BearControl->set(Globals::object.getMatrix());
	BearControl->addChild(bear);
	DragonControl->set(Globals::object.getMatrix());
	DragonControl->addChild(dragon);
	T->addChild(CubeControl);
	B -> addChild(BunnyControl);
	Be->addChild(BearControl);
	D->addChild(DragonControl);

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
		//Test->setMatrix(Globals::object.getMatrix());
		blank->setProperties();
		Light->lights[0].draw();
		Light->lights[1].draw();
		if (mat)
			Material->setProperties();
		BunnyControl->set(Globals::object.getMatrix());
		DragonControl->set(Globals::object.getMatrix());
		BearControl->set(Globals::object.getMatrix());
		CubeControl->set(Globals::object.getMatrix());
		Test->setMatrix(rotation *Test->getMatrix());
		bunny->setMatrix(rotation * bunny->getMatrix());
		dragon->setMatrix(rotation * dragon->getMatrix());
		bear->setMatrix(rotation * bear->getMatrix());
		if (show == 0) {
			if (shad) {
				Globals::shader->bind();
				B->draw(id, false, false, frustum);
				Globals::shader->unbind();
			}
			else
				B->draw(id, false, false, frustum);
		}
		else if (show == 1) {
			if (shad) {
				Globals::shader->bind();
				D->draw(id, false, false, frustum);
				Globals::shader->unbind();
			}
			else
				D->draw(id, false, false, frustum);
		}
		else if (show == 2) {
			if (shad) {
				Globals::shader->bind();
				Be->draw(id, false, false, frustum);
				Globals::shader->unbind();
			}
			else
				Be->draw(id, false, false, frustum);
		}
		else {
			if (shad) {
				Globals::shader->bind();
				T->draw(id, false, false, frustum);
				Globals::shader->unbind();
			}
			else
				T->draw(id, false, false, frustum);
		}
		rotation.identity();
		glFlush();
		glutSwapBuffers();
	}
	glFlush();
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
		Test->setMatrix(id);
		bunny->setMatrix(id);
		Light->setSpotCutoff(1, 45);
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
	case 'l':
		Light->turnOn(0);
		glFlush();
		break;
	case 'L':
		Light->turnOff(0);
		break;
	case 'j':
		Light->turnOn(1);
		glFlush();
		break;
	case 'J':
		Light->turnOff(1);
		break;
	case'.':
		mat = !mat;
		break;
	case'p':
		shad = !shad;
		if (shad) {
			std::cout << "shad on " << std::endl;
		}
		if (!shad) {
			std::cout << "shad off " << std::endl;
		}
		break;
	case'i':
		controlLight = !controlLight;
		break;
	}

}


void Window::funcMouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			scale = true;
			mouse.set(x, y, 0);
		}
		else if (state == GLUT_UP) {
			scale = false;
		}
	}
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN) {
			rot = true;
			double tempX = (2.0*x - Window::width) / Window::width;
			double tempY = (Window::height - 2.0*y) / Window::height;
			mouse.set(tempX, tempY, 0);
			double d = mouse.length();
			d = (d < 1.0) ? d : 1.0;
			mouse.set(tempX, tempY, sqrtf(1.001 - d*d));
			mouse.normalize();
		}
		else if (state == GLUT_UP) {
			rot = false;
		}
	}
}

void Window::funcMouseMovement(int x, int y) {

	if (scale) {
		int scaleFactor = y - mouse.getY();
		if (!controlLight) {
			if (scaleFactor == 0) {

			}
			else if (scaleFactor < 0) {
				Globals::object.scale(11.0 / 10.0);
			}
			else if (scaleFactor > 0) {
				Globals::object.scale(10.0 / 11.0);
			}
			mouse.set(x, y, 0);
		}
		else {
			if (scaleFactor == 0) {

			}
			else if (scaleFactor < 0) {
				lightAngle = lightAngle + 5;
				if (lightAngle == 90) {
					lightAngle = 85;
				}
				Light->setSpotCutoff(1, lightAngle);
				Light->turnOn(1);
			}
			else if (scaleFactor > 0) {
				lightAngle = lightAngle - 5;
				if (lightAngle == 0) {
					lightAngle = 5;
				}
				Light->setSpotCutoff(1, lightAngle);
				Light->turnOn(1);
			}
			mouse.set(x, y, 0);
		}
	}
	if (rot) {
		Vector3 newPosition;
		Matrix4 temp;
		double tempX = (2.0*x - Window::width) / Window::width;
		double tempY = (Window::height - 2.0*y) / Window::height;
		newPosition.set(tempX, tempY, 0);
		double d = newPosition.length();
		d = (d < 1.0) ? d : 1.0;
		newPosition.set(tempX, tempY, sqrtf(1.001 - d*d));
		newPosition.normalize();
		mouse.normalize();
		Vector3 direction = newPosition - mouse;
		double speed = direction.length();
		if (speed > 0.001) {
			Vector3 axis = axis.cross(mouse, newPosition);
			//Globals::object.spinV(speed * 100, axis);
			rotation.makeRotate(speed * 150, axis);
			mouse.set(newPosition.getX(), newPosition.getY(), newPosition.getZ());
		}
	}
}

void Window::funcKeyboardCallback(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		show = 0;
		break;
	case GLUT_KEY_F2:
		show = 1;
		break;
	case GLUT_KEY_F3:
		show = 2;
		break;
	case GLUT_KEY_F4:
		show = 3;
		break;
	}
}