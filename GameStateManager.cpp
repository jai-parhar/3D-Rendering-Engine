#include "GameStateManager.h"

void GameStateManager::initManager(int screenW, int screenH, Renderer* r, Camera* c)
{
	renderer = r;
	camera = c;
	resW = screenW;
	resH = screenH;
	gameStates.push_back(new GameplayState());
	switchGameState(0);
}

void GameStateManager::initGameState(int state) 
{
	gameStates.at(state)->init(&switchState, renderer, camera);
}

void GameStateManager::switchGameState(int state) 
{
	currentState = state;
	initGameState(currentState);
}

void GameStateManager::render() 
{
	gameStates.at(currentState)->render();
}

void GameStateManager::update() {
	gameStates.at(currentState)->update();
}

void GameStateManager::closeStates() 
{
	gameStates.at(currentState)->close();
}

void GameStateManager::resetState(bool fullReset, int state) 
{
	gameStates.at(state)->resetState(fullReset);
}