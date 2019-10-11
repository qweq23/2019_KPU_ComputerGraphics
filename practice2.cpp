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
GLclampf recR = 0.0f, recG = 0.0f, recB = 0.0f;

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
	glColor3f(recG, recG, recB);
	// 좌표계가 1.0에서 -1.0 사이다
	glRectf(-0.5,-0.5,0.5,0.5);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	glutPostRedisplay();
}

GLvoid Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 1);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (state != GLUT_DOWN) return;

	if (x > 200 && x < 600 && y > 200 && y < 400) {
		// 사각형 색 바꾸기
		recR = (rand() % 256) / 256.000000f;
		recG = (rand() % 256) / 256.000000f;
		recB = (rand() % 256) / 256.000000f;
	}
	else {
		r = (rand() % 256) / 256.000000f;
		g = (rand() % 256) / 256.000000f;
		b = (rand() % 256) / 256.000000f;
	}
	glutPostRedisplay();
}


