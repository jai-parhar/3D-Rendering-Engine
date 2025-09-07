#pragma once

#include <iostream>
#include <vector>
#include "GameState.h"
#include "GameplayState.h"

class GameStateManager
{
public:
	int currentState;
	int switchState = -1;

	Renderer* renderer;
	Camera* camera;

	std::vector<GameState*> gameStates;

	int resW, resH;

	void initManager(int screenW, int screenH, Renderer* r, Camera* c);
	void initGameState(int state);
	void switchGameState(int state);
	void render();
	void update();
	void closeStates();
	void resetState(bool fullReset, int state);
};
