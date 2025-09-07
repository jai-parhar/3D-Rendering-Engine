#pragma once
#include <stdio.h>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <map>

#include "Utils.h"

class Shader
{
public:

	GLuint shaderID;
	std::map<char*, GLuint> uniformLocations;

	void init(const char * vertex_file_path, const char * fragment_file_path);
	
	void useShader();

	void addUniform(char* name);
	GLuint getUniformLocation(char* name);

	void close();
};

