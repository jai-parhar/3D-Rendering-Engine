#include "Transform.h"

Transform::Transform(glm::vec3 pos, glm::vec3 eulerRot, glm::vec3 sc)
{
	translation = pos;
	setRotationFromEuler(eulerRot);
	scale = sc;
}

glm::mat4 Transform::getRotationMatrix()
{
	return glm::mat4_cast(rotation);
}

glm::mat4 Transform::getTranslationMatrix()
{
	return glm::translate(glm::mat4(1.0f), translation);
}

glm::mat4 Transform::getScaleMatrix()
{
	return glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Transform::getTransformMatrix()
{
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
}

glm::vec3 Transform::getForwardVector()
{
	return glm::vec3(getRotationMatrix()[0][2], getRotationMatrix()[1][2], getRotationMatrix()[2][2]);
}

glm::vec3 Transform::getRightVector()
{
	return glm::vec3(getRotationMatrix()[0][0], getRotationMatrix()[1][0], getRotationMatrix()[2][0]);
}

glm::vec3 Transform::getUpVector()
{
	return glm::cross(getForwardVector(), getRightVector());
}

void Transform::setRotationFromEuler(glm::vec3 euler)
{
	glm::quat x = glm::angleAxis(euler.x, glm::vec3(1, 0, 0));
	glm::quat y = glm::angleAxis(euler.y, glm::vec3(0, 1, 0));
	glm::quat z = glm::angleAxis(euler.z, glm::vec3(0, 0, 1));

	rotation = x*y*z;
	rotation = glm::normalize(rotation);
}

void Transform::rotateFromEuler(glm::vec3 euler)
{
	float pitch = glm::pitch(rotation);
	float yaw = glm::yaw(rotation);
	float roll = glm::roll(rotation);

	pitch += euler.x;
	yaw += euler.y;
	roll += euler.z;

	setRotationFromEuler(glm::vec3(pitch, yaw, roll));
}

Transform::~Transform()
{
}
