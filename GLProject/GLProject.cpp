//TODO make PCH for dependencies and STL
//STL
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//GL
#define GLM_SWIZZLE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//OTHER
//no fucking clue how to PCH a header-only lib but here we go
#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include <tiny_obj_loader.h>
//MINE
#include "mesh_data.cpp"
#include "ShaderProgram.h"
#include "Camera.h"


constexpr float PI = 3.141592653589793238462643383279502884197;

constexpr float CAM_MOVE_SPEED = 8.0f;
constexpr float CAM_ROTATE_SPEED = 75.0f;

static bool reloadShaders = true;
static float object_rotate_speed = 30;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
static Camera MainCamera;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		reloadShaders = true;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		object_rotate_speed = object_rotate_speed == 30 ? 0 : 30;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void MovementInput(GLFWwindow* window,float dT) {
	glm::vec3 targetTranslation = { 0.0f,0.0f,0.0f };

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		targetTranslation.z -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		targetTranslation.z += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		targetTranslation.x -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		targetTranslation.x += 1.0f;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		targetTranslation.y -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		targetTranslation.y += 1.0f;

	if (targetTranslation != glm::vec3(0.0f, 0.0f, 0.0f)) {
		targetTranslation = glm::normalize(targetTranslation);
		targetTranslation *= (CAM_MOVE_SPEED * dT);
		MainCamera.Translate(targetTranslation);
	}



	glm::vec3 targetRotation = { 0.0f,0.0f,0.0f };
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		targetRotation.x += (CAM_ROTATE_SPEED * dT);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		targetRotation.x -= (CAM_ROTATE_SPEED * dT);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		targetRotation.y -= (CAM_ROTATE_SPEED * dT);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		targetRotation.y += (CAM_ROTATE_SPEED * dT);
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
		targetRotation.z -= (CAM_ROTATE_SPEED * dT);
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
		targetRotation.z += (CAM_ROTATE_SPEED * dT);
	MainCamera.Rotate(targetRotation);


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
	glfwSwapInterval(1); //vsync ON
	return mainWindow;

}

