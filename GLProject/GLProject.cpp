//to start -- following https://www.glfw.org/docs/3.3/quick.html
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//sets up GLFW and GLEW , returns GLFW window
GLFWwindow* InitWindow() {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		printf("GLFW init failed!\n");
		exit(1);
	}
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* mainWindow = glfwCreateWindow(800, 450, "Jonny's Window", 0, 0);
	if (!mainWindow) {
		printf("GLFW window creation failed!\n");
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(mainWindow);
	//context MUST be made current BEFORE glew init
	if (glewInit() != GLEW_OK) {
		printf("GLEW init failed!\n");
		exit(1);
	}
	glfwSetKeyCallback(mainWindow, key_callback);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	int frameW, frameH;
	glfwGetFramebufferSize(mainWindow, &frameW, &frameH);
	glViewport(0, 0, frameW, frameH);

	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	return mainWindow;

}

const char* lOGLvertex = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* lOGLfragment = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

unsigned int MakeShaderProgram() {
	unsigned int vertexShader;
	unsigned int fragmentShader;
	int  success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &lOGLvertex, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile vertex shader!\n %s\n", infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &lOGLfragment, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Failed to compile fragment shader!\n %s\n", infoLog);
	}

	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);


	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Failed to link shader program!\n %s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}

int main()
{
	GLFWwindow* mainWindow = InitWindow();
	glfwSwapInterval(1);//vsync ON
	unsigned int vertexShader;
	unsigned int fragmentShader;
	int  success;
	char infoLog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &lOGLvertex, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile vertex shader!\n %s\n", infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &lOGLfragment, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Failed to compile fragment shader!\n %s\n", infoLog);
	}

	unsigned int program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);


	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Failed to link shader program!\n %s\n", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//vertex buffer is a "dumb" buffer, VertexArray contains metadata about the buffer
	//"3d" triangle(all z components set to 0)
	float TRIANGLE[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f//btw setting this to 0,-0.5 instead makes you spend over an hour debugging air
	};

	unsigned int vertexBuffer;
	unsigned int VertexArray;

	glGenVertexArrays(1, &VertexArray);
	glGenBuffers(1, &vertexBuffer);

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TRIANGLE), TRIANGLE, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//for-ever loops are funny, I will die on this hill
	double time = 0;//clock class incoming soonTM
	for (;;) {
		if (glfwWindowShouldClose(mainWindow)) break;
		time = glfwGetTime();
		//input
		//update
		//render
		glClearColor(0.0f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//draws the currently bound buffer with the specified program and buffer 
		glUseProgram(program);
		glBindVertexArray(VertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}


	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
