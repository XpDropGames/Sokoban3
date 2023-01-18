#pragma once

#include "Utils.h"
#include "LevelManager.h"

class Game
{
public:
	bool Init();
	void GameLoop();
	void Shutdown();

private:
	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning = true;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class LevelManager* levelManager;
};

