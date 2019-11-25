#include "pch.h"
#include "shader.h"
#define TIMERCLOCK 13

using namespace std;

GLuint shader_id;
GLuint VAO_cube, VBO_position_cube, VBO_color_cube;
GLuint VAO_pyramid, VBO_position_pyramid, VBO_color_pyramid;

glm::mat4 world_transform = glm::mat4(1);

glm::mat4 cube_transform = glm::mat4(1);
glm::mat4 cube_translate;
glm::mat4 cube_rotate;

glm::mat4 cube_up_transform = glm::mat4(1);
glm::mat4 cube_up_rotate;

glm::mat4 cube_front_transform = glm::mat4(1);
glm::mat4 cube_front_rotate;

glm::mat4 pyramid_transform = glm::mat4(1);
glm::mat4 pyramid_translate;
glm::mat4 pyramid_rotate;

glm::mat4 pyramid1_transform = glm::mat4(1);
glm::mat4 pyramid1_rotate;

glm::mat4 pyramid2_transform = glm::mat4(1);
glm::mat4 pyramid2_rotate;

glm::mat4 pyramid3_transform = glm::mat4(1);
glm::mat4 pyramid3_rotate;

glm::mat4 pyramid4_transform = glm::mat4(1);
glm::mat4 pyramid4_rotate;

float cube_front_angle = 0;
float pyramid_angle = 0;
bool cube_animation = false;
bool cube_open = false;
bool c_opening = true;
bool pyramid_open = false;
bool p_opening = true;
bool perspective_projection = true;

