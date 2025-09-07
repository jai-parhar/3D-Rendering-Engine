#pragma once

#include "Renderer.h"
#include "Camera.h"

class GameState
{
public:
	
	int* switchState;
	Renderer* renderer;
	Camera* camera;

	virtual void init(int* swState, Renderer* r, Camera* c) = 0;
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void close() = 0;
	virtual void resetState(bool fullReset) = 0;
};

