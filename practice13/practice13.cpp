#include "pch.h"
#include "shader.h"
#define TIMERCLOCK 13

using namespace std;


glm::vec3 scale = glm::vec3(0.3);


// ��� ���� ������ �ְ���
// �³״� ȸ���ϴ� �ֵ��� �̵� ����� ���ڷ� ���ž�
// �³׸� �����ϴ� ���۸� ���� ������ ���̴��� �����ָ� �׸� �� �ְ���...?

GLuint shader_id;
GLenum draw_style = GLU_FILL;

GLUquadricObj *center_sphere;

glm::mat4 center_md_tranform;
glm::mat4 scale_mat;
glm::mat4 translate_mat;

glm::mat4 earth1_transform = glm::mat4(1);
glm::mat4 earth1_rotate = glm::mat4(1);

glm::mat4 earth2_transform = glm::mat4(1);
glm::mat4 earth2_rotate = glm::mat4(1);

glm::mat4 earth3_transform = glm::mat4(1);
glm::mat4 earth3_rotate = glm::mat4(1);

glm::mat4 moon1_transform = glm::mat4(1);
glm::mat4 moon1_rotate = glm::mat4(1);

glm::mat4 moon2_transform = glm::mat4(1);
glm::mat4 moon2_rotate = glm::mat4(1);

glm::mat4 moon3_transform = glm::mat4(1);
glm::mat4 moon3_rotate = glm::mat4(1);


glm::vec3 cameraPos; 
glm::vec3 cameraDirection;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;

GLuint modelLocation;
GLuint viewLocation;
GLuint projectionLocation;
float earth1_speed = 1;
float earth2_speed = 2;
float earth3_speed = 3;
float moon_speed = 4;



void init_obj();
void set_matrix();

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Menu(int button);

void main(int argc, char** argv)
{
	srand(time(NULL));

	// ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
	glutCreateWindow("Let's SP");

	// GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// �޴� �����
	int mune;
	mune = glutCreateMenu(Menu);
	glutAddMenuEntry("Solid Model", 0);
	glutAddMenuEntry("Wire Model", 1);

	// ���̴� ����� ��� ����� ���� �����
	init_shader_program(&shader_id, "VertexShader_13.txt", "FragmentShader_13.txt");

	glUseProgram(shader_id);

	// �׸� ��ü �ʱ�ȭ
	init_obj();

	// ��ȯ ��� �ʱ�ȭ
	set_matrix();


	// �ݹ��Լ� ���
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(TIMERCLOCK, Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// ���� ����
	glutMainLoop();
}


void init_obj()
{
	center_sphere = gluNewQuadric();
	gluQuadricDrawStyle(center_sphere, GLU_LINE); // ������Ÿ�� 
	//gluQuadricDrawStyle(center_sphere, GLU_FILL); // ������Ÿ�� 
	//gluSphere(center_sphere, 1, 20, 20);
}

void set_matrix()
{
	modelLocation = glGetUniformLocation(shader_id, "modelTransform");
	viewLocation = glGetUniformLocation(shader_id, "viewTransform");
	projectionLocation = glGetUniformLocation(shader_id, "projectionTransform");

	center_md_tranform = glm::scale(glm::mat4(1), scale);
	translate_mat = glm::translate(glm::mat4(1), glm::vec3(-3, 0, 0));
	scale_mat = glm::scale(glm::mat4(1), scale);

	earth1_rotate = glm::mat4(1);
	earth1_transform = center_md_tranform * earth1_rotate * translate_mat * scale_mat;

	earth2_rotate = glm::rotate(glm::mat4(1), glm::radians(45.0f), glm::vec3(0, 0, 1));
	earth2_transform = center_md_tranform * earth2_rotate * translate_mat * scale_mat;

	earth3_rotate = glm::rotate(glm::mat4(1), glm::radians(-45.0f), glm::vec3(0, 0, 1));
	earth3_transform = center_md_tranform * earth3_rotate * translate_mat * scale_mat;

	moon1_rotate = glm::mat4(1);
	moon1_transform = earth1_transform * moon1_rotate * translate_mat * scale_mat;

	moon2_rotate = glm::mat4(1);
	moon2_transform = earth2_transform * moon2_rotate * translate_mat * scale_mat;

	moon3_rotate = glm::mat4(1);
	moon3_transform = earth3_transform * moon3_rotate * translate_mat * scale_mat;


	cameraPos = glm::vec3(0, 0, 3);
	cameraDirection = glm::vec3(0, 0, 0);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);


	projection = glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(center_md_tranform));
	gluSphere(center_sphere, 1, 20, 20);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 
			0, 
			glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();


	glBegin(GL_LINE_LOOP);
	float tmp = glm::radians(45.0f);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3 * glm::cos(tmp),
			glm::cos(glm::radians(10.0f) * i) * 3 * glm::sin(tmp),
			glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	tmp = glm::radians(-45.0f);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3 * glm::cos(tmp),
			glm::cos(glm::radians(10.0f) * i) * 3 * glm::sin(tmp),
			glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(earth1_transform));
	gluSphere(center_sphere, 1, 20, 20);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(earth2_transform));
	gluSphere(center_sphere, 1, 20, 20);


	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();



	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(earth3_transform));
	gluSphere(center_sphere, 1, 20, 20);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon1_transform));
	gluSphere(center_sphere, 1, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon2_transform));
	gluSphere(center_sphere, 1, 20, 20);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon3_transform));
	gluSphere(center_sphere, 1, 20, 20);


	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// ȭ�鿡 ���������� ����� ���� ����
}


