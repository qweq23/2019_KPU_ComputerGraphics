#include "pch.h"
#include "loadObj.h"
#include "shader.h"
#define TIMERCLOCK 13


using namespace std;
using namespace glm;

std::vector< glm::vec3 > outvertex, outnormal;
std::vector< glm::vec2 > outuv;

GLuint shader_id_cube1;
GLuint vao, vbo[2];


mat4 cube1_transform_mat;
mat4 cube2_transform_mat;
mat4 cube3_transform_mat;
mat4 floor_transform_mat;

vec3 cube1_scale;
vec3 cube1_translate;
float cube1_rotate_radians;

vec3 cube2_scale;
vec3 cube2_translate;
float cube2_rotate_angle;
vec3 cube2_pivot;

vec3 cube3_scale;
vec3 cube3_translate;
float cube3_rotate_angle;
vec3 cube3_pivot;

vec3 floor_scale;
vec3 floor_translate;
float floor_rotate_radians;


vec3 cube1_color;
vec3 cube2_color;
vec3 cube3_color;
vec3 floor_color;

glm::vec3 cameraPos;
glm::vec3 cameraDirection;
float camera_angle;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;

GLuint cube1_modelLocation;
GLuint cube1_viewLocation;
GLuint cube1_projectionLocation;
GLuint colorLocation;

GLuint lightPosLocation;
GLuint viewPosLocation;
GLuint lightColorLocation;
vec3 lightPos;

float light_angle = 0;

bool move_light = false;
bool move_right = true;


//float cubeVertexPostionBuffer[] = {
//	0.5f, 0.5f, 0.5f,			//0
//	0.5f, 0.5f, -0.5f,			//1
//	-0.5f, 0.5f, -0.5f,			//2
//
//	-0.5f, 0.5f, -0.5f,			//2
//	-0.5f, 0.5f, 0.5f,			//3
//	0.5f, 0.5f, 0.5f,			//0
//
//	-0.5f, 0.5f, 0.5f,			//3
//	-0.5f, 0.5f, -0.5f,			//2
//	-0.5f, -0.5f, 0.5f,			//7
//
//	-0.5f, 0.5f, -0.5f,			//2
//	-0.5f, -0.5f, -0.5f,		//6
//	-0.5f, -0.5f, 0.5f,			//7
//
//	-0.5f, -0.5f, -0.5f,		//6
//	-0.5f, 0.5f, -0.5f,			//2
//	0.5f, 0.5f, -0.5f,			//1
//
//	-0.5f, -0.5f, -0.5f,		//6
//	0.5f, 0.5f, -0.5f,			//1
//	0.5f, -0.5f, -0.5f,			//5
//
//	0.5f, -0.5f, -0.5f,			//5
//	0.5f, 0.5f, -0.5f,			//1
//	0.5f, 0.5f, 0.5f,			//0
//
//	0.5f, -0.5f, 0.5f,			//4
//	0.5f, -0.5f, -0.5f,			//5
//	0.5f, 0.5f, 0.5f,			//0
//
//	0.5f, -0.5f, 0.5f,			//4
//	0.5f, 0.5f, 0.5f,			//0
//	-0.5f, -0.5f, 0.5f,			//7
//
//	-0.5f, -0.5f, 0.5f,			//7
//	0.5f, 0.5f, 0.5f,			//0
//	-0.5f, 0.5f, 0.5f,			//3
//
//	-0.5f, -0.5f, 0.5f,			//7
//	0.5f, -0.5f, -0.5f,			//5
//	0.5f, -0.5f, 0.5f,			//4
//
//	0.5f, -0.5f, -0.5f,			//5
//	-0.5f, -0.5f, 0.5f,			//7
//	-0.5f, -0.5f, -0.5f,		//6
//};

float cubeVertexPostionBuffer[512];
float cubeNormalBuffer[512];