// 36
float cubeVertexPostionBuffer[] = {
	0.5f, 0.5f, 0.5f,			//0
	0.5f, 0.5f, -0.5f,			//1
	-0.5f, 0.5f, -0.5f,			//2

	-0.5f, 0.5f, -0.5f,			//2
	-0.5f, 0.5f, 0.5f,			//3
	0.5f, 0.5f, 0.5f,			//0

	-0.5f, 0.5f, 0.5f,			//3
	-0.5f, 0.5f, -0.5f,			//2
	-0.5f, -0.5f, 0.5f,			//7

	-0.5f, 0.5f, -0.5f,			//2
	-0.5f, -0.5f, -0.5f,		//6
	-0.5f, -0.5f, 0.5f,			//7

	-0.5f, -0.5f, -0.5f,		//6
	-0.5f, 0.5f, -0.5f,			//2
	0.5f, 0.5f, -0.5f,			//1

	-0.5f, -0.5f, -0.5f,		//6
	0.5f, 0.5f, -0.5f,			//1
	0.5f, -0.5f, -0.5f,			//5

	0.5f, -0.5f, -0.5f,			//5
	0.5f, 0.5f, -0.5f,			//1
	0.5f, 0.5f, 0.5f,			//0

	0.5f, -0.5f, 0.5f,			//4
	0.5f, -0.5f, -0.5f,			//5
	0.5f, 0.5f, 0.5f,			//0

	0.5f, -0.5f, 0.5f,			//4
	0.5f, 0.5f, 0.5f,			//0
	-0.5f, -0.5f, 0.5f,			//7

	-0.5f, -0.5f, 0.5f,			//7
	0.5f, 0.5f, 0.5f,			//0
	-0.5f, 0.5f, 0.5f,			//3

	-0.5f, -0.5f, 0.5f,			//7
	0.5f, -0.5f, -0.5f,			//5
	0.5f, -0.5f, 0.5f,			//4

	0.5f, -0.5f, -0.5f,			//5
	-0.5f, -0.5f, 0.5f,			//7
	-0.5f, -0.5f, -0.5f,		//6
};
float cubeVertexColorBuffer[] = {
	1.0f, 0.0f, 0.0f,		// 5 빨강
	1.0f, 0.0f, 0.0f,		// 5 빨강
	1.0f, 0.0f, 0.0f,		// 5 빨강
	1.0f, 0.0f, 0.0f,		// 5 빨강
	1.0f, 0.0f, 0.0f,		// 5 빨강
	1.0f, 0.0f, 0.0f,		// 5 빨강


	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록

	0.0f, 0.0f, 1.0f,		// 7 파랑
	0.0f, 0.0f, 1.0f,		// 7 파랑
	0.0f, 0.0f, 1.0f,		// 7 파랑
	0.0f, 0.0f, 1.0f,		// 7 파랑
	0.0f, 0.0f, 1.0f,		// 7 파랑
	0.0f, 0.0f, 1.0f,		// 7 파랑

	1.0f, 1.0f, 0.0f,		// 1 노랑
	1.0f, 1.0f, 0.0f,		// 1 노랑
	1.0f, 1.0f, 0.0f,		// 1 노랑
	1.0f, 1.0f, 0.0f,		// 1 노랑
	1.0f, 1.0f, 0.0f,		// 1 노랑
	1.0f, 1.0f, 0.0f,		// 1 노랑


	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록

	1.0f, 0.0f, 1.0f,		// 4 보라
	1.0f, 0.0f, 1.0f,		// 4 보라
	1.0f, 0.0f, 1.0f,		// 4 보라
	1.0f, 0.0f, 1.0f,		// 4 보라
	1.0f, 0.0f, 1.0f,		// 4 보라
	1.0f, 0.0f, 1.0f,		// 4 보라


	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 1.0f, 1.0f, 0.0f,		// 1 노랑
	// 0.0f, 1.0f, 0.0f,		// 2 초록
	// 
	// 0.0f, 1.0f, 0.0f,		// 2 초록
	// 0.0f, 1.0f, 1.0f,		// 3 청록
	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 
	// 0.0f, 1.0f, 1.0f,		// 3 청록
	// 0.0f, 1.0f, 0.0f,		// 2 초록
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 
	// 0.0f, 1.0f, 0.0f,		// 2 초록
	// 1.0f, 1.0f, 1.0f,		// 6 흰
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 
	// 1.0f, 1.0f, 1.0f,		// 6 흰
	// 0.0f, 1.0f, 0.0f,		// 2 초록
	// 1.0f, 1.0f, 0.0f,		// 1 노랑
	// 
	// 1.0f, 1.0f, 1.0f,		// 6 흰
	// 1.0f, 1.0f, 0.0f,		// 1 노랑
	// 1.0f, 0.0f, 0.0f,		// 5 빨강
	// 
	// 1.0f, 0.0f, 0.0f,		// 5 빨강
	// 1.0f, 1.0f, 0.0f,		// 1 노랑
	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 
	// 1.0f, 0.0f, 1.0f,		// 4 보라
	// 1.0f, 0.0f, 0.0f,		// 5 빨강
	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 
	// 1.0f, 0.0f, 1.0f,		// 4 보라
	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 0.0f, 0.0f, 0.0f,		// 0 검은색
	// 0.0f, 1.0f, 1.0f,		// 3 청록
	// 
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 1.0f, 0.0f, 0.0f,		// 5 빨강
	// 1.0f, 0.0f, 1.0f,		// 4 보라
	// 
	// 1.0f, 0.0f, 0.0f,		// 5 빨강
	// 0.0f, 0.0f, 1.0f,		// 7 파랑
	// 1.0f, 1.0f, 1.0f,		// 6 흰
};

