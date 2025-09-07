#include "Renderer.h"

void Renderer::init(int screenW, int screenH)
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	FBO = 0;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	depthRenderBuffer = 0;
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenW, screenH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
}

void Renderer::attachTextureToFBO(GLuint & texture, GLuint attachmentType)
{
	glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, texture, 0);
}

void Renderer::setShader(Shader* shader)
{
	currentShader = shader;
	currentShader->useShader();
}

void Renderer::setTexture(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(currentShader->getUniformLocation("TexSampler"), 0);
}

void Renderer::renderMesh(Camera* cam, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer, GLuint elementbuffer, int numIndices, 
	glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, translation);
	ModelMatrix = glm::scale(ModelMatrix, scale);

	if(rotation.x != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	}

	if (rotation.y != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	}

	if (rotation.z != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	}

	glUniformMatrix4fv(currentShader->getUniformLocation("ModelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(currentShader->getUniformLocation("ProjectionMatrix"), 1, GL_FALSE, &cam->getProjectionMatrix()[0][0]);
	glUniformMatrix4fv(currentShader->getUniformLocation("ViewMatrix"), 1, GL_FALSE, &cam->getViewMatrix()[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		nullptr             // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute 1. must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		nullptr                           // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		nullptr                           // array buffer offset
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	glDrawElements(
		GL_TRIANGLES,      // mode
		numIndices,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Renderer::renderMesh(Camera* cam, GLuint vertexbuffer, int meshSize,
	glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, translation);
	ModelMatrix = glm::scale(ModelMatrix, scale);

	if (rotation.x != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	}

	if (rotation.y != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	}

	if (rotation.z != 0)
	{
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	}

	glUniformMatrix4fv(currentShader->getUniformLocation("ModelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(currentShader->getUniformLocation("ProjectionMatrix"), 1, GL_FALSE, &cam->getProjectionMatrix()[0][0]);
	glUniformMatrix4fv(currentShader->getUniformLocation("ViewMatrix"), 1, GL_FALSE, &cam->getViewMatrix()[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		nullptr             // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, meshSize);

	glDisableVertexAttribArray(0);
}

void Renderer::renderToFBO(int x, int y, int w, int h, GLuint attachmentType)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(x, y, w, h);
	glDrawBuffer(attachmentType);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderToTargetBuffers(int x, int y, int w, int h, int count, ...)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(x, y, w, h);
	std::vector<GLuint> renderTargets;
	
	va_list varArguments;
	va_start(varArguments, count);
	for (int i = 0; i < count; i++)
	{
		renderTargets.push_back(va_arg(varArguments, GLuint));
	}
	va_end(varArguments);

	glDrawBuffers(count, &renderTargets[0]);
}

void Renderer::renderToScreen(int x, int y, int w, int h)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(x, y, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::close()
{
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &depthRenderBuffer);
}