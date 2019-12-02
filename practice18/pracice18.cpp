#include "pch.h"
#include "shader.h"
#define TIMERCLOCK 13

using namespace std;
using namespace glm;

GLuint shader_id;
GLuint vao, vbo_position, vbo_color;

float cubeVertexPositionBuffer[] = {
	1.0f, -1.0f, 1.0f,			//4
	1.0f, 1.0f, 1.0f,			//0
	-1.0f, -1.0f, 1.0f,			//7

	-1.0f, -1.0f, 1.0f,			//7
	1.0f, 1.0f, 1.0f,			//0
	-1.0f, 1.0f, 1.0f,			//3

	1.0f, 1.0f, 1.0f,			//0
	1.0f, 1.0f, -1.0f,			//1
	-1.0f, 1.0f, -1.0f,			//2

	-1.0f, 1.0f, -1.0f,			//2
	-1.0f, 1.0f, 1.0f,			//3
	1.0f, 1.0f, 1.0f,			//0

	-1.0f, 1.0f, 1.0f,			//3
	-1.0f, 1.0f, -1.0f,			//2
	-1.0f, -1.0f, 1.0f,			//7

	-1.0f, 1.0f, -1.0f,			//2
	-1.0f, -1.0f, -1.0f,		//6
	-1.0f, -1.0f, 1.0f,			//7

	-1.0f, -1.0f, -1.0f,		//6
	-1.0f, 1.0f, -1.0f,			//2
	1.0f, 1.0f, -1.0f,			//1

	-1.0f, -1.0f, -1.0f,		//6
	1.0f, 1.0f, -1.0f,			//1
	1.0f, -1.0f, -1.0f,			//5

	1.0f, -1.0f, -1.0f,			//5
	1.0f, 1.0f, -1.0f,			//1
	1.0f, 1.0f, 1.0f,			//0

	1.0f, -1.0f, 1.0f,			//4
	1.0f, -1.0f, -1.0f,			//5
	1.0f, 1.0f, 1.0f,			//0

	-1.0f, -1.0f, 1.0f,			//7
	1.0f, -1.0f, -1.0f,			//5
	1.0f, -1.0f, 1.0f,			//4

	1.0f, -1.0f, -1.0f,			//5
	-1.0f, -1.0f, 1.0f,			//7
	-1.0f, -1.0f, -1.0f,		//6
};
float stage_color_buffer[] = {
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,


	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
};
float robot_color_buffer[] = {
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,

	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,

	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,

	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,

	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,

	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
	1,1,0,
};
float robot_head_color_buffer[] = {
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
	0,1,0,
};

float robot_left_color_buffer[] = {
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
	0.5,0,0,
};
float robot_right_color_buffer[] = {
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,

	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,

	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,

	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,

	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,

	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
	0,0,0.5,
};


// Metrix
mat4 stage_front_transform;
mat4 head_transform;
mat4 nose_transform;
mat4 robot_transform;

mat4 left_arm_transform;
mat4 right_arm_transform;
mat4 left_leg_transform;
mat4 right_leg_transform;

vec3 stage_front_pos;

vec3 robot_scale_rate;
vec3 robot_pos;
float robot_angle;

vec3 arm_pos;
vec3 arm_scale_rate;

float arm_rotate_angle;
vec3 left_arm_pibot;
vec3 right_arm_pibot;

float leg_rotate_angle;
vec3 left_leg_pibot;
vec3 right_leg_pibot;

float rotate_rate = 0;

glm::vec3 cameraPos;
glm::vec3 cameraDirection;
float camera_angle;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;

GLuint modelLocation;
GLuint viewLocation;
GLuint projectionLocation;

void init_buffers();
void set_matrix();

bool stage_opening = false;

bool move_w = false;
bool move_a = false;
bool move_s = false;
bool move_d = false;

bool jumping = false;
float jump;

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid key_up(unsigned char key, int x, int y);

int main(int argc, char** argv)
{
	srand(time(NULL));

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
	glutCreateWindow("practice18");

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
	// glEnable(GL_CULL_FACE); -> 벽 보이게 하기 위해서


	// 쉐이더 만들고 경로 출력할 버퍼 만들기
	init_shader_program(&shader_id, "VertexShader_18.txt", "FragmentShader_18.txt");
	init_buffers();

	glUseProgram(shader_id);

	// 변환 행렬 초기화
	set_matrix();

	// 콜백함수 등록
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutTimerFunc(TIMERCLOCK, Timer, 0);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(key_up);

	// 루프 시작
	glutMainLoop();
}

