#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>


// 전역변수-----------------------------------------------
GLuint ClientWidth = 800, ClientHeight = 800;
GLuint ShaderProgramID;

GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;

int timerValue = 0;

GLuint VAO_grid, VBO_position_grid, VBO_color_grid;
GLuint VAO_cube, VBO_position_cube, VBO_color_cube, EBO_cube;
GLuint VAO_pyramid, VBO_position_pyramid, VBO_color_pyramid, EBO_pyramid;

glm::mat4 GridTransformMatrix = glm::mat4(1.0f);
glm::mat4 CubeTransformMatrix = glm::mat4(1.0f);
glm::mat4 PyramidTransformMatrix = glm::mat4(1.0f);


glm::mat4 cubeScaleMatrix = glm::mat4(1.0f);
glm::mat4 cubeRotateMatrix = glm::mat4(1.0f);
glm::mat4 cubeTranslateMatrix = glm::mat4(1.0f);

glm::mat4 pyramidScaleMatrix = glm::mat4(1.0f);
glm::mat4 pyramidRotateMatrix = glm::mat4(1.0f);
glm::mat4 pyramidTranslateMatrix = glm::mat4(1.0f);

unsigned int transformLocation;
bool otherShape = false;

GLUquadricObj* sphere;
GLUquadricObj* cylinder;


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

float pyramidVertexPositionBuffer[] = {
	0, 0.5, 0,
	0.5, -0.5, 0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, -0.5, 0.5
};
float pyramidVertexColorBuffer[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.1f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.1f, 0.0f,
	0.0f, 1.0f, 1.0f,
};

unsigned int pyramidIndex[18] = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 4,
	0, 4, 1,
	1, 4, 3,
	1, 3, 2,
};



//----------------------------------------------------------
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

GLboolean InitShaderProgram()
{
	using namespace std;

	GLint result;
	GLchar errorLog[512];


	const GLchar* vertexShaderSource = fileToBuf("VertexShader_12.txt");
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

	const GLchar* fragmentShaderSource = fileToBuf("FragmentShader_12.txt");
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

	//GLint newShaderProgram = glCreateProgram();
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


	//*ShaderProgramID = newShaderProgram;
	return true;
}

GLvoid InitBuffer() {
	// 버퍼 생성
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
		glGenVertexArrays(1, &VAO_pyramid);
		glGenBuffers(1, &VBO_position_pyramid);
		glGenBuffers(1, &VBO_color_pyramid);
		glGenBuffers(1, &EBO_pyramid);

		glBindVertexArray(VAO_pyramid);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositionBuffer), pyramidVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_pyramid);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndex), pyramidIndex, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexColorBuffer), pyramidVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);

	}
	
}



GLvoid SetMatrix() {
	GridTransformMatrix = glm::rotate(GridTransformMatrix, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
	GridTransformMatrix = glm::rotate(GridTransformMatrix, glm::radians(15.0f), glm::vec3(1.0, 0.0, 0.0));
	
	cubeScaleMatrix = glm::scale(cubeScaleMatrix, glm::vec3(0.25, 0.25, 0.25));
	cubeTranslateMatrix = glm::translate(cubeTranslateMatrix, glm::vec3(0.5, 0.0, 0.0));

	pyramidScaleMatrix = glm::scale(pyramidScaleMatrix, glm::vec3(0.25, 0.25, 0.25));
	pyramidTranslateMatrix = glm::translate(pyramidTranslateMatrix, glm::vec3(-0.5, 0.0, 0.0));


	transformLocation = glGetUniformLocation(ShaderProgramID, "modelTransform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(GridTransformMatrix));
}
//----------------------------------------------------------


// 함수 선언-------------------------------------------------

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid ChangeWindowSize(int w, int h);

void drawGrid();
void drawCube();
void drawPyramid();
void drawSphere();
void drawCylinder();

void main(int argc, char** argv)
{
	srand(time(NULL));

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(ClientWidth, ClientHeight);
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


	//glPointSize(20);
	glLineWidth(3);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	InitShaderProgram();
	glUseProgram(ShaderProgramID);

	InitBuffer();
	SetMatrix();

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);

	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_LINE);


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(ChangeWindowSize);
	glutTimerFunc(100, Timer, timerValue);
	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderProgramID);



	drawGrid();
	if (!otherShape) {
		drawCube();
		drawPyramid();
	}
	else {
		drawSphere();
		drawCylinder();
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	// 육면체를 x축에 대하여 회전
	case 'X':
	case 'x':
		cubeRotateMatrix = glm::rotate(cubeRotateMatrix, glm::radians(-15.0f), glm::vec3(1.0, 0.0, 0.0));
		break;

	// 피라미드를 y축에 대하여 회전
	case 'Y':
	case 'y':
		pyramidRotateMatrix = glm::rotate(pyramidRotateMatrix, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
		break;

	// 좌표계 전체를 y축에 대하여 회전
	case 'B':
	case 'b':
		timerValue = 1;
		break;

	// 두 도형을 다른 도형으로 바꿈
	case 'C':
	case 'c':
		if (otherShape) {
			otherShape = false;
		}
		else {
			otherShape = true;
		}
		break;

	// 회전 멈추기
	case 'S':
	case 's':
		timerValue = 0;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

GLvoid Timer(int value)
{
	if (value == 1) {
		GridTransformMatrix = glm::rotate(GridTransformMatrix, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
	}
	glutPostRedisplay();
	glutTimerFunc(100, Timer, timerValue);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON) return;
	if (state != GLUT_DOWN) return;

	//float mouse_x = ((float)x / ClientWidth * 2) - 1;
	//float mouse_y = -(((float)y / ClientHeight * 2) - 1);



	glutPostRedisplay();
}

GLvoid ChangeWindowSize(int w, int h)
{

}

void drawCube()
{
	CubeTransformMatrix = GridTransformMatrix * cubeTranslateMatrix * cubeScaleMatrix * cubeRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(CubeTransformMatrix));

	glBindVertexArray(VAO_cube);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawPyramid()
{
	PyramidTransformMatrix = GridTransformMatrix * pyramidTranslateMatrix * pyramidScaleMatrix * pyramidRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(PyramidTransformMatrix));

	glBindVertexArray(VAO_pyramid);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

}

void drawSphere()
{
	CubeTransformMatrix = GridTransformMatrix * cubeTranslateMatrix * cubeScaleMatrix * cubeRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(CubeTransformMatrix));

	gluSphere(sphere, 0.5, 20, 20);
}

void drawCylinder()
{
	PyramidTransformMatrix = GridTransformMatrix * pyramidTranslateMatrix * pyramidScaleMatrix * pyramidRotateMatrix;
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(PyramidTransformMatrix));

	gluCylinder(cylinder, 0.5, 0.5, 0.5, 20, 5);

}



void drawGrid()
{
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(GridTransformMatrix));

	glBindVertexArray(VAO_grid);
	glDrawArrays(GL_LINES, 0, 6);
}
