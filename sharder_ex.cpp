#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <iostream>

GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);
GLvoid Mouse(int button, int state, int x, int y);

GLclampf r = 1.0f, g = 1.0f, b = 1.0f, a = 0.0f;
GLuint ShaderProgramID;
using namespace std;

GLuint complie_shaders() {
	const GLchar* vertexShaderSource =
		"#version 330 core\n" 
		"void main()"
		"{" 
		"const vec4 vertex[3] = vec4[3]    (vec4(0.25, -0.25, 0.5, 1.0), "
		"vec4(-0.25, -0.25, 0.5, 1.0),"
		"vec4(0.25, 0.25, 0.5, 1.0));"
		"gl_Position = vertex [gl_VertexID];"
		"}";
	//---버텍스세이더읽어저장하고컴파일하기 
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

	const GLchar* fragmentShaderSource = 
		"#version 330 core\n"
		"out vec4 fragmentColor;\n"
		"void main()\n" 
		"{\n" 
		"fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);//Red color\n" 
		"}\n";
	//---프래그먼트세이더읽어저장하고컴파일하기 
	GLuint fragmentShader= glCreateShader (GL_FRAGMENT_SHADER);
	glShaderSource (fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader (fragmentShader);
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
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &result);

	if (!result) { 
		//glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog); 
		cerr << "ERROR: shader program 연결실패\n" << errorLog << endl; 
		return false; 
	}
	glUseProgram(ShaderProgramID);
	return ShaderProgramID;
}

void main(int argc, char** argv)
{
	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
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

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutTimerFunc(0, Timer, 0);
	complie_shaders();
	glutMainLoop();
}


GLvoid drawScene()
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);

	// 그리기
	glUseProgram(ShaderProgramID);
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
}

GLvoid Mouse(int button, int state, int x, int y)
{
}


