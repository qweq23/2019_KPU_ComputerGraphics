#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

float randmax = 32767;

GLuint ShaderProgramID;
GLuint ClientWidth = 800, ClientHeight = 600;
GLenum drawMode = GL_TRIANGLES;


float vertexPosition[] = {
	-0.5, 0.75, 0.0,
	-0.75, 0.25, 0.0,
	-0.25, 0.25, 0.0,
	0.5, 0.75, 0.0,
	0.75, 0.25, 0.0,
	0.25, 0.25, 0.0,
	-0.5, -0.25, 0.0,
	-0.75, -0.75, 0.0,
	-0.25, -0.75, 0.0,
	0.5, -0.25, 0.0,
	0.75, -0.75, 0.0,
	0.25, -0.75, 0.0,
};

float vertexColor[] = {
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
};


int index = 0;
int timerTrigger = 0; // 0이면 멈추고 1이면 감


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
		fileToBuf("VertexShader.txt");
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
		fileToBuf("FragmentShader.txt");
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

GLuint VAO, VBO_position, VBO_color;

GLvoid InitBuffer() {
	// 변수 선언

	// VAO 생성
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 위치 속성
	glGenBuffers(1, &VBO_position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertexPosition),
		vertexPosition, GL_STATIC_DRAW
	);

	// 색상 속성
	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertexColor),
		vertexColor, GL_STATIC_DRAW
	);

	// aPos 속성 변수에 값을 저장
	GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	// aColor 속성 변수 값을 저장
	GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);
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


	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(ChangeWindowSize);
	glutTimerFunc(0, Timer, timerTrigger);
	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);

	// 그리기
	glUseProgram(ShaderProgramID);
	glDrawArrays(
		drawMode,	// 렌더링할 도형 종류
		0,				// 배열에서 도형 시작 인덱스
		3				// 렝더링할 인덱스 개수
	);
	glDrawArrays(
		drawMode,	// 렌더링할 도형 종류
		3,				// 배열에서 도형 시작 인덱스
		3				// 렝더링할 인덱스 개수
	);
	glDrawArrays(
		drawMode,	// 렌더링할 도형 종류
		6,				// 배열에서 도형 시작 인덱스
		3				// 렝더링할 인덱스 개수
	);
	glDrawArrays(
		drawMode,	// 렌더링할 도형 종류
		9,				// 배열에서 도형 시작 인덱스
		3				// 렝더링할 인덱스 개수
	);
	


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'F':
	case 'f':
		// 도형 그리기 모드 변경
		if (drawMode == GL_TRIANGLES)
			drawMode = GL_LINE_LOOP;
		else drawMode = GL_TRIANGLES;
		break;
	case 'M':
	case 'm':
		// 반시계 방향 이동
		timerTrigger = 1;
		break;
	case 'S':
	case 's':
		// 멈추기
		timerTrigger = 0;
		break;
	case 'C':
	case 'c':
	{
		// 색상 바꾸기
		// 랜덤한 색깔을 4개 뽑아서
		// 컬러 버퍼에 인덱스 3개에 같은 값을 넣는다.
		int index = 0;

		for (int i = 0; i < 4; i++) {
			float r = rand() / randmax;
			float g = rand() / randmax;
			float b = rand() / randmax;

			for (int j = 0; j < 3; j++) {
				vertexColor[index] = r;
				vertexColor[index + 1] = g;
				vertexColor[index + 2] = b;
				index += 3;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
		glBufferData(
			GL_ARRAY_BUFFER, sizeof(vertexColor),
			vertexColor, GL_STATIC_DRAW
		);


		break;
	}
	case 'Q':
	case 'q':
		// 종료
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
	InitBuffer();
	glutPostRedisplay();
}

GLvoid Timer(int value)
{
	if (value == 1) {

		for (int i = 0; i < 4; i++) {
			// 삼각형이 어디 방향으로 가야하는지 판단

			float center_x = vertexPosition[i * 9];
			float center_y = vertexPosition[i * 9 + 1] - 0.25;


			if (center_x >= 0.5 && center_y < 0.5) {
				// 위로 가라
				for (int j = 0; j < 3; j++) {
					vertexPosition[i * 9 + j * 3 + 1] += 0.000001;
				}
			}
			if (center_y >= 0.5 && center_x > -0.5) {
				// 왼쪽으로 가라
				for (int j = 0; j < 3; j++) {
					vertexPosition[i * 9 + j * 3] -= 0.000001;
				}
			}
			if (center_x <= -0.5 && center_y > -0.5) {
				// 아래로 가라
				for (int j = 0; j < 3; j++) {
					vertexPosition[i * 9 + j * 3 + 1] -= 0.000001;
				}
			}
			if (center_y <= -0.5 && center_x < 0.5) {
				// 오른쪽으로 가라
				for (int j = 0; j < 3; j++) {
					vertexPosition[i * 9 + j * 3] += 0.000001;
				}
			}

		}

		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
		glBufferData(
			GL_ARRAY_BUFFER, sizeof(vertexPosition),
			vertexPosition, GL_STATIC_DRAW
		);
		


	}
	glutPostRedisplay();
	glutTimerFunc(0, Timer, timerTrigger);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON) return;
	if (state != GLUT_DOWN) return;

	float mouse_x = ((float)x / ClientWidth * 2) - 1;
	float mouse_y = -(((float)y / ClientHeight * 2) - 1);

	// 마우스 좌표를 버텍스 position에 저장
	vertexPosition[index++] = mouse_x;
	vertexPosition[index++] = mouse_y + 0.25;
	vertexPosition[index++] = 0.0;
	vertexPosition[index++] = mouse_x - 0.25;
	vertexPosition[index++] = mouse_y - 0.25;
	vertexPosition[index++] = 0.0;
	vertexPosition[index++] = mouse_x + 0.25;
	vertexPosition[index++] = mouse_y - 0.25;
	vertexPosition[index++] = 0.0;
	index = index % 36;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertexPosition),
		vertexPosition, GL_STATIC_DRAW
	);
	glutPostRedisplay();
}

GLvoid ChangeWindowSize(int w, int h)
{
	ClientWidth = w;
	ClientHeight = h;
}


