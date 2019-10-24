#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#define TRIANGLENUM 10
#define RADIUS 0.1			// 0.0025가 1픽셀
#define M_PI 3.141592
#define SPEED 0.000002


// 화면 중앙에 사각형을 그린다.
// 밖에서 튕기던 삼각형은 내부 사각형을 만나도 방향을 바꾼다.
// 내부 사각형 안에는 2개의 삼각형이 s튕기고 있다.
// 2개의 삼각형은 각 꼭짓점마다 다른 색상이 설정되어 있다.
// 삼각형이 벽에 닿으면 색상이 변경된다

GLuint ShaderProgramID;
GLuint ClientWidth = 800, ClientHeight = 800;

GLuint VAO, VBO_position, VBO_color;


float getMin(float* firstindex) {

	float min = firstindex[0];

	for (int i = 1; i < 3; i++) {
		if (min > firstindex[i * 2])
			min = firstindex[i * 2];
	}
	return min;
}
float getMax(float* firstindex) {

	float max = firstindex[0];

	for (int i = 1; i < 3; i++) {
		if (max < firstindex[i * 2])
			max = firstindex[i * 2];
	}
	return max;
}


int dir[TRIANGLENUM];	//위 아래 왼쪽 오른쪽
float _centerx[TRIANGLENUM];
float _centery[TRIANGLENUM];
float DiretionVector[TRIANGLENUM * 2];

// 맨 앞에 두개의 삼각형이 내부 삼각형
float vertexPosition[60];
float vertexColor[90];

void initVertexAttrib();
void initDirVector();
void updateVertexPos();
void rotationTriangle(float* firstindex, int i);
void crushCheck();

void initVertexAttrib() {

	for (int i = 0; i < TRIANGLENUM; i++) {

		float centerX;
		float centerY;

		if (i == 0 || i == 1) {
			centerX = 0;
			centerY = 0;
		}
		else {
			//centerX = ((float)rand() / RAND_MAX) - 0.5f;
			//centerY = ((float)rand() / RAND_MAX) - 0.5f;
			centerX = -0.7;
			centerY = -0.7;
		}

		float colorR = ((float)rand() / RAND_MAX);
		float colorG = ((float)rand() / RAND_MAX);
		float colorB = ((float)rand() / RAND_MAX);


		vertexColor[i * 9] = colorR;
		vertexColor[i * 9 + 1] = colorG;
		vertexColor[i * 9 + 2] = colorB;

		if (i == 0 || i == 1) {
			colorR = ((float)rand() / RAND_MAX);
			colorB = ((float)rand() / RAND_MAX);
			colorG = ((float)rand() / RAND_MAX);
		}	

		vertexColor[i * 9 + 3] = colorR;
		vertexColor[i * 9 + 4] = colorG;
		vertexColor[i * 9 + 5] = colorB;

		if (i == 0 || i == 1) {
			colorR = ((float)rand() / RAND_MAX);
			colorB = ((float)rand() / RAND_MAX);
			colorG = ((float)rand() / RAND_MAX);
		}

		vertexColor[i * 9 + 6] = colorR;
		vertexColor[i * 9 + 7] = colorG;
		vertexColor[i * 9 + 8] = colorB;

		_centerx[i] = centerX;
		_centery[i] = centerY;

		dir[i] = rand() % 4;

		updateVertexPos();
	}

}
void initDirVector() {

	for (int i = 0; i < TRIANGLENUM; i++) {
		float angle = (M_PI * 2) / 360.0f * (rand() % 360);

		DiretionVector[i * 2] = cosf(angle) * SPEED * (rand() % 5 + 1);
		DiretionVector[i * 2 + 1]	= sinf(angle)  * SPEED * (rand() % 5 + 1);
	}
}

