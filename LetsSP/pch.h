#pragma once

//#include <gl/glew.h>
//#include <gl/freeglut.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>


#include "gl/glew.h"
#include "gl/freeglut.h"
#include "gl/glm/glm.hpp"
#include "gl/glm/ext.hpp"
#include "gl/glm/gtc/matrix_transform.hpp"


#include <iostream>
#include <ctime>

#define CLIENT_WIDTH 800
#define CLIENT_HIEGHT 600

typedef enum Event {
	LMOUSE_DOWN, LMOUSE_UP, MOUSE_MOVE,
	A_KEY_DOWN, S_KEY_DOWN, Z_KEY_DOWN, X_KEY_DOWN,
}Event;

