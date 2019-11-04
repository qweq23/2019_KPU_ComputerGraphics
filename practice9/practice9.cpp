#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#define M_PI 3.141592

// ��������-----------------------------------------------
GLuint ShaderProgramID;
GLuint ClientWidth = 800, ClientHeight = 800;

GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;

GLuint VAO_cube, VBO_position_cube, VBO_color_cube, EBO_cube;
GLuint VAO_pyramid, VBO_position_pyramid, VBO_color_pyramid, EBO_pyramid;

GLenum drawMode = GL_TRIANGLES;
glm::mat4 rotMatrix = glm::mat4(1.0f);	// �����࿭�� �ʱ�ȭ

bool hideface = true;
bool drawcube = true;

// x������ 30�� y������ -30�� ȸ�����ִ�.
//glm::mat4 rotMatrix = 


//float cubeVertexBuffer[] = {
//	0.5f, 0.5f, 0.5f,			//0
//	0.5f, 0.5f, -0.5f,			//1
//	-0.5f, 0.5f, -0.5f,			//2
//	-0.5f, 0.5f, 0.5f,			//3
//	0.5f, -0.5f, 0.5f,			//4
//	0.5f, -0.5f, -0.5f,			//5
//	-0.5f, -0.5f, -0.5f,		//6
//	-0.5f, -0.5f, 0.5f,			//7
//};

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
	0.0f, 0.0f, 0.0f,		// 0 ������
	1.0f, 1.0f, 0.0f,		// 1 ���
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�

	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	0.0f, 1.0f, 1.0f,		// 3 û��
	0.0f, 0.0f, 0.0f,		// 0 ������

	0.0f, 1.0f, 1.0f,		// 3 û��
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	0.0f, 0.0f, 1.0f,		// 7 �Ķ�

	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	1.0f, 1.0f, 1.0f,		// 6 ��
	0.0f, 0.0f, 1.0f,		// 7 �Ķ�

	1.0f, 1.0f, 1.0f,		// 6 ��
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	1.0f, 1.0f, 0.0f,		// 1 ���

	1.0f, 1.0f, 1.0f,		// 6 ��
	1.0f, 1.0f, 0.0f,		// 1 ���
	1.0f, 0.0f, 0.0f,		// 5 ����

	1.0f, 0.0f, 0.0f,		// 5 ����
	1.0f, 1.0f, 0.0f,		// 1 ���
	0.0f, 0.0f, 0.0f,		// 0 ������

	1.0f, 0.0f, 1.0f,		// 4 ����
	1.0f, 0.0f, 0.0f,		// 5 ����
	0.0f, 0.0f, 0.0f,		// 0 ������
	
	1.0f, 0.0f, 1.0f,		// 4 ����
	0.0f, 0.0f, 0.0f,		// 0 ������
	0.0f, 0.0f, 1.0f,		// 7 �Ķ�

	0.0f, 0.0f, 1.0f,		// 7 �Ķ�
	0.0f, 0.0f, 0.0f,		// 0 ������
	0.0f, 1.0f, 1.0f,		// 3 û��

	0.0f, 0.0f, 1.0f,		// 7 �Ķ�
	1.0f, 0.0f, 0.0f,		// 5 ����
	1.0f, 0.0f, 1.0f,		// 4 ����

	//0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	//0.0f, 1.0f, 1.0f,		// 3 û��
	//0.0f, 0.0f, 0.0f,		// 0 ������

	1.0f, 0.0f, 0.0f,		// 5 ����
	0.0f, 0.0f, 1.0f,		// 7 �Ķ�
	1.0f, 1.0f, 1.0f,		// 6 ��
};

//float cubeVertexColorBuffer[] = {
//	0.0f, 0.0f, 0.0f,		// 0 ������
//	1.0f, 1.0f, 0.0f,		// 1 ���
//	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
//	0.0f, 1.0f, 1.0f,		// 3 û��
//	1.0f, 0.0f, 1.0f,		// 4 ����
//	1.0f, 0.0f, 0.0f,		// 5 ����
//	1.0f, 1.0f, 1.0f,		// 6 ��
//	0.0f, 0.0f, 1.0f,		// 7 �Ķ�
//
//};
//float pyramidVertexPositionBuffer[15] = {
//	0, 0.5, 0,				// 0
//	0.5, -0.5, 0.5,			// 1
//	0.5, -0.5, -0.5,		// 2
//	-0.5, -0.5, -0.5,		// 3
//	-0.5, -0.5, 0.5			// 4
//};
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

	- 0.5, -0.5, -0.5,		// 3
	0.5, -0.5, 0.5,			// 1
	-0.5, -0.5, 0.5			// 4
};