// 18
float pyramidVertexPositionBuffer[] = {
	0, 0.5, 0,				// 0
	0.5, -0.5, 0.5,			// 1
	0.5, -0.5, -0.5,		// 2

	0, 0.5, 0,				// 0
	0.5, -0.5, -0.5,		// 2
	-0.5, -0.5, -0.5,		// 3

	0, 0.5, 0,				// 0
	-0.5, -0.5, -0.5,		// 3
	-0.5, -0.5, 0.5,		// 4

	0, 0.5, 0,				// 0
	-0.5, -0.5, 0.5,		// 4
	0.5, -0.5, 0.5,			// 1

	0.5, -0.5, 0.5,			// 1
	-0.5, -0.5, -0.5,		// 3
	0.5, -0.5, -0.5,		// 2

	-0.5, -0.5, -0.5,		// 3
	0.5, -0.5, 0.5,			// 1
	-0.5, -0.5, 0.5			// 4
};
float pyramidVertexColorBuffer[] = {
	0.0f, 0.0f, 1.0f,		// 1 파
	0.0f, 0.0f, 1.0f,		// 1 파
	0.0f, 0.0f, 1.0f,		// 1 파

	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록
	0.0f, 1.0f, 1.0f,		// 3 청록

	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록
	0.0f, 1.0f, 0.0f,		// 2 초록

	1.0f, 0.0f, 0.0f,		// 0 빨
	1.0f, 0.0f, 0.0f,		// 0 빨
	1.0f, 0.0f, 0.0f,		// 0 빨

	1.0f, 1.0f, 0.0f,		// 노랑
	1.0f, 1.0f, 0.0f,		// 노랑
	1.0f, 1.0f, 0.0f,		// 노랑
	1.0f, 1.0f, 0.0f,		// 노랑
	1.0f, 1.0f, 0.0f,		// 노랑
	1.0f, 1.0f, 0.0f,		// 노랑


	//1.0f, 0.0f, 0.0f,		// 0 빨
	//0.0f, 0.0f, 1.0f,		// 1 파
	//0.0f, 1.0f, 0.0f,		// 2 초록
	//
	//1.0f, 0.0f, 0.0f,		// 0 빨
	//0.0f, 1.0f, 0.0f,		// 2 초록
	//0.0f, 1.0f, 1.0f,		// 3 청록
	//
	//1.0f, 0.0f, 0.0f,		// 0 빨
	//0.0f, 1.0f, 1.0f,		// 3 청록
	//1.0f, 0.0f, 1.0f,		// 4 보라
	//
	//1.0f, 0.0f, 0.0f,		// 0 빨
	//1.0f, 0.0f, 1.0f,		// 4 보라
	//0.0f, 0.0f, 1.0f,		// 1 파
	//
	//0.0f, 0.0f, 1.0f,		// 1 파
	//0.0f, 1.0f, 1.0f,		// 3 청록
	//0.0f, 1.0f, 0.0f,		// 2 초록
	//
	//0.0f, 1.0f, 1.0f,		// 3 청록
	//0.0f, 0.0f, 1.0f,		// 1 파
	//1.0f, 0.0f, 1.0f,		// 4 보라
};


glm::vec3 cameraPos;
glm::vec3 cameraDirection;
glm::vec3 cameraUp;
glm::mat4 view;
glm::mat4 projection;

GLuint modelLocation;
GLuint viewLocation;
GLuint projectionLocation;


void init_buffers();
void set_matrix();

GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLvoid Timer(int value);
GLvoid Keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv)
{
	srand(time(NULL));

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(CLIENT_WIDTH, CLIENT_HIEGHT);
	glutCreateWindow("practice15");

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
	//glEnable(GL_CULL_FACE);


	// 쉐이더 만들고 경로 출력할 버퍼 만들기
	init_shader_program(&shader_id, "VertexShader_15.txt", "FragmentShader_15.txt");
	init_buffers();
	
	glUseProgram(shader_id);
		
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


void init_buffers() {

	{
		glGenVertexArrays(1, &VAO_cube);
		glGenBuffers(1, &VBO_position_cube);
		glGenBuffers(1, &VBO_color_cube);

		glBindVertexArray(VAO_cube);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPostionBuffer), cubeVertexPostionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(shader_id, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexColorBuffer), cubeVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(shader_id, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_pyramid);
		glGenBuffers(1, &VBO_position_pyramid);
		glGenBuffers(1, &VBO_color_pyramid);

		glBindVertexArray(VAO_pyramid);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositionBuffer), pyramidVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(shader_id, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexColorBuffer), pyramidVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(shader_id, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);
	}
}