void convert_vec3_float();
void init_buffers();
void set_matrix();

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// 오브젝트 불러오기
	int vert_num = loadObj("box.obj", outvertex, outnormal, outuv);
	cout << vert_num << endl;

	convert_vec3_float();

	// 쉐이더 만들고 경로 출력할 버퍼 만들기
	init_shader_program(&shader_id_cube1, "VertexShader_16.txt", "FragmentShader_16.txt");
	glUseProgram(shader_id_cube1);

	// 버퍼 초기화
	init_buffers();

	// 변환 행렬 초기화
	set_matrix();


	// 콜백함수 등록
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(TIMERCLOCK, Timer, 0);
	glutKeyboardFunc(Keyboard);

	// 루프 시작
	glutMainLoop();
}


void convert_vec3_float()
{
	for (int i = 0; i < outvertex.size(); i++) {
		cubeVertexPostionBuffer[i * 3 + 0] = outvertex[i].x;
		cubeVertexPostionBuffer[i * 3 + 1] = outvertex[i].y;
		cubeVertexPostionBuffer[i * 3 + 2] = outvertex[i].z;

		cubeNormalBuffer[i * 3 + 0] = outnormal[i].x;
		cubeNormalBuffer[i * 3 + 1] = outnormal[i].y;
		cubeNormalBuffer[i * 3 + 2] = outnormal[i].z;
	}
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
	cube1_modelLocation = glGetUniformLocation(shader_id_cube1, "modelTransform");
	cube1_viewLocation = glGetUniformLocation(shader_id_cube1, "viewTransform");
	cube1_projectionLocation = glGetUniformLocation(shader_id_cube1, "projectionTransform");
	colorLocation = glGetUniformLocation(shader_id_cube1, "objectColor");
	
	lightPosLocation = glGetUniformLocation(shader_id_cube1, "lightPos");
	viewPosLocation = glGetUniformLocation(shader_id_cube1, "viewPos");
	lightColorLocation = glGetUniformLocation(shader_id_cube1, "lightColor");

	cube1_scale = vec3(0.5, 0.2, 0.5);
	cube1_translate = vec3(0, 0.1, 0);
	cube1_rotate_radians = 0;
	cube1_transform_mat = translate(mat4(1), cube1_translate) * scale(mat4(1), cube1_scale);
	cube1_color = vec3(1, 0, 0);

	cube2_scale = vec3(0.5, 1, 0.5);
	cube2_translate = vec3(0, 0.8, 0);
	cube2_rotate_angle = 0;
	cube3_pivot = vec3(0, 0.4, 0);
	cube2_transform_mat = cube1_transform_mat * translate(mat4(1), cube2_translate) * scale(mat4(1), cube2_scale);
	cube2_color = vec3(1, 1, 0);

	cube3_scale = vec3(0.3, 1, 0.3);
	cube3_translate = vec3(0, 1, 0);
	cube3_rotate_angle = 0;
	cube3_pivot = vec3(0, 0.4, 0);
	cube3_transform_mat = cube2_transform_mat * translate(mat4(1), cube3_translate) * scale(mat4(1), cube3_scale);
	cube3_color = vec3(0, 1, 1);

	floor_scale = vec3(3, 0.1, 3);
	floor_translate;
	floor_rotate_radians;
	floor_color = vec3(0.5, 0.5, 0.5);


	floor_transform_mat = scale(mat4(1), floor_scale);

	cameraPos = glm::vec3(0, 1, 5);
	camera_angle = radians(-90.f);
	cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	projection = glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);


	lightPos = vec3(cos(light_angle), 1, sin(light_angle));

	glUniform3f(lightColorLocation, 1, 1, 1);


}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glUseProgram(shader_id_cube1);
	glBindVertexArray(vao);
	glUniformMatrix4fv(cube1_viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(cube1_projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);



	glUniformMatrix4fv(cube1_modelLocation, 1, GL_FALSE, glm::value_ptr(cube1_transform_mat));
	glUniform3f(colorLocation, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(cube1_modelLocation, 1, GL_FALSE, glm::value_ptr(cube2_transform_mat));
	glUniform3f(colorLocation, 1, 1, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glUniformMatrix4fv(cube1_modelLocation, 1, GL_FALSE, glm::value_ptr(cube3_transform_mat));
	glUniform3f(colorLocation, 0, 1, 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glUniformMatrix4fv(cube1_modelLocation, 1, GL_FALSE, glm::value_ptr(floor_transform_mat));
	glUniform3f(colorLocation, 0.5, 0.5, 0.5);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// 화면에 최종적으로 연결된 영역 설정
}


GLvoid Timer(int value)
{
	if (move_right) {
		cube1_translate.x += 0.01;
		if (cube1_translate.x > 1)
			move_right = false;
	}
	else {
		cube1_translate.x -= 0.01;
		if (cube1_translate.x < -1)
			move_right = true;
	}
	if (move_light) {
		light_angle += 5;
	}
	lightPos = vec3(cos(radians(light_angle)), 0.5, sin(radians(light_angle)));

	cube1_transform_mat = translate(mat4(1), cube1_translate) * scale(mat4(1), cube1_scale) 
		* rotate(mat4(1), cube1_rotate_radians, vec3(0, 1, 0));

	mat4 cube2_rotate = translate(mat4(1), -cube3_pivot) * rotate(mat4(1), radians(cube2_rotate_angle), vec3(1, 0, 0)) * translate(mat4(1), cube3_pivot);
	cube2_transform_mat = cube1_transform_mat * translate(mat4(1), cube2_translate) * scale(mat4(1), cube2_scale) * cube2_rotate;

	mat4 cube3_rotate = translate(mat4(1), -cube3_pivot) * rotate(mat4(1), radians(cube3_rotate_angle), vec3(0, 0, 1)) * translate(mat4(1), cube3_pivot);
	cube3_transform_mat = cube2_transform_mat * translate(mat4(1), cube3_translate) * cube3_rotate * scale(mat4(1), cube3_scale);


	cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	glutPostRedisplay();
	glutTimerFunc(TIMERCLOCK, Timer, 0);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
		// 크래인의 아래 몸체가 y축에 대하여 회전
	case 'B':
		cube1_rotate_radians += radians(10.0f);
		break;
	case 'b':
		cube1_rotate_radians -= radians(10.0f);
		break;

		// 크래인의 중앙이 x축에 대하여 회전 90~180사이
	case 'M':
		if (cube2_rotate_angle == 0) break;
		cube2_rotate_angle += 10;
		break;
	case 'm':
		if (cube2_rotate_angle == -90) break;
		cube2_rotate_angle -= 10;
		break;

		// 크레인의 팔이 z축에 대하여 회전 90~180사이
	case 'T':
		if (cube3_rotate_angle == 0) break;
		cube3_rotate_angle += 10;
		break;
	case 't':
		if (cube3_rotate_angle == -90) break;
		cube3_rotate_angle -= 10;
		break;

	case 'A':
	case 'a':
		move_light = true;
		break;

		// 움직임 멈추기
	case 'S':
	case 's':
		move_light = false;
		break;

		// 모든 움직임 초기화
	case 'C':
	case 'c':
		cube1_rotate_radians = 0;
		cube2_rotate_angle = 0;
		cube3_rotate_angle = 0;
		break;
		
		// 앞뒤로 이동
	case 'Z':
		cameraPos = cameraPos + vec3(0, 0, 0.1);
		break;
	case 'z':
		cameraPos = cameraPos + vec3(0, 0, -0.1);
		break;

		// 좌우로 이동
	case 'X':
		cameraPos += vec3(0.1, 0, 0);
		break;
	case 'x':
		cameraPos += vec3(-0.1, 0, 0);
		break;

		// 카메라 기준 y축 회전
	case 'R':
		camera_angle += radians(10.0f);
		break;
	case 'r':
		camera_angle -= radians(10.0f);
		break;

	case 'O':
		glUniform3f(lightColorLocation, 1, 1, 1);
		break;
	case 'o':
		glUniform3f(lightColorLocation, 0, 0, 0);
		break;

	case 'Q':
	case 'q':
		glutLeaveMainLoop();
		break;
	}
}
