#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
static constexpr glm::vec3 WorldUp = {0.0f,1.0f,0.0f};
struct Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;

	//assuming no roll for now?
	glm::vec3 EulerAngles;

	//default constructor
	Camera() {
		Position = { 0.0f,0.0f,10.0f };
		EulerAngles = { 0.0f,-90.0f,0.0f };

		Forward = Right = Up = { 0.0f,0.0f,0.0f };//about to be overwritten rn
		updateVectors();
	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Forward, Up);
	}
	void Translate(glm::vec3 offset) {
		Position += Forward * offset.z;
		Position += Right * offset.x;
		Position += Up * offset.y;
		
	}
	void Rotate(glm::vec3 eulerOffset) {
		EulerAngles += eulerOffset;
		ConstrainPitch();
		updateVectors();
	}
	
private:
	//call this every time eulerAngles is changed
	void updateVectors() {
		glm::vec3 newForward = {
			cos(glm::radians(EulerAngles.y)) * cos(glm::radians(EulerAngles.x)),
			sin(glm::radians(EulerAngles.x)),
			sin(glm::radians(EulerAngles.y)) * cos(glm::radians(EulerAngles.x))
		};
		Forward = glm::normalize(newForward);
		Right = glm::normalize(glm::cross(Forward, WorldUp));
		Up = glm::normalize(glm::cross(Right, Forward));
	}
	void ConstrainPitch() {
		if (EulerAngles.x > 89.0f) EulerAngles.x = 89.0f;
		if (EulerAngles.x < -89.0f) EulerAngles.x = -89.0f;
	}
};


