#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 viewDirection;
	glm::vec3 right;
	glm::vec3 up;

	float xRot;
	float yRot;
	float zRot;

	float FoV = 75.0f;
	float screenRatio;
	float renderDistMin = 0.1f;
	float renderDistMax = 100.0f;

	int screenW, screenH;

	GLFWwindow* window;

	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

	void init(GLFWwindow* window);
	void calculateMatrices();
	void Camera::camSetPos(glm::vec3 pos);
	void Camera::camMove(glm::vec3 vector);
	void Camera::camRotate(glm::vec3 vector);

	glm::mat4 Camera::getProjectionMatrix();
	glm::mat4 Camera::getViewMatrix();

	glm::vec3 getViewDirVector();
	glm::vec3 getRightVector();

	GLFWwindow* getWindow();
	
	void close();
};