void init_buffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_position);
	glGenBuffers(1, &vbo_color);


	glBindVertexArray(vao);

	GLint pAttribute;
	GLint cAttribute;

	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositionBuffer), cubeVertexPositionBuffer, GL_STATIC_DRAW);
	pAttribute = glGetAttribLocation(shader_id, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(stage_color_buffer), stage_color_buffer, GL_STATIC_DRAW);
	cAttribute = glGetAttribLocation(shader_id, "vColor");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
}


void set_matrix()
{
	using namespace glm;

	modelLocation = glGetUniformLocation(shader_id, "modelTransform");
	viewLocation = glGetUniformLocation(shader_id, "viewTransform");
	projectionLocation = glGetUniformLocation(shader_id, "projectionTransform");

	stage_front_pos = vec3(0);
	//stage_front_pos = vec3(0, 2, 0);
	stage_front_transform = translate(mat4(1), stage_front_pos);

	robot_scale_rate = vec3(0.1);
	robot_pos = vec3(0, -0.8, 0);
	robot_angle = 0; 

	robot_transform = translate(mat4(1), robot_pos) * scale(mat4(1), robot_scale_rate);
	head_transform = robot_transform * translate(mat4(1), vec3(0, 1.5, 0)) * scale(mat4(1), vec3(0.5, 0.5, 0.5));
	nose_transform = head_transform * translate(mat4(1), vec3(0, 0, 1)) * scale(mat4(1), vec3(0.2, 0.2, 1));

	arm_pos = vec3(1.2, 0, 0);
	arm_scale_rate = vec3(0.3, 1, 0.3);

	arm_rotate_angle = 180;

	left_arm_pibot = arm_pos + vec3(0, 1, 0);
	left_arm_transform = robot_transform * translate(mat4(1), -arm_pos) * scale(mat4(1), arm_scale_rate);

	right_arm_pibot = -arm_pos + vec3(0, 1, 0);
	right_arm_transform = robot_transform * translate(mat4(1), arm_pos) * scale(mat4(1), arm_scale_rate);

	leg_rotate_angle = 180;
	left_leg_pibot = vec3(0, 1, 0);
	right_leg_pibot = vec3(0, 1, 0);


	left_leg_transform = robot_transform * translate(mat4(1), vec3(-0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));
	right_leg_transform = robot_transform * translate(mat4(1), vec3(0.5, -1.5, 0)) * scale(mat4(1), vec3(0.3, 0.5, 0.3));


	cameraPos = glm::vec3(0, 0, 4);
	camera_angle = radians(-90.f);
	cameraDirection = cameraPos + vec3(cos(camera_angle), 0, sin(camera_angle));
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);


	projection = glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);

}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);

	// draw stage
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(stage_color_buffer), stage_color_buffer, GL_STATIC_DRAW);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(stage_front_transform));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mat4(1)));
	glDrawArrays(GL_TRIANGLES, 6, 30);

	//로봇 몸통
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(robot_color_buffer), robot_color_buffer, GL_STATIC_DRAW);
	
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(robot_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(nose_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(robot_head_color_buffer), robot_head_color_buffer, GL_STATIC_DRAW);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(head_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 로봇 왼쪽
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(robot_left_color_buffer), robot_left_color_buffer, GL_STATIC_DRAW);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_arm_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(left_leg_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);



	// 로봇 오른쪽
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(robot_right_color_buffer), robot_right_color_buffer, GL_STATIC_DRAW);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_arm_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(right_leg_transform));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// 화면에 최종적으로 연결된 영역 설정
}