GLvoid Timer(int value)
{

	earth1_rotate = glm::rotate(earth1_rotate, glm::radians(earth1_speed), glm::vec3(0, 1, 0));
	earth1_transform = center_md_tranform * earth1_rotate * translate_mat * scale_mat;

	earth2_rotate = glm::rotate(earth2_rotate, glm::radians(earth2_speed), glm::vec3(0, 1, 0));
	earth2_transform = center_md_tranform * earth2_rotate * translate_mat * scale_mat;

	earth3_rotate = glm::rotate(earth3_rotate, glm::radians(earth3_speed), glm::vec3(0, 1, 0));
	earth3_transform = center_md_tranform * earth3_rotate * translate_mat * scale_mat;

	moon1_rotate = glm::rotate(moon1_rotate, glm::radians(moon_speed), glm::vec3(0, 1, 0));
	moon1_transform = earth1_transform * moon1_rotate * translate_mat * scale_mat;

	moon2_rotate = glm::rotate(moon2_rotate, glm::radians(moon_speed), glm::vec3(0, 1, 0));
	moon2_transform = earth2_transform * moon2_rotate * translate_mat * scale_mat;

	moon3_rotate = glm::rotate(moon3_rotate, glm::radians(moon_speed), glm::vec3(0, 1, 0));
	moon3_transform = earth3_transform * moon3_rotate * translate_mat * scale_mat;


	glutPostRedisplay();
	glutTimerFunc(TIMERCLOCK, Timer, 0);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	// ���� ī�޶� �����̴� �Ŷ� ����
	switch (key) {
	// ������ �����¿�� �̵�
	case 'W':
	case 'w':
		cameraPos = cameraPos + glm::vec3(0, -0.1, 0);
		cameraDirection = cameraDirection + glm::vec3(0, -0.1, 0);
		break;
	case 'A':
	case 'a':
		cameraPos = cameraPos + glm::vec3(0.1, 0, 0);
		cameraDirection = cameraDirection + glm::vec3(0.1, 0, 0);
		break;
	case 'S':
	case 's':
		cameraPos = cameraPos + glm::vec3(0, 0.1, 0);
		cameraDirection = cameraDirection + glm::vec3(0, 0.1, 0);
		break;
	case 'D':
	case 'd':
		cameraPos = cameraPos + glm::vec3(-0.1, 0, 0);
		cameraDirection = cameraDirection + glm::vec3(-0.1, 0, 0);
		break;

	// ������ �� �ڷ� �̵�
	case 'Z':
	case 'z':
		cameraPos = cameraPos + glm::vec3(0, 0, -0.1);
		break;
	case 'X':
	case 'x':
		cameraPos = cameraPos + glm::vec3(0, 0, 0.1);
		break;

	// ��ü ��ü y������ ��/�� ���� ȸ��
	case 'Y':
	case 'y':
		center_md_tranform = glm::rotate(center_md_tranform, glm::radians(-10.0f), glm::vec3(0, 1, 0));
		break;
	case 'U':
	case 'u':
		center_md_tranform = glm::rotate(center_md_tranform, glm::radians(10.0f), glm::vec3(0, 1, 0));
		break;
	// test
	case 'T':
	case 't':


		break;
	}
	view = glm::mat4(1);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

}

GLvoid Mouse(int button, int state, int x, int y) {

}

GLvoid Menu(int button)
{
	switch (button) {
	case 0:
		gluQuadricDrawStyle(center_sphere, GLU_FILL); // ������Ÿ�� 
		break;
	case 1:
		gluQuadricDrawStyle(center_sphere, GLU_LINE); // ������Ÿ�� 
		break;
	}
}