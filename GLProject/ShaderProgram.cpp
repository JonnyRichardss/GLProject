#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) : vertPath(vertexPath),fragPath(fragmentPath)
{
	Reload();
}

ShaderProgram::~ShaderProgram()
{
	//delete program if it exists
	if (ID != 0) {
		glDeleteProgram(ID);
	}
}

bool ShaderProgram::IsValid()
{
	return valid;
}

void ShaderProgram::Reload()
{
	//delete program if it exists
	if (ID != 0) {
		glDeleteProgram(ID);
	}
	valid = true;//assume valid until compilation fails
	//load files + compile
	GLuint Vertex = CompileShader(vertPath, GL_VERTEX_SHADER);
	GLuint Fragment = CompileShader(fragPath, GL_FRAGMENT_SHADER);
	//link
	//skip linking if compilation has already failed to potentially preserve a previously valid shader
	if (valid)
		LinkProgram(Vertex, Fragment);
}

ShaderProgram::operator GLuint()
{
	return ID;
}

void ShaderProgram::LinkProgram(GLuint vertexShader,GLuint fragmentShader)
{
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		valid = false;
		char infoLog[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("Failed to link shader program!\n %s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


GLuint ShaderProgram::CompileShader(const char* filePath, GLenum shaderType)
{
	//load
	std::ifstream f = std::ifstream(filePath);
	const char* sourceText;
	std::string* str = new std::string();
	if (f.is_open()) {
		std::stringstream ss;
		ss << f.rdbuf();
		*str = ss.str();
		sourceText = str->c_str();
	}
	else {
		valid = false;
		printf("Shader file at %s not opened!\n", filePath);
		sourceText = "!";
	}
	//compile
	GLuint shader = glCreateShader(shaderType);
	int success;
	glShaderSource(shader, 1, &sourceText, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		valid = false;
		//potentially not necessary in here anymore
		char infoLog[511];
		glGetShaderInfoLog(shader, 511, NULL, infoLog);
		printf("Failed to complile shader at %s!\n%s\n",filePath, infoLog);
		return 0;
	}
	return shader;
}
