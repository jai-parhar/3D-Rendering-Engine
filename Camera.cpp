#include "Camera.h"

void Camera::init(GLFWwindow* w)
{
	camSetPos(glm::vec3(0, 0, 10));

	window = w;
	glfwGetWindowSize(window, &screenW, &screenH);

	screenRatio = screenW / screenH;

	xRot = 0.0f;
	yRot = 90.0f;
	zRot = 0.0f;

	calculateMatrices();
}

void Camera::calculateMatrices() 
{
	ProjectionMatrix = glm::perspective(FoV, screenRatio, renderDistMin, renderDistMax);

	glm::mat4 matRoll = glm::rotate(glm::mat4(1.0f), glm::radians(zRot), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 matPitch = glm::rotate(glm::mat4(1.0f), glm::radians(xRot), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 matYaw = glm::rotate(glm::mat4(1.0f), glm::radians(yRot), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotate = matRoll * matPitch * matYaw;

	viewDirection = glm::vec3(rotate[0][2], rotate[1][2], rotate[2][2]);
	right = glm::vec3(rotate[0][0], rotate[1][0], rotate[2][0]);
	up = glm::cross(viewDirection, right);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -position);

	ViewMatrix = rotate * translate;
}

glm::mat4 Camera::getViewMatrix()
{
	return ViewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return ProjectionMatrix;
}

void Camera::camSetPos(glm::vec3 pos) 
{
	position = pos;
}

void Camera::camMove(glm::vec3 vector) 
{
	position += vector;
}

void Camera::camRotate(glm::vec3 vector)
{
	xRot += vector.x;
	yRot += vector.y;
	zRot += vector.z;
	calculateMatrices();
}

glm::vec3 Camera::getViewDirVector()
{
	return viewDirection;
}

glm::vec3 Camera::getRightVector()
{
	return right;
}

GLFWwindow* Camera::getWindow()
{
	return window;
}

void Camera::close()
{
}