void updateVertexPos() {
	for (int i = 0; i < TRIANGLENUM; i++) {
		_centerx[i] += DiretionVector[i * 2];
		_centery[i] += DiretionVector[i * 2 + 1];

		if (dir[i] == 0) {
			// 위
			vertexPosition[i * 6] = _centerx[i];
			vertexPosition[i * 6 + 1] = _centery[i] + 0.1f;

			vertexPosition[i * 6 + 2] = _centerx[i] - 0.1f;
			vertexPosition[i * 6 + 3] = _centery[i] - 0.1f;

			vertexPosition[i * 6 + 4] = _centerx[i] + 0.1f;
			vertexPosition[i * 6 + 5] = _centery[i] - 0.1f;
		}
		else if (dir[i] == 1) {
			// 아래
			vertexPosition[i * 6] = _centerx[i] - 0.1f;
			vertexPosition[i * 6 + 1] = _centery[i] + 0.1f;

			vertexPosition[i * 6 + 2] = _centerx[i];
			vertexPosition[i * 6 + 3] = _centery[i] - 0.1f;

			vertexPosition[i * 6 + 4] = _centerx[i] + 0.1f;
			vertexPosition[i * 6 + 5] = _centery[i] + 0.1f;

		}
		else if (dir[i] == 2) {
			// 왼쪽
			vertexPosition[i * 6] = _centerx[i] - 0.1f;
			vertexPosition[i * 6 + 1] = _centery[i];

			vertexPosition[i * 6 + 2] = _centerx[i] + 0.1f;
			vertexPosition[i * 6 + 3] = _centery[i] - 0.1f;

			vertexPosition[i * 6 + 4] = _centerx[i] + 0.1f;
			vertexPosition[i * 6 + 5] = _centery[i] + 0.1f;
		}
		else {
			// 오른쪽
			vertexPosition[i * 6] = _centerx[i] - 0.1f;
			vertexPosition[i * 6 + 1] = _centery[i] + 0.1f;

			vertexPosition[i * 6 + 2] = _centerx[i] - 0.1f;
			vertexPosition[i * 6 + 3] = _centery[i] - 0.1f;

			vertexPosition[i * 6 + 4] = _centerx[i] + 0.1f;
			vertexPosition[i * 6 + 5] = _centery[i];
		}

		//vertexPosition[i * 6] += DiretionVector[i * 2];
		//vertexPosition[i * 6 + 1] += DiretionVector[i * 2 + 1];
		//
		//vertexPosition[i * 6 + 2] += DiretionVector[i * 2];
		//vertexPosition[i * 6 + 3] += DiretionVector[i * 2 + 1];
		//
		//vertexPosition[i * 6 + 4] += DiretionVector[i * 2];
		//vertexPosition[i * 6 + 5] += DiretionVector[i * 2 + 1];

	}
}

