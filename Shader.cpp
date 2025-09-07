#include "Shader.h"

void Shader::init(const char * vertex_file_path, const char * fragment_file_path)
{
	shaderID = LoadShaders(vertex_file_path, fragment_file_path);
	uniformLocations["ModelMatrix"] = -1;
	uniformLocations["ViewMatrix"] = -1;
	uniformLocations["ProjectionMatrix"] = -1;
}

void Shader::useShader()
{
	glUseProgram(shaderID);
}

void Shader::addUniform(char * name)
{
	uniformLocations[name] = glGetUniformLocation(shaderID, name);
}

GLuint Shader::getUniformLocation(char * name)
{
	return uniformLocations[name];
}

void Shader::close()
{
	glDeleteProgram(shaderID);
}