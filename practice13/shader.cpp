#include "shader.h"

char* get_filedata_to_buf(const char* filename) {
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

bool init_shader_program(GLuint* id, const char* filename_vert, const char* filename_frag) {
	using namespace std;

	GLint result;
	GLchar errorLog[512];

	const GLchar* vertexShaderSource = get_filedata_to_buf(filename_vert);
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

	const GLchar* fragmentShaderSource = get_filedata_to_buf(filename_frag);
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


	*id = glCreateProgram();

	glAttachShader(*id, vertexShader);
	glAttachShader(*id, fragmentShader);
	glLinkProgram(*id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(*id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(*id, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결실패\n" << errorLog << endl;
		return false;
	}

	return true;

}