void rotationTriangle(float* firstindex, int i) {
	int x, y;
	x = firstindex[0] - _centerx[i];
	y = firstindex[1] - _centery[i];

	firstindex[0] = x * cosf(M_PI / 2.0f) - y * sinf(M_PI / 2.0f) + _centerx[i];
	firstindex[1] = x * sinf(M_PI / 2.0f) + y * cosf(M_PI / 2.0f) + _centery[i];

	x = firstindex[2] - _centerx[i];
	y = firstindex[3] - _centery[i];

	firstindex[2] = x * cosf(M_PI / 2.0f) - y * sinf(M_PI / 2.0f) + _centerx[i];
	firstindex[3] = x * sinf(M_PI / 2.0f) + y * cosf(M_PI / 2.0f) + _centery[i];

	x = firstindex[4] - _centerx[i];
	y = firstindex[5] - _centery[i];

	firstindex[4] = x * cosf(M_PI / 2.0f) - y * sinf(M_PI / 2.0f) + _centerx[i];
	firstindex[5] = x * sinf(M_PI / 2.0f) + y * cosf(M_PI / 2.0f) + _centery[i];

}
void crushCheck() {
	// x 좌표 -1에서 1
	// y좌표 -1에서 1
	for (int i = 0; i < TRIANGLENUM; i++) {

		bool crush = false;
		
		float min_x = getMin(&vertexPosition[i * 6]);
		float max_x = getMax(&vertexPosition[i * 6]);

		float min_y = getMin(&vertexPosition[i * 6 + 1]);
		float max_y = getMax(&vertexPosition[i * 6 + 1]);

		float boundary;
		
		if (i == 0 || i == 1) {
			boundary = 0.5f;
		}
		else boundary = 1.0f;


		// 삼각형 돌리고 방향벡터 바꾸는 부분
		// 내부 외부 공통
		if (min_x < -boundary) {
			DiretionVector[i * 2] = -DiretionVector[i * 2];
			dir[i] = (dir[i] + 1) % 4;
			crush = true;
		}
		if (max_x > boundary) {
			DiretionVector[i * 2] = -DiretionVector[i * 2];
			dir[i] = (dir[i] + 1) % 4;
			crush = true;

		}
		if (min_y < -boundary) {
			DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
			dir[i] = (dir[i] + 1) % 4;
			crush = true;

		}
		if (max_y > boundary) {
			DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
			dir[i] = (dir[i] + 1) % 4;
			crush = true;
		}

		if (i != 0 && i != 1) {
			// 삼각형과 내부 사각형 충돌체크...
			if (_centerx[i] < -0.5) {
				if (min_y < 0.5 && max_y > -0.5 && max_x > -0.5) {
					DiretionVector[i * 2] = -DiretionVector[i * 2];
					dir[i] = (dir[i] + 1) % 4;
					crush = true;
				}
			}
			if (_centerx[i] > 0.5) {
				if (min_y < 0.5 && max_y > -0.5 && min_x < 0.5) {
					DiretionVector[i * 2] = -DiretionVector[i * 2];
					dir[i] = (dir[i] + 1) % 4;
					crush = true;
				}
			}
			if (_centery[i] < -0.5) {
				if (min_x < 0.5 && max_x > -0.5 && max_y > -0.5) {
					DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
					dir[i] = (dir[i] + 1) % 4;
					crush = true;
				}
			}
			if (_centery[i] > 0.5) {
				if (min_x < 0.5 && max_x > -0.5 && min_y < 0.5) {
					DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
					dir[i] = (dir[i] + 1) % 4;
					crush = true;
				}
			}
			//if (max_x > -0.5) {
			//	DiretionVector[i * 2] = -DiretionVector[i * 2];
			//	dir[i] = (dir[i] + 1) % 4;
			//	crush = true;
			//
			//}
			//if (min_y < 0.5) {
			//	DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
			//	dir[i] = (dir[i] + 1) % 4;
			//	crush = true;
			//
			//}
			//if (max_y > -0.5) {
			//	DiretionVector[i * 2 + 1] = -DiretionVector[i * 2 + 1];
			//	dir[i] = (dir[i] + 1) % 4;
			//	crush = true;
			//}
		}

		// 부딪혔다면 색 바꾸기
		if (crush) {
			if (i == 0 || i == 1) {

				vertexColor[i * 9] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 1] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 2] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 3] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 4] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 5] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 6] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 7] = ((float)rand() / RAND_MAX);
				vertexColor[i * 9 + 8] = ((float)rand() / RAND_MAX);

			}
			else {
				float colorR = ((float)rand() / RAND_MAX);
				float colorG = ((float)rand() / RAND_MAX);
				float colorB = ((float)rand() / RAND_MAX);


				vertexColor[i * 9] = colorR;
				vertexColor[i * 9 + 1] = colorG;
				vertexColor[i * 9 + 2] = colorB;

				vertexColor[i * 9 + 3] = colorR;
				vertexColor[i * 9 + 4] = colorG;
				vertexColor[i * 9 + 5] = colorB;

				vertexColor[i * 9 + 6] = colorR;
				vertexColor[i * 9 + 7] = colorG;
				vertexColor[i * 9 + 8] = colorB;

			}
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
		fileToBuf("VertexShader_7.txt");
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
		fileToBuf("FragmentShader_7.txt");
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


GLvoid InitBuffer() {
	// VAO 생성
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// 위치 속성
	glGenBuffers(1, &VBO_position);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition),vertexPosition, GL_STATIC_DRAW);

	GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
	glVertexAttribPointer(pAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(pAttribute);

	glGenBuffers(1, &VBO_color);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);

	GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
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
	srand(time(NULL));

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

	// 사용할 데이터 초기화 부분
	initVertexAttrib();
	initDirVector();
	glPointSize(3);
	InitShader();
	InitBuffer();



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

	// 그리기
	glUseProgram(ShaderProgramID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);

	glBegin(GL_LINE_STRIP);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glVertex2f(-0.5f, -0.5f);
	glEnd();
	glFinish();

	glDrawArrays(
		GL_TRIANGLES,							// 렌더링할 도형 종류
		0,												// 배열에서 도형 시작 인덱스
		TRIANGLENUM * 3						// 버텍스 개수
	);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
}

GLvoid Timer(int value)
{
	crushCheck();
	updateVertexPos();
	glutPostRedisplay();
	glutTimerFunc(0, Timer, 0);

}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON) return;
	if (state != GLUT_DOWN) return;

	float mouse_x = ((float)x / ClientWidth * 2) - 1;
	float mouse_y = -(((float)y / ClientHeight * 2) - 1);



	glutPostRedisplay();
}

GLvoid ChangeWindowSize(int w, int h)
{

}


