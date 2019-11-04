#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#define M_PI 3.141592

// 전역변수-----------------------------------------------
GLuint ShaderProgramID;
GLuint GridShader;
GLuint ClientWidth = 800, ClientHeight = 800;

GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;

GLuint VAO_cube, VBO_position_cube, VBO_color_cube, EBO_cube;
GLuint VAO_pyramid, VBO_position_pyramid, VBO_color_pyramid, EBO_pyramid;

glm::mat4 translateMatrix = glm::mat4(1.0f);	// 단위행열로 초기화
glm::mat4 rotateMatrix = glm::mat4(1.0f);	// 단위행열로 초기화
glm::mat4 rotateyMatrix = glm::mat4(1.0f);	// 단위행열로 초기화

// 변환이 얼만큼 이루어졌나 저장하고 싶다
float tanslate_x = 0;
float translate_y = 0;
float angle = 0;

GLUquadricObj* pobj;
// 1구 2y원뿔 3z원뿔 4디스크 5실린더
int shape = 1;
bool drawSolid = true;

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
GLuint InitShader() {
	using namespace std;

	{
		// Vertex Shader 소스 파일 읽기
		const GLchar* vertexShaderSource =
			fileToBuf("VertexShader_10.txt");
		if (vertexShaderSource == nullptr) {
			cout << "VertexShaderSource 읽기 실패" << endl;
		}

		// Vertex Shader 객체 생성하고 컴파일 하기
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
			cerr << "ERROR: vertex shader 컴파일실패\n" << errorLog << endl;
			return false;
		}

		// Fragment Shader 소스 파일 읽기
		const GLchar* fragmentShaderSource =
			fileToBuf("FragmentShader_10.txt");
		if (fragmentShaderSource == nullptr) {
			cout << "FragmentShaderSource 읽기 실패" << endl;
		}

		//Fragment Shader 객체 생성하고 컴파일 하기
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


		// 링크하기
		glAttachShader(ShaderProgramID, vertexShader);
		glAttachShader(ShaderProgramID, fragmentShader);
		glLinkProgram(ShaderProgramID);
		glDeleteShader(vertexShader);
		glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);

		if (!result) {
			glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
			cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
			return false;
		}
		glUseProgram(ShaderProgramID);

	}

	{
		// Vertex Shader 소스 파일 읽기
		const GLchar* vertexShaderSource =
			fileToBuf("GridVertexShader.txt");
		if (vertexShaderSource == nullptr) {
			cout << "GridVertexShaderSource 읽기 실패" << endl;
		}

		// Vertex Shader 객체 생성하고 컴파일 하기
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
			cerr << "ERROR: vertex shader 컴파일실패\n" << errorLog << endl;
			return false;
		}

		// Fragment Shader 소스 파일 읽기
		const GLchar* fragmentShaderSource =
			fileToBuf("GridFragmentShader.txt");
		if (fragmentShaderSource == nullptr) {
			cout << "FragmentShaderSource 읽기 실패" << endl;
		}

		//Fragment Shader 객체 생성하고 컴파일 하기
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

		if (!result) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
			cerr << "ERROR: fragment shader 컴파일실패\n" << errorLog << endl;
			return false;
		}
		GridShader = glCreateProgram();


		// 링크하기
		glAttachShader(GridShader, vertexShader);
		glAttachShader(GridShader, fragmentShader);
		glLinkProgram(GridShader);
		glDeleteShader(GridShader);
		glGetProgramiv(GridShader, GL_LINK_STATUS, &result);

		if (!result) {
			glGetProgramInfoLog(GridShader, 512, NULL, errorLog);
			cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
			return false;
		}
		glUseProgram(GridShader);

	}
}
GLvoid InitBuffer() {
	// 버퍼 생성
	//{
	//	glGenVertexArrays(1, &VAO_cube);
	//	glGenBuffers(1, &VBO_position_cube);
	//	glGenBuffers(1, &VBO_color_cube);
	//
	//	glBindVertexArray(VAO_cube);
	//
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPostionBuffer), cubeVertexPostionBuffer, GL_STATIC_DRAW);
	//	GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
	//	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//	glEnableVertexAttribArray(pAttribute);
	//
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_color_cube);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexColorBuffer), cubeVertexColorBuffer, GL_STATIC_DRAW);
	//	GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
	//	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//	glEnableVertexAttribArray(cAttribute);
	//
	//}
	//
	//rotMatrix = glm::rotate(rotMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//rotMatrix = glm::rotate(rotMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));
}
//----------------------------------------------------------


