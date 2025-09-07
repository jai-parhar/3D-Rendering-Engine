#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include <stdio.h>
#include <iostream>
#include <map>

class Component {};

class Entity
{
public:
	std::map<char*, Component> components;

	void addComponent(char* name, Component c);
	Component getComponent(char* name);
};
