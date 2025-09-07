#pragma once
#include "GameState.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <vector>
#include "Utils.h"
#include "Camera.h"

class GameplayState :
	public GameState
{
public:

	int screenW, screenH;

	Shader baseShader;
	Shader screenShader;
	Shader weaponShader;

	GLuint cVbuffer;
	GLuint cUVbuffer;
	GLuint cNbuffer;
	GLuint cEbuffer;
	GLuint cTexture;
	int cObjSize;

	GLuint swordVbuffer;
	GLuint swordUVbuffer;
	GLuint swordNbuffer;
	GLuint swordEbuffer;
	GLuint swordTexture;
	int swordObjSize;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint texture;
	int objSize;

	GLuint vertexbuffer2;
	GLuint uvbuffer2;
	GLuint normalbuffer2;
	GLuint elementbuffer2;
	GLuint texture2;
	int objSize2;

	GLuint vertexbuffer3;
	GLuint uvbuffer3;
	GLuint normalbuffer3;
	GLuint elementbuffer3;
	GLuint texture3;
	int objSize3;

	GLuint screenTexture;
	GLuint screenVBuffer;
	
	float angle = 0.0f;
	
	void init(int* swState, Renderer* r, Camera* c);
	void render();
	void update();
	void close();
	void resetState(bool fullReset);
};