void set_matrix()
{
	using namespace glm;

	modelLocation = glGetUniformLocation(shader_id, "modelTransform");
	viewLocation = glGetUniformLocation(shader_id, "viewTransform");
	projectionLocation = glGetUniformLocation(shader_id, "projectionTransform");

	world_transform = glm::rotate(mat4(1), radians(30.0f), vec3(1, 1, 0));

	cube_translate = translate(mat4(1), vec3(-1,0,0));
	cube_rotate = mat4(1);
	cube_transform = world_transform * cube_translate * cube_rotate;

	cube_up_rotate = mat4(1);
	cube_up_transform = cube_transform;

	cube_front_rotate = mat4(1);
	cube_front_transform = cube_transform;

	pyramid_translate = translate(mat4(1), vec3(1, 0, 0));
	pyramid_rotate = mat4(1);
	pyramid_transform = world_transform * pyramid_translate * pyramid_rotate;

	pyramid1_rotate = mat4(1);
	pyramid1_transform = pyramid_transform;

	pyramid2_rotate = mat4(1);
	pyramid2_transform = pyramid_transform;

	pyramid3_rotate = mat4(1);
	pyramid3_transform = pyramid_transform;

	pyramid4_rotate = mat4(1);
	pyramid4_transform = pyramid_transform;



	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cube_transform));


	cameraPos = glm::vec3(0, 0, 5);
	cameraDirection = glm::vec3(0, 0, -1);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


	projection = glm::translate(mat4(1), vec3(0, 0, -0.5)) * 
		glm::perspective(glm::radians(45.0f), (float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 100.0f);
	
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}

GLvoid drawScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 그릴 때 움직이는 부분마다 다른 매트릭스를 이용해 그린다.
	// 육면체, 윗면, 앞면
	// 사각뿔, 옆면1,2,3,4;

	glBindVertexArray(VAO_cube);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cube_transform));
	glDrawArrays(GL_TRIANGLES, 6, 6);
	glDrawArrays(GL_TRIANGLES, 12, 6);
	glDrawArrays(GL_TRIANGLES, 18, 6);
	glDrawArrays(GL_TRIANGLES, 30, 6);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cube_up_transform));
	glDrawArrays(GL_TRIANGLES, 0, 6);	//윗면

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cube_front_transform));
	glDrawArrays(GL_TRIANGLES, 24, 6);	//앞면


	glBindVertexArray(VAO_pyramid);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid_transform));
	glDrawArrays(GL_TRIANGLES, 12, 6);


	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid1_transform));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid2_transform));
	glDrawArrays(GL_TRIANGLES, 3, 3);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid3_transform));
	glDrawArrays(GL_TRIANGLES, 6, 3);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pyramid4_transform));
	glDrawArrays(GL_TRIANGLES, 9, 3);


	glutSwapBuffers();
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);		// 화면에 최종적으로 연결된 영역 설정
}

