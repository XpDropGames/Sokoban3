#pragma once

#include "Utils.h"
#include "LevelManager.h"
#include "Player.h"

class Game
{
public:
	bool Init();
	void GameLoop();
	void Shutdown();

	SDL_Texture* LoadTexture(string path);
	bool HitWall(int x, int y);
	bool HitBox(int x, int y);
	bool PushBox(int moveX, int moveY, int pX, int pY);

private:
	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning = true;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* wallTexture = nullptr;
	SDL_Texture* groundTexture = nullptr;
	SDL_Texture* boxTexture = nullptr;
	SDL_Texture* goalTexture = nullptr;

	class LevelManager* levelManager;
	class Player* player;
};

