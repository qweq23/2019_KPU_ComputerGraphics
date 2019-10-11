#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);

GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;
GLint timerTrigger = 0;


void main(int argc, char** argv)
{
	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");

	// GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(0, Timer, 0);

	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);

	// 그리기

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'R':
	case 'r':
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
		break;
	case 'G':
	case 'g':
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
		break;
	case 'B':
	case 'b':
		r = 0.0f;
		g = 0.0f;
		b = 1.0f;
		break;
	case 'A':
	case 'a':
		r = (rand() % 256) / 256.000000f;
		g = (rand() % 256) / 256.000000f;
		b = (rand() % 256) / 256.000000f;
		break;
	case 'W':
	case 'w':
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		break;
	case 'K':
	case 'k':
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		break;
	case 'T':
	case 't':
		timerTrigger = 1;
		break;
	case 'S':
	case 's':
		timerTrigger = 0;
		break;
	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

GLvoid Timer(int value)
{
	if (timerTrigger == 1) {
		r = (rand() % 256) / 256.000000f;
		g = (rand() % 256) / 256.000000f;
		b = (rand() % 256) / 256.000000f;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, Timer, timerTrigger);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	return GLvoid();
}