// 함수 선언-------------------------------------------------

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid specialkey(int key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid ChangeWindowSize(int w, int h);

void drawGrid();
void initailizeTransform();

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

	// 필요한 초기 설정
	glUseProgram(ShaderProgramID);



	// 깊이 검사
	//glEnable(GL_DEPTH_TEST);

	// 컬링
	//glFrontFace(GL_CCW);		// 반시계방향이 앞면
	//glEnable(GL_CULL_FACE);

	//glPointSize(10);

	InitShader();
	//InitBuffer();


	// 2차 곡면 객체 만들기
	pobj = gluNewQuadric();
	gluQuadricDrawStyle(pobj, GLU_FILL);

	//gluQuadricNormals(pobj, GLU_SMOOTH);
	//gluQuadricOrientation(pobj, GLU_OUTSIDE);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(specialkey);
	glutMouseFunc(Mouse);
	glutReshapeFunc(ChangeWindowSize);
	glutTimerFunc(0, Timer, 0);
	glutMainLoop();
}



GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(GridShader);
	drawGrid();

	glUseProgram(ShaderProgramID);

	//unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "transform");
	//glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformMatrix));

	unsigned int translateLocation = glGetUniformLocation(ShaderProgramID, "translate");
	glUniformMatrix4fv(translateLocation, 1, GL_FALSE, glm::value_ptr(translateMatrix));

	unsigned int rotateLocation = glGetUniformLocation(ShaderProgramID, "rotate");
	glUniformMatrix4fv(rotateLocation, 1, GL_FALSE, glm::value_ptr(rotateMatrix));

	unsigned int rotateyLocation = glGetUniformLocation(ShaderProgramID, "rotatey");
	glUniformMatrix4fv(rotateyLocation, 1, GL_FALSE, glm::value_ptr(rotateyMatrix));


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (shape) {
	case 1:
		gluSphere(pobj, 0.5, 30, 30);
		break;
	case 2:
		gluCylinder(pobj, 0.5, 0, 1, 30, 8);
		break;
	case 3:
		// 뾰족한 부분이 y축으로 오게 변환해라
		gluCylinder(pobj, 0.5, 0, 1, 30, 8);

		break;
	case 4:
		gluCylinder(pobj, 0.3, 0.3, 0.5, 30, 8);
		break;
	case 5:
		gluDisk(pobj, 0.2, 0.5, 20, 3);
		break;
	}


	

	//// 그리기
	//if (drawcube) {
	//	glBindVertexArray(VAO_cube);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPostionBuffer), cubeVertexPostionBuffer, GL_STATIC_DRAW);
	//
	//	unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
	//	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));
	//
	//	glDrawArrays(drawMode, 0, 36);		// 마지막 인자: 점의 개수
	//
	//}
	//else {
	//	glBindVertexArray(VAO_pyramid);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_position_pyramid);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositionBuffer), pyramidVertexPositionBuffer, GL_STATIC_DRAW);
	//
	//	unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
	//	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));
	//
	//	glDrawArrays(drawMode, 0, 18);		// 마지막 인자: 점의 개수
	//}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {

	// 구
	case '1':
	{
		shape = 1;
		initailizeTransform();

	}
	break;

	// 원뿔 뾰족한 부분이 z축
	case '2':
	{
		shape = 2;
		initailizeTransform();
	}
	break;

	// 원뿔 뾰족한 부분이 y축
	case '3':
	{
		shape = 3;
		initailizeTransform();
		rotateMatrix = glm::rotate(rotateMatrix, glm::radians(-90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));

	}
	break;

	// 디스크
	case '4':
	{
		shape = 4;
		initailizeTransform();

	}
	break;

	// 실린더
	case '5':
	{
		shape = 5;
		initailizeTransform();

	}
	break;

	// 와이어 객체/솔리드 객체
	case 'W':
	case 'w':
	{
		if (drawSolid) {
			gluQuadricDrawStyle(pobj, GLU_LINE);
			drawSolid = false;
		}
		else {
			gluQuadricDrawStyle(pobj, GLU_FILL);
			drawSolid = true;
		}
	}
	break;


	// y축을 기준으로 양/음 방향으로 회전
	case 'Y':
	case 'y':
	{
		angle += 15.0f;
		if (3 == shape) {
			rotateMatrix = glm::rotate(rotateMatrix, glm::radians(15.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
			rotateMatrix = glm::rotate(rotateMatrix, glm::radians(15.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
	}
		break;

	// 좌표계의 y축으로 공전
	case 'R':
	case 'r':
	{
		angle += 15.0f;

		if (shape == 3) {
			rotateMatrix = glm::rotate(rotateMatrix, glm::radians(15.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
			translateMatrix = glm::mat4(1.0f);

			//translateMatrix = glm::translate(translateMatrix, glm::vec3(tanslate_x * glm::cos(glm::radians(angle)),
			//	tanslate_x * glm::sin(glm::radians(angle)), 0.0f));

			translateMatrix = glm::translate(translateMatrix, glm::vec3(tanslate_x * glm::cos(glm::radians(angle)), translate_y,
				tanslate_x * glm::sin(glm::radians(angle))));


		}
		else {
			// 이동과 변환을 같이 해라
			rotateMatrix = glm::rotate(rotateMatrix, glm::radians(15.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			translateMatrix = glm::mat4(1.0f);

			translateMatrix = glm::translate(translateMatrix, glm::vec3(tanslate_x * glm::cos(glm::radians(angle)), translate_y,
				tanslate_x * glm::sin(glm::radians(angle))));
		}
	}
	break;

	// 모든 변환 리셋
	case 'C':
	case 'c':
	{
		initailizeTransform();

	}
	break;

	default:
		break;
	}
}

GLvoid specialkey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
	{
		translateMatrix = glm::translate(translateMatrix, glm::vec3(0.1f, 0.0f, 0.0f));
		tanslate_x += 0.1;
		angle = 0;
	}
	break;
	case GLUT_KEY_LEFT:
	{
		translateMatrix = glm::translate(translateMatrix, glm::vec3(-0.1f, 0.0f, 0.0f));
		tanslate_x += -0.1;
		angle = 0;

	}
	break;
	case GLUT_KEY_UP:
	{
		translateMatrix = glm::translate(translateMatrix, glm::vec3(0.0f, 0.1f, 0.0f));
		translate_y += 0.1f;
		angle = 0;

	}
	break;
	case GLUT_KEY_DOWN:
	{
		translateMatrix = glm::translate(translateMatrix, glm::vec3(0.0f, -0.1f, 0.0f));
		translate_y += -0.1f;
		angle = 0;


	}
	break;

	}
}
GLvoid Timer(int value)
{

	glutPostRedisplay();
	glutTimerFunc(0, Timer, 0);
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

void initailizeTransform() {
	translateMatrix = glm::mat4(1.0f);
	rotateMatrix = glm::mat4(1.0f);
	tanslate_x = 0;
	translate_y = 0;
	angle = 0.0f;
}


void drawGrid()
{
	glBegin(GL_LINES);
	glVertex2f(0.0f, 1.0f);
	glVertex2f(0.0f, -1.0f);
	glVertex2f(1.0f, 0.0f);
	glVertex2f(-1.0f, 0.0f);
	glEnd();
	glFinish();
}
