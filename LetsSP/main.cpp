#include "pch.h"
#include "framework.h"
#include "shader.h"
#define TIMERCLOCK 13


Cframework framework;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

void main(int argc, char** argv)
{
	srand(time(NULL));

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
	glutCreateWindow("Let's SP");

	// GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	glPointSize(5);
	framework.Initialize();


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(TIMERCLOCK, Timer, 0);

	glutKeyboardFunc(Keyboard);

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();

	framework.Release();
}


GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	framework.Draw();

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'A':
	case 'a':
		framework.KeyEvent(Event::A_KEY_DOWN);
		break;
	case 'S':
	case 's':
		framework.KeyEvent(Event::S_KEY_DOWN);
		break;
	case 'Z':
	case 'z':
		framework.KeyEvent(Event::Z_KEY_DOWN);
		break;

	case 'X':
	case 'x':
		framework.KeyEvent(Event::X_KEY_DOWN);
		break;

	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}

GLvoid Timer(int value)
{
	framework.Update();
	glutPostRedisplay();
	glutTimerFunc(TIMERCLOCK, Timer, 0);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	// button(버튼파라미터) : GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
	// state(상태파라미터) : GLUT_UP, GLUT_DOWN
	// x, y : 윈도우에서마우스의위치

	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) framework.MouseEvent(Event::LMOUSE_UP, x, y);
		else framework.MouseEvent(Event::LMOUSE_DOWN, x, y);
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	}
}

GLvoid Motion(int x, int y)
{
	framework.MouseEvent(Event::MOUSE_MOVE, x, y);
}
