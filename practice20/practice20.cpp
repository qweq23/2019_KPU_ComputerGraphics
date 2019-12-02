#include "pch.h"
#include "shader.h"
#include "loadObj.h"

#define TIMERCLOCK 13

using namespace std;



glm::vec3 scale = glm::vec3(0.3);

std::vector< glm::vec3 > outvertex, outnormal;
std::vector< glm::vec2 > outuv;

GLuint shader_id;
GLuint vao, vbo[2];
GLenum draw_style = GL_TRIANGLES;

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
GLuint lightPosLocation;
GLuint viewPosLocation;
GLuint lightColorLocation;
glm::vec3 lightPos;
glm::vec3 lightColor;
float light_angle;

float earth1_speed = 1;
float earth2_speed = 2;
float earth3_speed = 3;
float moon_speed = 4;

bool move_light = false;

void init_obj();
void init_buffers();
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

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
	glutCreateWindow("practice20");

	// GLEW 초기화하기
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

	// 메뉴 만들기
	int mune;
	mune = glutCreateMenu(Menu);
	glutAddMenuEntry("Solid Model", 0);
	glutAddMenuEntry("Wire Model", 1);

	// 쉐이더 만들고 경로 출력할 버퍼 만들기
	init_shader_program(&shader_id, "VertexShader_20.txt", "FragmentShader_20.txt");

	glUseProgram(shader_id);

	// 그릴 객체 초기화
	init_obj();

	// 버퍼 초기화
	init_buffers();

	// 변환 행렬 초기화
	set_matrix();


	// 콜백함수 등록
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(TIMERCLOCK, Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// 루프 시작
	glutMainLoop();
}


void init_obj()
{
	int vert_num = loadObj("sphere.obj", outvertex, outnormal, outuv);
	cout << vert_num << endl;
}

void init_buffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, outvertex.size() * sizeof(glm::vec3), &outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, outnormal.size() * sizeof(glm::vec3), &outnormal[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(1);
}


void set_matrix()
{
	modelLocation = glGetUniformLocation(shader_id, "modelTransform");
	viewLocation = glGetUniformLocation(shader_id, "viewTransform");
	projectionLocation = glGetUniformLocation(shader_id, "projectionTransform");

	lightPosLocation = glGetUniformLocation(shader_id, "lightPos");
	viewPosLocation = glGetUniformLocation(shader_id, "viewPos");
	lightColorLocation = glGetUniformLocation(shader_id, "lightColor");


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


	lightPos = glm::vec3(0, 1, 0);
	lightColor = glm::vec3(1);

	projection = glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(lightColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(center_md_tranform));
	glDrawArrays(draw_style, 0, outvertex.size());

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
	glDrawArrays(draw_style, 0, outvertex.size());

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(earth2_transform));
	glDrawArrays(draw_style, 0, outvertex.size());


	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();



	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(earth3_transform));
	glDrawArrays(draw_style, 0, outvertex.size());

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 36; i++) {
		glVertex3f(glm::cos(glm::radians(10.0f) * i) * 3, 0, glm::sin(glm::radians(10.0f) * i) * 3);
	}
	glEnd();

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon1_transform));
	glDrawArrays(draw_style, 0, outvertex.size());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon2_transform));
	glDrawArrays(draw_style, 0, outvertex.size());

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(moon3_transform));
	glDrawArrays(draw_style, 0, outvertex.size());


	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// 화면에 최종적으로 연결된 영역 설정
}


GLvoid Timer(int value)
{

	if (move_light) {
		light_angle += 5;
	}
	lightPos = glm::vec3(cos(glm::radians(light_angle)), 0.5, sin(glm::radians(light_angle)));


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
	// 여긴 카메라를 움직이는 거랑 같음
	switch (key) {
	// 도형들 상하좌우로 이동
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

	// 도형들 앞 뒤로 이동
	case 'Z':
	case 'z':
		cameraPos = cameraPos + glm::vec3(0, 0, -0.1);
		break;
	case 'X':
	case 'x':
		cameraPos = cameraPos + glm::vec3(0, 0, 0.1);
		break;

	// 전체 객체 y축으로 양/음 방향 회전
	case 'Y':
	case 'y':
		center_md_tranform = glm::rotate(center_md_tranform, glm::radians(-10.0f), glm::vec3(0, 1, 0));
		break;
	case 'U':
	case 'u':
		center_md_tranform = glm::rotate(center_md_tranform, glm::radians(10.0f), glm::vec3(0, 1, 0));
		break;

	case 'O':
		lightColor = glm::vec3(1);
		break;
	case 'o':
		lightColor = glm::vec3(0);
		break;

	case 'R':
		move_light = false;
		break;
	case 'r':
		move_light = true;
		break;

	case 'b':
		lightColor = glm::vec3(1, 0, 0);
		break;
	case 'n':
		lightColor = glm::vec3(0, 1, 0);
		break;
	case 'm':
		lightColor = glm::vec3(0, 0, 1);
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
		draw_style = GL_TRIANGLES;
		break;
	case 1:
		draw_style = GL_LINE_LOOP;
		break;
	}
}