unsigned int CreateVAPs2() {
	using namespace teapot;
	//using the namespace allows this function to be invariant to different models 
	std::vector<float> bufVec;
	for (int j = 0; j < points_count; j++) {
		for (int i = 0; i < 4; i++) {
			bufVec.push_back(vertices[points[j]][i]);
		}
		for (int i = 0; i < 4; i++) {
			bufVec.push_back(colours[points[j]][i]);
		}
	}
	unsigned int output;
	unsigned int buffer;

	glGenVertexArrays(1, &output);
	glGenBuffers(1, &buffer);

	//put data in
	glBindVertexArray(output);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, bufVec.size() * sizeof(float), &bufVec[0], GL_DYNAMIC_DRAW);

	//tell OGL the layout of the data

	//position
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//colour
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return output;
}
//TODO find best way to transport VAO size info to draw call site
static size_t BUFSIZTEST=0;
GLuint CreateObjVA(tinyobj::ObjReader& reader) {
	using namespace tinyobj;
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

	//read all attribute data into a single buffer
	std::vector<float> bufVec;
	bool warnedMissingNorm = false;
	bool warnedMissingTex = false;
	for(size_t s = 0;s < shapes.size();s++){
		printf("%u\n", s);
		size_t idx_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			//for each face of the shape
			//TODO make this error if fed non triangular faces
			for (size_t v = 0; v < size_t(shapes[s].mesh.num_face_vertices[f]); v++) {
				//for each vertex of the face
				index_t idx = shapes[s].mesh.indices[idx_offset + v];

				//vertex access
				bufVec.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 0]);
				bufVec.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 1]);
				bufVec.push_back(attrib.vertices[3 * size_t(idx.vertex_index) + 2]);
				//vertex colour access
				bufVec.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 0]);
				bufVec.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 1]);
				bufVec.push_back(attrib.colors[3 * size_t(idx.vertex_index) + 2]);
				//IMPROVEMENT do these checks once at start
				if (idx.normal_index >= 0) {
					//normal access(if they exist)
					bufVec.push_back(attrib.normals[3 * size_t(idx.normal_index) + 0]);
					bufVec.push_back(attrib.normals[3 * size_t(idx.normal_index) + 1]);
					bufVec.push_back(attrib.normals[3 * size_t(idx.normal_index) + 2]);
					
				}
				else {
					if (!warnedMissingNorm) printf("Normal vectors don't exist! Appending zeros!");
					for (int i = 0; i < 3; i++) bufVec.push_back(0.0f);
					warnedMissingNorm = true;
				}
				if (idx.texcoord_index >= 0) {
					//uv access(if they exist)
					bufVec.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 0]);
					bufVec.push_back(attrib.texcoords[2 * size_t(idx.texcoord_index) + 1]);
				}
				else {
					if (!warnedMissingTex) printf("Texture coords don't exist! Appending zeros!");
					for (int i = 0; i < 2; i++) bufVec.push_back(0.0f);
					warnedMissingTex = true;
				}
			}
			idx_offset += size_t(shapes[s].mesh.num_face_vertices[f]);
		}
	}
	BUFSIZTEST = bufVec.size();
	//add data into a GL buffer
	GLuint output;
	GLuint buffer;

	glGenVertexArrays(1, &output);
	glGenBuffers(1, &buffer);
	glBindVertexArray(output);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, bufVec.size() * sizeof(float), &bufVec[0], GL_STATIC_DRAW);

	//TODO find out how people normally un-hardcode buffer construction

	//VCNT
	//3332

	//11total
	
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//UV
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float),(void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	return output;
}
int main()
{
	GLFWwindow* mainWindow = InitWindow();
	


	tinyobj::ObjReaderConfig readerConfig;
	{
		readerConfig.mtl_search_path = "./";
		readerConfig.triangulate = true;
		readerConfig.triangulation_method = "earcut";
		readerConfig.vertex_color = true;
	}
	tinyobj::ObjReader reader;
	if (!reader.ParseFromFile("sphere.obj",readerConfig)) {
		printf("TinyObjReader error: %s\n", reader.Error());
		exit(1);
	}
	if (!reader.Warning().empty()) {
		printf("TinyObjReader warning: %s\n", reader.Warning());
	}
	GLuint VertexArray = CreateObjVA(reader);
	//unsigned int VertexArray = CreateVAPs2();

	glBindVertexArray(VertexArray);
	
	
	const char* vertPath = "ColouredVert.vert";
	const char* fragPath = "ColouredFrag.frag";
	ShaderProgram program(vertPath,fragPath);
	auto vertTime = std::filesystem::last_write_time(vertPath);
	auto fragTime = std::filesystem::last_write_time(fragPath);
	program.Reload();
	double deltaTime = 0;
	double PrevTime = 0;
	double time = 0;//clock class incoming soonTM

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//for-ever loops are funny, I will die on this hill
	for (;;) {
		if (glfwWindowShouldClose(mainWindow)) break;

		auto newVertTime = std::filesystem::last_write_time(vertPath);
		auto newFragTime = std::filesystem::last_write_time(fragPath);
		if (vertTime != newVertTime || fragTime != newFragTime || reloadShaders) { 
			printf("Reloading Shader!\n");
			program.Reload();
			glUseProgram(program);
			vertTime = newVertTime;
			fragTime = newFragTime;
			reloadShaders = false;
		}
		//input
		time = glfwGetTime();
		deltaTime = time - PrevTime;
		MovementInput(mainWindow, deltaTime);
		//update

		//MODEL TO WORLD SPACE
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians((float)time * object_rotate_speed ), glm::vec3(0.0f, 1.0f, 0.0f));


		//WORLD SPACE TO CAMERA SPACE
		glm::mat4 view = glm::mat4(1.0f);
		auto translated = glm::translate(view, MainCamera.Position);
		//view = glm::rotate(translated,(float)MainCamera.rotation.length(), glm::eulerAngles(MainCamera.rotation));
		//view = translated * glm::toMat4(MainCamera.rotation) ;
		//view = translated;
		view = MainCamera.GetViewMatrix();
		//PROJECT ONTO CAMERA PLANE
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 450.0f, 0.1f, 1000.0f);

		int modelLoc = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		int viewLoc = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projectionLoc = glGetUniformLocation(program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		int cameraPosLoc = glGetUniformLocation(program, "cameraPos");
		glUniform3f(cameraPosLoc,MainCamera.Position.x, MainCamera.Position.y, MainCamera.Position.z);

		//render
		glClearColor(0.0f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draws the currently bound buffer with the specified program and buffer 
		glDrawArrays(GL_TRIANGLES, 0, BUFSIZTEST);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
		PrevTime = time;
	}


	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