GLvoid Timer(int value)
{
	using namespace glm;
	if (cube_animation) {
		cube_up_rotate = glm::rotate(cube_up_rotate, glm::radians(10.0f), glm::vec3(1, 0, 0));

		cube_up_transform = cube_transform * glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0)) *
			cube_up_rotate * glm::translate(glm::mat4(1), glm::vec3(0, -0.5, 0));
	}
	if (cube_open) {
		if (c_opening) {
			cube_front_angle += 10;
			if (cube_front_angle == 90) {
				c_opening = false;
			}
		}
		else {
			cube_front_angle -= 10;
			if (cube_front_angle == 0) {
				c_opening = true;
			}
		}

		cube_front_rotate = glm::rotate(glm::mat4(1), radians(cube_front_angle), glm::vec3(1, 0, 0));
		cube_front_transform = cube_transform * translate(mat4(1), vec3(0, -0.5, 0.5))
			* cube_front_rotate * translate(mat4(1), vec3(0, 0.5, -0.5));

	}
	if (pyramid_open) {
		if (p_opening) {
			pyramid_angle += 10;
			if (pyramid_angle == 120) {
				p_opening = false;
			}
		}
		else {
			pyramid_angle -= 10;
			if (pyramid_angle == 0) {
				p_opening = true;
			}
		}
		pyramid1_rotate = glm::rotate(glm::mat4(1), radians(-pyramid_angle), glm::vec3(0, 0, 1));
		pyramid1_transform = pyramid_transform * translate(mat4(1), vec3(0.5, -0.5, 0))
			* pyramid1_rotate * translate(mat4(1), vec3(-0.5, 0.5, 0));

		pyramid2_rotate = glm::rotate(glm::mat4(1), radians(-pyramid_angle), glm::vec3(1, 0, 0));
		pyramid2_transform = pyramid_transform * translate(mat4(1), vec3(0, -0.5, -0.5))
			* pyramid2_rotate * translate(mat4(1), vec3(0, 0.5, 0.5));

		pyramid3_rotate = glm::rotate(glm::mat4(1), radians(pyramid_angle), glm::vec3(0, 0, 1));
		pyramid3_transform = pyramid_transform * translate(mat4(1), vec3(-0.5, -0.5, 0))
			* pyramid3_rotate * translate(mat4(1), vec3(0.5, 0.5, 0));


		pyramid4_rotate = glm::rotate(glm::mat4(1), radians(pyramid_angle), glm::vec3(1, 0, 0));
		pyramid4_transform = pyramid_transform * translate(mat4(1), vec3(0, -0.5, 0.5))
			* pyramid4_rotate* translate(mat4(1), vec3(0, 0.5, -0.5));

	}

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


	glutPostRedisplay();
	glutTimerFunc(70, Timer, 0);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	using namespace glm;
	switch (key) {
		// y축에 대하여 자전
	case 'Y':
	case 'y':
		cube_rotate = glm::rotate(cube_rotate, glm::radians(10.0f), glm::vec3(0, 1, 0));
		cube_transform = world_transform * cube_translate * cube_rotate;

		cube_up_transform = cube_transform * glm::translate(glm::mat4(1), glm::vec3(0, 0.5, 0)) *
			cube_up_rotate * glm::translate(glm::mat4(1), glm::vec3(0, -0.5, 0));
		cube_front_transform = cube_transform * translate(mat4(1), vec3(0, -0.5, 0.5))
			* cube_front_rotate * translate(mat4(1), vec3(0, 0.5, -0.5));

		pyramid_rotate = glm::rotate(cube_rotate, glm::radians(10.0f), glm::vec3(0, 1, 0));
		pyramid_transform = world_transform * pyramid_translate * pyramid_rotate;
		pyramid1_transform = pyramid_transform * translate(mat4(1), vec3(0.5, -0.5, 0))
			* pyramid1_rotate * translate(mat4(1), vec3(-0.5, 0.5, 0));
		pyramid2_transform = pyramid_transform * translate(mat4(1), vec3(0, -0.5, -0.5))
			* pyramid2_rotate * translate(mat4(1), vec3(0, 0.5, 0.5));
		pyramid3_transform = pyramid_transform * translate(mat4(1), vec3(-0.5, -0.5, 0))
			* pyramid3_rotate * translate(mat4(1), vec3(0.5, 0.5, 0));
		pyramid4_transform = pyramid_transform * translate(mat4(1), vec3(0, -0.5, 0.5))
			* pyramid4_rotate * translate(mat4(1), vec3(0, 0.5, -0.5));


		break;
		// 육면체 윗면 애니메이션 시작
	case 'T':
	case 't':
		if (cube_animation) cube_animation = false;
		else cube_animation = true;
		break;

		// 육면체 앞면 열기/닫기
	case 'F':
	case 'f':
		if (cube_open) cube_open = false;
		else cube_open = true;
		break;

		// 사각뿔 열림/닫힘
	case 'O':
	case 'o':
		if (pyramid_open) pyramid_open = false;
		else pyramid_open = true;
		break;

		// 직각투영/원근투영
	case 'P':
	case 'p':
		if (perspective_projection) {
			// 직각 투영으로 바꾸기
			projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 50.0f);
			//projection = glm::mat4(1);

			perspective_projection = false;
		}
		else {
			projection = glm::perspective(glm::radians(45.0f),
				(float)CLIENT_WIDTH / (float)CLIENT_HIEGHT, 0.1f, 50.0f);
			perspective_projection = true;
		}

		//projection = glm::translate(projection, vec3(0, 0, -0.5));

		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		break;

	// test
	case 'z':
		//cameraDirection += vec3(1, -1, 0);
		//view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		break;
	}

	view = glm::mat4(1);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);


}