GLvoid Timer(int value)
{
	if (stage_opening) {
		if (stage_front_pos.y > 2) stage_opening = false;
		stage_front_pos.y += 0.01;
		stage_front_transform = translate(mat4(1), stage_front_pos);
	}
	if (jumping) {
		robot_pos.y += cos(radians(jump)) / 50.0;
		jump += 5;

		if (jump > 180) {
			jumping = false;
			jump = 0;
		}
	}
	if (move_w) {
		rotate_rate += 10;
		arm_rotate_angle += cos(radians(rotate_rate)) * 5;
		leg_rotate_angle += cos(radians(rotate_rate)) * 5;

		if (robot_pos.z > -0.9) 
			robot_pos.z -= 0.005;
	}
	if (move_a) {
		rotate_rate += 10;
		arm_rotate_angle += cos(radians(rotate_rate)) * 5;
		leg_rotate_angle += cos(radians(rotate_rate)) * 5;

		if (robot_pos.x > -0.9)
			robot_pos.x -= 0.005;
	}
	if (move_s) {
		rotate_rate += 10;
		arm_rotate_angle += cos(radians(rotate_rate)) * 5;
		leg_rotate_angle += cos(radians(rotate_rate)) * 5;
		if (robot_pos.z < 0.9)
			robot_pos.z += 0.005;
	}
	if (move_d) {
		rotate_rate += 10;
		arm_rotate_angle += cos(radians(rotate_rate)) * 5;
		leg_rotate_angle += cos(radians(rotate_rate)) * 5;

		if (robot_pos.x < 0.9)
			robot_pos.x += 0.005;
	}


	mat4 left_arm_rotate = translate(mat4(1), left_arm_pibot) * rotate(mat4(1), radians(arm_rotate_angle),
		vec3(1, 0, 0)) * translate(mat4(1), left_arm_pibot);
	mat4 right_arm_rotate = translate(mat4(1), right_arm_pibot) * rotate(mat4(1), radians(-arm_rotate_angle),
		vec3(1, 0, 0)) * translate(mat4(1), right_arm_pibot);

	mat4 left_leg_rotate = translate(mat4(1), left_leg_pibot) * rotate(mat4(1), radians(leg_rotate_angle),
		vec3(1, 0, 0)) * translate(mat4(1), left_leg_pibot);
	mat4 right_leg_rotate = translate(mat4(1), right_leg_pibot) * rotate(mat4(1), radians(-leg_rotate_angle),
		vec3(1, 0, 0)) * translate(mat4(1), right_leg_pibot);


	robot_transform = translate(mat4(1), robot_pos) * rotate(mat4(1), radians(robot_angle), vec3(0, 1, 0)) *  scale(mat4(1), robot_scale_rate);
	head_transform = robot_transform * translate(mat4(1), vec3(0, 1.5, 0)) * scale(mat4(1), vec3(0.5, 0.5, 0.5));
	nose_transform = head_transform * translate(mat4(1), vec3(0, 0, 1)) * scale(mat4(1), vec3(0.3, 0.3, 1));

	left_arm_transform = robot_transform * translate(mat4(1), -arm_pos) * left_arm_rotate * scale(mat4(1), arm_scale_rate);
	right_arm_transform = robot_transform * translate(mat4(1), arm_pos) * right_arm_rotate * scale(mat4(1), arm_scale_rate);

	left_leg_transform = robot_transform * translate(mat4(1), vec3(-0.5, -1.5, 0)) * left_leg_rotate  * scale(mat4(1), vec3(0.3, 0.5, 0.3));
	right_leg_transform = robot_transform * translate(mat4(1), vec3(0.5, -1.5, 0)) * right_leg_rotate * scale(mat4(1), vec3(0.3, 0.5, 0.3));


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();
	glutTimerFunc(TIMERCLOCK, Timer, 0);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
		// 무대 앞면이 올라감
	case 'O':
	case 'o':
		stage_opening = true;
		break;
		// 로봇 이동
	case 'w':
		robot_angle = 180;
		move_w = true;
		break;
	case 'a':
		robot_angle = 270;
		move_a = true;
		break;
	case 's':
		robot_angle = 0;
		move_s = true;
		break;
	case 'd':
		robot_angle = 90;
		move_d = true;
		break;
		// 로봇 점프
	case 'J':
	case 'j':
		if (!jumping) jumping = true;
		break;
		// 모든 변환 리셋 
	case 'I':
	case 'i':
		set_matrix();
		break;
	}
}

GLvoid key_up(unsigned char key, int x, int y)
{
	switch (key) {
		// 무대 앞면이 올라감
	case 'O':
	case 'o':
		break;
		// 로봇 이동
	case 'w':
		move_w = false;
		break;
	case 'a':
		move_a = false;
		break;
	case 's':
		move_s = false;
		break;
	case 'd':
		move_d = false;
		break;
		// 로봇 점프
	case 'J':
	case 'j':
		break;
		// 모든 변환 리셋 
	case 'I':
	case 'i':
		break;
	}
}
