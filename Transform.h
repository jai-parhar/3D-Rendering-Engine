#pragma once
#include "Entity.h"
class Transform :
	public Component
{
public:
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 eulerRot = glm::vec3(0), glm::vec3 scale = glm::vec3(1.0f));

	glm::mat4 getRotationMatrix();
	glm::mat4 getTranslationMatrix();
	glm::mat4 getScaleMatrix();
	glm::mat4 getTransformMatrix();

	glm::vec3 getForwardVector();
	glm::vec3 getRightVector();
	glm::vec3 getUpVector();

	void setRotationFromEuler(glm::vec3 euler);
	void rotateFromEuler(glm::vec3 euler);

	~Transform();
};

