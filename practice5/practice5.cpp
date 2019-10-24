#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

#define PI 3.141592
#define CIRCLENUM 10


// �������� ����
GLuint ShaderProgramID;
GLuint ClientWidth = 800, ClientHeight = 800;
GLenum drawMode = GL_LINE_LOOP;

GLuint VAO[10], VBO_position[10], VBO_color[10];		// 10���� VAO, VBO6
bool activeCircle[10] = { false, };		// 10���� �Ҹ��� ������ �ٲٰ�, Ȱ��ȭ �� �ָ� ũ�� �ٲٰ� �׸���
int circleNum = 0;

// VertexArray�� 10�� �ʿ���
// ���� �׸��µ� �ʿ��� ���� ���� 40��
// ������ ���� 40 * 2 = 80��
float vertexPosition[10][80];
float vertexColor[10][120];
float radius[10];
float sinArray[40];
float cosArray[40];
float centerX[10], centerY[10];


void initSinCosValue() {
	float angle = 0;	// ����
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
		vertexPosition[index][i * 2] = centerX[index] + radius[index] * cosArray[i];		// x��
		vertexPosition[index][i * 2 + 1] = centerY[index] + radius[index] * sinArray[i];	// y��
	}
	if (index % 2 == 0) {
		//���߿� �ϳ��� �����
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

	fseek(fptr, 0, SEEK_END);	//������ ������ ã�´�
	length = ftell(fptr);		// ���� �������� ���� ��ġ ������
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, sizeof(char), length, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}
GLuint InitShader() {
	using namespace std;

	// Vertex Shader �ҽ� ���� �б�
	const GLchar* vertexShaderSource =
		fileToBuf("VertexShader_5.txt");
	if (vertexShaderSource == nullptr) {
		cout << "VertexShaderSource �б� ����" << endl;
	}

	// Vertex Shader ��ü �����ϰ� ������ �ϱ�
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader �����Ͻ���\n" << errorLog << endl;
		return false;
	}

	// Fragment Shader �ҽ� ���� �б�
	const GLchar* fragmentShaderSource =
		fileToBuf("FragmentShader_5.txt");
	if (fragmentShaderSource == nullptr) {
		cout << "FragmentShaderSource �б� ����" << endl;
	}

	//Fragment Shader ��ü �����ϰ� ������ �ϱ�
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader �����Ͻ���\n" << errorLog << endl;
		return false;
	}
	ShaderProgramID = glCreateProgram();


	// ��ũ�ϱ�
	glAttachShader(ShaderProgramID, vertexShader);
	glAttachShader(ShaderProgramID, fragmentShader);
	glLinkProgram(ShaderProgramID);
	glDeleteShader(vertexShader);
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program �������\n" << errorLog << endl;
		return false;
	}
	glUseProgram(ShaderProgramID);
	return ShaderProgramID;
}
GLvoid CreateVAO() {
	for (int i = 0; i < CIRCLENUM; i++) {
		glGenVertexArrays(1, &VAO[i]);	// VAO ��ü ����
	}

	//glBindVertexArray(VAO);		// ������ VAO �Ѱ��ϱ� ���ε嵵 ����
}
GLvoid CreateVBO(const int index) {

	// glBindBuffer: ���� ������Ʈ �̸��� ���ε��Ѵ�.
	// glBufferData: ���� ������Ʈ�� �����͸� ����
	
	// ���� ���� �Ӽ��� 3���� ���̴� ���α׷� �ȿ��� ����ϴ°� �غ���

	// ��ġ
	glGenBuffers(1, &VBO_position[index]);					// ���� �����
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position[index]);		// ���ε� ���ֱ�
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertexPosition[index]), vertexPosition[index], GL_DYNAMIC_DRAW);
	// �����Ͱ� �׷��� ������ ���� ����Ǵ� ���

	GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos"); 
	glVertexAttribPointer(pAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);	// ���ؽ� �Ӽ� �����Ϳ� �迭�� ����
	glEnableVertexAttribArray(pAttribute);	// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�

	// ������
	glGenBuffers(1, &VBO_color[index]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color[index]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor[index]), vertexColor[index], GL_DYNAMIC_DRAW);
	GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
	glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);	// ���ؽ� �Ӽ� �����Ϳ� �迭�� ����
	glEnableVertexAttribArray(cAttribute);	// ���ؽ� �Ӽ� �迭�� ����ϵ��� �Ѵ�

	// glGetAttribLocation(): �Ӽ� ������ ��ġ�� �����´�
	// ���� ���ε��Ǿ��ִ� VBO�� shader program�̶� �����ϴ� �κ�
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
	// ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(ClientWidth, ClientHeight);
	glutCreateWindow("Example1");

	// GLEW �ʱ�ȭ�ϱ�
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

	// ������ Ư�� �κ� ä��⵵ ����
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPosition), &vertexPosition);;
	// �̰� ���� ��ü ä���
	// ������ �ٸ� ���۸� ����ϰ� �Ǹ� ���ε� �ϴ� ���۸� ��� �ٲ������
	// ��� ���� ���� ��ġ ���۴�

	for (int i = 0; i < CIRCLENUM; i++) {
		if (activeCircle[i] == false)
			continue;
		// VAO ���ε� ��
		glBindVertexArray(VAO[i]);


		glBindBuffer(GL_ARRAY_BUFFER, VBO_position[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition[i]), vertexPosition[i], GL_DYNAMIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_color[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor[i]), vertexColor[i], GL_DYNAMIC_DRAW);

		// �迭�κ��� ������Ƽ�� �������ϱ�
		glDrawArrays(drawMode, 0, 40);	// �� 40��
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
		// ������ �׸���
		drawMode = GL_POINTS;
		break;
	case '2':
		// ������ �׸���
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
		updatePosition(i);				// ���� �׷����ϴ� ���� �� ��ǥ ��� ������Ʈ
	}

	glutPostRedisplay();		// �ٽ� �׸���
	glutTimerFunc(0, Timer, 0);
}

GLvoid Mouse(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
	// Ȱ��ȭ ���� ���� ���� ã��
	circleNum = -1;
	for (int i = 0; i < CIRCLENUM; i++) {
		if (activeCircle[i] == false) {
			circleNum = i;
			break;
		}
	}
	if (circleNum  < 0) return;		// �� ���� ���� �� �� ��
	
	// ������ ���콺 ��ǥ -> GL ����Ʈ ��ǥ
	float mouseX = ((float)x / ClientWidth * 2) - 1;
	float mouseY = -(((float)y / ClientHeight * 2) - 1);
	
	glBindVertexArray(VAO[circleNum]);			// VAO�� ���̵� �� ������
	initPosition(circleNum, mouseX, mouseY);	// ���콺 ��ǥ�� ���ۿ� ������
	CreateVBO(circleNum);						// �� ���۰��� ������ VBO�� ������
	activeCircle[circleNum] = true;

	circleNum += 1;
}

GLvoid ChangeWindowSize(int w, int h)
{
}