#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

#define M_PI 3.14592


// 경로와 도형을 선택할 수 있다
// 경로: 사인 곡선, 스프링 곡선, 지그재그
// 도형: 2차원 객체, 3차원 객체

//변환: 도형에 다음의 변환을 적용한다.
// 1. 이동: 경로를 따라 이동
// 2. 신축: 확대 축소를 반복함
// 3. 회전: 경로는 기준 좌표축의 y축을 기준으로 자전 (좌표계는 화전하지 않음)



// 전역 변수
GLint ShaderProgramID;
GLuint VAO_grid, VBO_position_grid, VBO_color_grid;
GLuint VAO_sincurve, VBO_position_sincurve, VBO_color_sincurve;
GLuint VAO_springcurve, VBO_position_springcurve, VBO_color_springcurve;
GLuint VAO_zigzagcurve, VBO_position_zigzagcurve, VBO_color_zigzagcurve;

GLuint VAO_cube, VBO_position_cube, VBO_color_cube, EBO_cube;
GLuint VAO_triangle, VBO_position_triangle, VBO_color_triangle;


glm::mat4 GridTransformMatrix = glm::mat4(1.0f);
glm::mat4 CurveTransformMatrix = glm::mat4(1.0f);
glm::mat4 ShapeTransformMatrix = glm::mat4(1.0f);

glm::mat4 ShapeTranslateMatrix = glm::mat4(1.0f);
glm::mat4 ShapeScaleMatrix = glm::mat4(1.0f);
glm::mat4 ShapeRotateMatrix = glm::mat4(1.0f);


unsigned int transformLocation;

int path = 0;
int shape = 0;

int position_on_line = 0;
float scalecycle = 0;
float angle = 0;

bool pathrot = true;
bool shapescale = true;
bool shapetranslate = true;

float gridVertexPositionBuffer[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
};
float gridVertexColorBuffer[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
};


float sincurve[150];
float springcurve[150];
float zigzag[150];

float pathcolor[150];



float cubeVertexPositionBuffer[] = {
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
};
float cubeVertexColorBuffer[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
};
unsigned int cubeIndex[36] = {
	0, 1, 2,
	3, 0, 2,
	3, 2, 7,
	2, 6, 7,
	6, 2, 1,
	6, 1, 5,
	5, 1, 0,
	5, 0, 4,
	4, 0, 7,
	7, 0, 3,
	5, 4, 7,
	5, 7, 6
};

float triangleVertexPositionBuffer[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
};
float triangleVertexColorBuffer[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
};



char* fileToBuf(const char* filename) {
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(filename, "rb");
	if (!fptr) return nullptr;

	fseek(fptr, 0, SEEK_END);	//파일의 끝까지 찾는다
	length = ftell(fptr);		// 파일 포인터의 현재 위치 가져옴
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, sizeof(char), length, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}
GLboolean InitShaderProgram();
GLvoid InitBuffer();
void fillCurvePoint();
void SetMatrix();




GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);

void drawGrid();
void drawSinCurve();
void drawSpringCurve();
void drawZigZagCurve();
void drawCube();
void drawTriangle();

void main(int argc, char** argv)
{
	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(400, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Practice11");

	// GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << std::endl;


	glLineWidth(2);
	fillCurvePoint();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);


	InitShaderProgram();
	InitBuffer();
	SetMatrix();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(60, Timer, 0);

	glutMainLoop();
}


GLboolean InitShaderProgram()
{
	using namespace std;

	GLint result;
	GLchar errorLog[512];


	const GLchar* vertexShaderSource = fileToBuf("VertexShader_11.txt");
	if (vertexShaderSource == nullptr) {
		cout << "VertexShaderSource 읽기 실패" << endl;
	}

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일실패\n" << errorLog << endl;
		return false;
	}

	const GLchar* fragmentShaderSource = fileToBuf("FragmentShader_11.txt");
	if (fragmentShaderSource == nullptr) {
		cout << "FragmentShaderSource 읽기 실패" << endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일실패\n" << errorLog << endl;
		return false;
	}

	ShaderProgramID = glCreateProgram();

	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
		return false;
	}

	return true;
}
GLvoid InitBuffer() {
	{
		glGenVertexArrays(1, &VAO_grid);
		glGenBuffers(1, &VBO_position_grid);
		glGenBuffers(1, &VBO_color_grid);

		glBindVertexArray(VAO_grid);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_grid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertexPositionBuffer), gridVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_grid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertexColorBuffer), gridVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_sincurve);
		glGenBuffers(1, &VBO_position_sincurve);
		glGenBuffers(1, &VBO_color_sincurve);

		glBindVertexArray(VAO_sincurve);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_sincurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sincurve), sincurve, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_sincurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pathcolor), pathcolor, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_springcurve);
		glGenBuffers(1, &VBO_position_springcurve);
		glGenBuffers(1, &VBO_color_springcurve);

		glBindVertexArray(VAO_springcurve);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_springcurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(springcurve), springcurve, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_springcurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pathcolor), pathcolor, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_zigzagcurve);
		glGenBuffers(1, &VBO_position_zigzagcurve);
		glGenBuffers(1, &VBO_color_zigzagcurve);

		glBindVertexArray(VAO_zigzagcurve);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_zigzagcurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(zigzag), zigzag, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_zigzagcurve);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pathcolor), pathcolor, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_cube);
		glGenBuffers(1, &VBO_position_cube);
		glGenBuffers(1, &VBO_color_cube);
		glGenBuffers(1, &EBO_cube);

		glBindVertexArray(VAO_cube);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositionBuffer), cubeVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndex), cubeIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexColorBuffer), cubeVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

	{
		glGenVertexArrays(1, &VAO_triangle);
		glGenBuffers(1, &VBO_position_triangle);
		glGenBuffers(1, &VBO_color_triangle);

		glBindVertexArray(VAO_triangle);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexPositionBuffer), triangleVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexColorBuffer), triangleVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}

}

