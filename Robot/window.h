#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "Vector3.h"
#include <vector>
#define M_PI 3.14159265358979323846
class Window	  // OpenGL output window related routines
{
  public:
    static int width, height; 	            // window size
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
	static void keyboardCallback(unsigned char, int, int);
	static void funcKeyboardCallback(int, int, int);
	static void loadRobot();
	static void funcMouseCallback(int, int, int, int);
	static void funcMouseMovement(int, int);
};

#endif