float pyramidVertexColorBuffer[] = {
	1.0f, 0.0f, 0.0f,		// 0 ��
	0.0f, 0.0f, 1.0f,		// 1 ��
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�

	1.0f, 0.0f, 0.0f,		// 0 ��
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�
	0.0f, 1.0f, 1.0f,		// 3 û��

	1.0f, 0.0f, 0.0f,		// 0 ��
	0.0f, 1.0f, 1.0f,		// 3 û��
	1.0f, 0.0f, 1.0f,		// 4 ����

	1.0f, 0.0f, 0.0f,		// 0 ��
	1.0f, 0.0f, 1.0f,		// 4 ����
	0.0f, 0.0f, 1.0f,		// 1 ��

	0.0f, 0.0f, 1.0f,		// 1 ��
	0.0f, 1.0f, 1.0f,		// 3 û��
	0.0f, 1.0f, 0.0f,		// 2 �ʷ�

	0.0f, 1.0f, 1.0f,		// 3 û��
	0.0f, 0.0f, 1.0f,		// 1 ��
	1.0f, 0.0f, 1.0f,		// 4 ����
};


//----------------------------------------------------------
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
		fileToBuf("VertexShader_9.txt");
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
		fileToBuf("FragmentShader_9.txt");
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
GLvoid InitBuffer() {
	// ���� ����
	{
		glGenVertexArrays(1, &VAO_cube);
		glGenBuffers(1, &VBO_position_cube);
		glGenBuffers(1, &VBO_color_cube);

		glBindVertexArray(VAO_cube);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPostionBuffer), cubeVertexPostionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

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

		glBindVertexArray(VAO_pyramid);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositionBuffer), pyramidVertexPositionBuffer, GL_STATIC_DRAW);
		GLint pAttribute = glGetAttribLocation(ShaderProgramID, "vPos");
		glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pAttribute);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_color_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexColorBuffer), pyramidVertexColorBuffer, GL_STATIC_DRAW);
		GLint cAttribute = glGetAttribLocation(ShaderProgramID, "vColor");
		glVertexAttribPointer(cAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(cAttribute);


	}
	rotMatrix = glm::rotate(rotMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	rotMatrix = glm::rotate(rotMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));
}
//----------------------------------------------------------


// �Լ� ����-------------------------------------------------

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid ChangeWindowSize(int w, int h);

void drawGrid();



void main(int argc, char** argv)
{
	srand(time(NULL));

	// ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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

	// �ʿ��� �ʱ� ����
	glUseProgram(ShaderProgramID);


	if (hideface) {
		// ���� �˻�
		glEnable(GL_DEPTH_TEST);

		// �ø�
		glFrontFace(GL_CCW);		// �ݽð������ �ո�
		glEnable(GL_CULL_FACE);
	}
	//glPointSize(10);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrid();


	// �׸���
	if (drawcube) {
		glBindVertexArray(VAO_cube);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_cube);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPostionBuffer), cubeVertexPostionBuffer, GL_STATIC_DRAW);

		unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));

		glDrawArrays(drawMode, 0, 36);		// ������ ����: ���� ����

	}
	else {
		glBindVertexArray(VAO_pyramid);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_position_pyramid);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertexPositionBuffer), pyramidVertexPositionBuffer, GL_STATIC_DRAW);

		unsigned int transformLocation = glGetUniformLocation(ShaderProgramID, "rotate");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(rotMatrix));

		glDrawArrays(drawMode, 0, 18);		// ������ ����: ���� ����
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

	// ����ü �׸���
	case 'C':
	case 'c':
	{
		drawcube = true;
	}
		break;

	// �簢 �� �׸���
	case 'P':
	case 'p':
	{
		drawcube = false;
	}
		break;

	// �������� ����/����
	case 'H':
	case 'h':
	{
		if (hideface) {
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			hideface = false;
		}
		else {
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			hideface = true;
		}
	}
		break;

	// y���� �������� ��/�� �������� ȸ��
	case 'Y':
	case 'y':
	{
		rotMatrix = glm::rotate(rotMatrix, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
		break;

	// ���̾� ��ü/�ָ��� ��ü
	case 'W':
	case 'w':
	{
		if (drawMode == GL_TRIANGLES)
			drawMode = GL_LINE_LOOP;
		else if (drawMode == GL_LINE_LOOP)
			drawMode = GL_TRIANGLES;
	}
		break;

	default:
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