void SetMatrix() {
	ShapeScaleMatrix = glm::scale(ShapeScaleMatrix, glm::vec3(0.25, 0.25, 0.25));

}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// 흰색 바탕
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 그리기
	glUseProgram(ShaderProgramID);

	drawGrid();

	switch (path) {
	case 0:
		drawSinCurve();
		break;
	case 1:
		drawSpringCurve();
		break;
	case 2:
		drawZigZagCurve();
		break;
	}
	switch (shape) {
	case 0:
		drawCube();
		break;
	case 1:
		drawTriangle();
		break;
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {

	case '1':
		path = 0;
		break;

	case '2':
		path = 1;
		break;

	case '3':
		path = 2;
		break;

	case 'q':
		shape = 0;
		break;
	case 'w':
		shape = 1;
		break;

	case 'a':
		if (pathrot) {
			pathrot = false;
		}
		else pathrot = true;

		break;
	case 's':
		if (shapescale) {
			shapescale = false;
		}
		else shapescale = true;
		break;
	case 'd':
		if (shapetranslate) {
			shapetranslate = false;
		}
		else shapetranslate = true;		break;

	case 'x':
		// tmp
		CurveTransformMatrix = glm::rotate(CurveTransformMatrix, glm::radians(15.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		break;
	}

}

GLvoid Timer(int value) {
	if (pathrot) {
		CurveTransformMatrix = glm::rotate(CurveTransformMatrix, glm::radians(15.0f), glm::vec3(0.0f, 0.1f, 0.0f));
	}
	
	if (shapescale) {
		angle += 30.0f;
		scalecycle = glm::sin(glm::radians(angle));

		if (scalecycle > 0) {
			ShapeScaleMatrix = glm::scale(ShapeScaleMatrix, glm::vec3(1.1, 1.1, 1.1));
		}
		else {
			ShapeScaleMatrix = glm::scale(ShapeScaleMatrix, glm::vec3(0.9, 0.9, 0.9));
		}
	}

	if (shapetranslate) {
		position_on_line = (position_on_line + 1) % 50;
		if (path == 0) {
			ShapeTranslateMatrix = glm::translate(glm::mat4(1.0f),
				glm::vec3(sincurve[position_on_line * 3], sincurve[position_on_line * 3 + 1], sincurve[position_on_line * 3 + 2]));

		}
		else if (path == 1) {
			ShapeTranslateMatrix = glm::translate(glm::mat4(1.0f),
				glm::vec3(springcurve[position_on_line * 3], springcurve[position_on_line * 3 + 1], springcurve[position_on_line * 3 + 2]));

		}
		else if (path == 2) {
			ShapeTranslateMatrix = glm::translate(glm::mat4(1.0f),
				glm::vec3(zigzag[position_on_line * 3], zigzag[position_on_line * 3 + 1], zigzag[position_on_line * 3 + 2]));

		}
	}


	glutPostRedisplay();
	glutTimerFunc(60, Timer, 0);
}

GLvoid Mouse(int button, int state, int x, int y) {

}

void drawGrid()
{
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(GridTransformMatrix));

	glBindVertexArray(VAO_grid);
	glDrawArrays(GL_LINES, 0, 6);
}

void fillCurvePoint()
{
	const int NUM_OF_POINTS = 50;
	float angle = glm::radians(15.0f);

	for (int i = 0; i < NUM_OF_POINTS; i++) {
		int index = i * 3;
		

		sincurve[index + 0] = ((float)i / 50.f) - 0.5;
		sincurve[index + 1] = glm::sin(angle * i) / 2;
		sincurve[index + 2] = 0;

		springcurve[index + 0] = ((float)i / 50.f) - 0.5;
		springcurve[index + 1] = glm::sin(angle * i) / 2;
		springcurve[index + 2] = glm::cos(angle * i) / 2;
		
		zigzag[index + 0] = ((float)i / 50.f) - 0.5;

		if (i % 10 < 5) {
			zigzag[index + 1] = (i % 5) / 5.0f - 0.5f;
		}
		else {
			zigzag[index + 1] = (5 - (i % 5)) / 5.0f - 0.5f;
		}
		zigzag[index + 2] = 0;


		pathcolor[index + 0] = 0.0f;
		pathcolor[index + 1] = 0.0f;
		pathcolor[index + 2] = 0.0f;
	}
}

void drawSinCurve()
{
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(CurveTransformMatrix));

	glBindVertexArray(VAO_sincurve);
	glDrawArrays(GL_LINE_STRIP, 0, 50);
}

void drawSpringCurve()
{
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(CurveTransformMatrix));

	glBindVertexArray(VAO_springcurve);
	glDrawArrays(GL_LINE_STRIP, 0, 50);

}

void drawZigZagCurve() {
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(CurveTransformMatrix));

	glBindVertexArray(VAO_zigzagcurve);
	glDrawArrays(GL_LINE_STRIP, 0, 50);

}

void drawCube() {
	ShapeTransformMatrix = CurveTransformMatrix * ShapeTranslateMatrix * ShapeScaleMatrix * ShapeRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(ShapeTransformMatrix));

	glBindVertexArray(VAO_cube);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawTriangle() {
	ShapeTransformMatrix = CurveTransformMatrix * ShapeTranslateMatrix * ShapeScaleMatrix * ShapeRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(ShapeTransformMatrix));

	glBindVertexArray(VAO_triangle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
