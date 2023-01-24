#pragma once

#include "Utils.h"
#include "LevelManager.h"
#include "Player.h"
#include "Box.h"

#include <vector>

class Game
{
public:
	bool Init();
	void GameLoop();
	void Shutdown();

	SDL_Texture* LoadTexture(string path);
	bool HitWall(int x, int y);
	bool BoxUpdated(int moveX, int moveY, int pX, int pY);

private:
	void HandleEvents();
	void Update();
	void Draw();

	bool HitGoal(int x, int y);
	bool AllGoalsComplete();
	void DestroyBoxes();
	void InitLevel();
	void GoToNextLevel();
	bool CanPushBox(Box* box, int x, int y);

	bool isRunning = true;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* wallTexture = nullptr;
	SDL_Texture* groundTexture = nullptr;
	SDL_Texture* boxTexture = nullptr;
	SDL_Texture* goalTexture = nullptr;

	class LevelManager* levelManager;
	class Player* player;
	vector<Box*> boxes;
};

