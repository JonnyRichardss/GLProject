#pragma once
typedef unsigned int GLenum;
typedef unsigned int GLuint;
//TODO replace const char* with std::string& to make ownership semantics more obvious
class ShaderProgram
{
public:
	ShaderProgram(const char* vertexPath, const char* fragmentPath);
	~ShaderProgram();
	bool IsValid();
	void Reload();
	operator GLuint();
private:
	unsigned int ID;
	const char* vertPath;
	const char* fragPath;
	bool valid;
	void LinkProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint CompileShader(const char* sourceText,GLenum shaderType);
};

