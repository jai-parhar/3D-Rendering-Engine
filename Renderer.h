#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdarg>

#include "Shader.h"
#include "Camera.h"

class Renderer
{
public:

	GLuint VertexArrayID;

	Shader* currentShader;

	GLuint FBO;
	GLuint depthRenderBuffer;

	void init(int screenW, int screenH);
	void attachTextureToFBO(GLuint &texture, GLuint attachmentType = GL_COLOR_ATTACHMENT0);
	void setShader(Shader* shader);
	void setTexture(GLuint texture);
	void renderMesh(Camera* cam, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer, GLuint elementbuffer, int numIndices,
		glm::vec3 translation = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
	void renderMesh(Camera* cam, GLuint vertexbuffer, int meshSize,
		glm::vec3 translation = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1));
	void renderToScreen(int x, int y, int w, int h);
	void renderToFBO(int x, int y, int w, int h, GLuint attachmentType = GL_COLOR_ATTACHMENT0);
	void renderToTargetBuffers(int x, int y, int w, int h, int count, ...);
	void close();

};

