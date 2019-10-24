#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.141592
#define CIRCLENUM 10


// 전역변수 선언
GLuint ShaderProgramID;
GLuint ClientWidth = 800, ClientHeight = 800;
GLenum drawMode = GL_LINE_LOOP;

GLuint VAO[10], VBO_position[10], VBO_color[10];		// 10개의 VAO, VBO6
bool activeCircle[10] = { false, };		// 10개의 불리언 값으로 바꾸고, 활성화 된 애만 크기 바꾸고 그리기
int circleNum = 0;

// VertexArray도 10개 필요함
// 원을 그리는데 필요한 점의 개수 40개
// 인자의 개수 40 * 2 = 80개
float vertexPosition[10][80];
float vertexColor[10][120];
float radius[10];
float sinArray[40];
float cosArray[40];
float centerX[10], centerY[10];


void initSinCosValue() {
	float angle = 0;	// 라디안
	for (int i = 0; i < 40; i++) {
		sinArray[i] = sin(angle);
		cosArray[i] = cos(angle);
		angle += (float)(2 * PI / 40.0f);
	}
}

void initPosition(const int index, float centerx, float centery) {
	centerX[index] = centerx;
	centerY[index] = centery;

	for (int i = 0; i < 40; i++) {
		vertexPosition[index][i * 2] = centerx;
		vertexPosition[index][i * 2 + 1] = centery;
	}
}
void initColor() {
	for (int i = 0; i < CIRCLENUM; i++) {
		for (int j = 0; j < 40; j++) {
			vertexColor[i][j * 3] = 1.0f;
			vertexColor[i][j * 3 + 1] = 0.0f;
			vertexColor[i][j * 3 + 2] = 0.0f;
		}
	}
}

void updatePosition(const int index) {
	if (radius[index] > 1) radius[index] = 0;
	else radius[index] += 0.0000002;

	for (int i = 0; i < 40; i++) {
		vertexPosition[index][i * 2] = centerX[index] + radius[index] * cosArray[i];		// x값
		vertexPosition[index][i * 2 + 1] = centerY[index] + radius[index] * sinArray[i];	// y값
	}
	if (index % 2 == 0) {
		//둘중에 하나는 흐려짐
		for (int j = 0; j < 40; j++) {
			//vertexColor[index][j * 3] = 1.0f;
			vertexColor[index][j * 3 + 1] = radius[index];
			vertexColor[index][j * 3 + 2] = radius[index];
		}
	}
}



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

	// Vertex Shader 소스 파일 읽기
	const GLchar* vertexShaderSource =
		fileToBuf("VertexShader_5.txt");
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
		fileToBuf("FragmentShader_5.txt");
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
	return ShaderProgramID;
}
GLvoid CreateVAO() {
	for (int i = 0; i < CIRCLENUM; i++) {
		glGenVertexArrays(1, &VAO[i]);	// VAO 객체 생성
	}

	//glBindVertexArray(VAO);		// 어차피 VAO 한개니까 바인드도 해줌
}
GLvoid CreateVBO(const int index) {

	// glBindBuffer: 버퍼 오브젝트 이름을 바인드한다.
	// glBufferData: 버퍼 오프젝트의 테이터를 생성
	
	// 현재 점의 속성이 3개로 셰이더 프로그램 안에서 계산하는거 해보기

	// 위치
	glGenBuffers(1, &VBO_position[index]);					// 버퍼 만들기
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[index]);		// 바인드 해주기
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertexPosition[index]), vertexPosition[index], GL_DYNAMIC_DRAW);
	// 데이터가 그려질 때마다 값이 변경되는 모드

	GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos"); 
	glVertexAttribPointer(pAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);	// 버텍스 속성 데이터에 배열을 정의
	glEnableVertexAttribArray(pAttribute);	// 버텍스 속성 배열을 사용하도록 한다

	// 반지름
	glGenBuffers(1, &VBO_color[index]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor[index]), vertexColor[index], GL_DYNAMIC_DRAW);
	GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);	// 버텍스 속성 데이터에 배열을 정의
	glEnableVertexAttribArray(cAttribute);	// 버텍스 속성 배열을 사용하도록 한다

	// glGetAttribLocation(): 속성 변수의 위치를 가져온다
	// 현재 바인딩되어있는 VBO를 shader program이랑 연결하는 부분
}



GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid ChangeWindowSize(int w, int h);
GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;



void main(int argc, char** argv)
{
	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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

;

	InitShader();
	glUseProgram(ShaderProgramID);

	glPointSize(3);
	glLineWidth(2);
	initColor();
	initSinCosValue();

	CreateVAO();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(ChangeWindowSize);
	glutTimerFunc(0, Timer, 0);
	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);

	// 버퍼의 특정 부분 채우기도 가능
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), &vertexPosition);;
	// 이건 버퍼 전체 채우기
	// 원마다 다른 버퍼를 사용하게 되면 바인딩 하는 버퍼를 계속 바꿔줘야함
	// 모든 원이 같은 위치 버퍼는

	for (int i = 0; i < CIRCLENUM; i++) {
		if (activeCircle[i] == false)
			continue;
		// VAO 바인딩 함
		glBindVertexArray(VAO[i]);


		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition[i]), vertexPosition[i], GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor[i]), vertexColor[i], GL_DYNAMIC_DRAW);

		// 배열로부터 프리미티브 렌더링하기
		glDrawArrays(drawMode, 0, 40);	// 점 40개
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
	case '1':
		// 점으로 그리기
		drawMode = GL_POINTS;
		break;
	case '2':
		// 선으로 그리기
		drawMode = GL_LINE_LOOP;
		break;
	default:
		break;
	}
}

GLvoid Timer(int value)
{
	for (int i = 0; i < CIRCLENUM; i++) {
		if (activeCircle[i] == false)
			continue;
		updatePosition(i);				// 현재 그려야하는 원의 점 좌표 모두 업데이트
	}

	glutPostRedisplay();		// 다시 그리기
	glutTimerFunc(0, Timer, 0);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
	// 활성화 되지 않은 원을 찾기
	circleNum = -1;
	for (int i = 0; i < CIRCLENUM; i++) {
		if (activeCircle[i] == false) {
			circleNum = i;
			break;
		}
	}
	if (circleNum  < 0) return;		// 빈 원이 없고 꽉 찬 것
	
	// 윈도우 마우스 좌표 -> GL 뷰포트 좌표
	float mouseX = ((float)x / ClientWidth * 2) - 1;
	float mouseY = -(((float)y / ClientHeight * 2) - 1);
	
	glBindVertexArray(VAO[circleNum]);			// VAO를 바이딩 한 다음에
	initPosition(circleNum, mouseX, mouseY);	// 마우스 좌표를 버퍼에 저장함
	CreateVBO(circleNum);						// 그 버퍼값을 가지는 VBO를 생성함
	activeCircle[circleNum] = true;

	circleNum += 1;
}

GLvoid ChangeWindowSize(int w, int h)
{
}