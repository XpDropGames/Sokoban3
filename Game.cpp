#include "Game.h"

bool Game::Init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		cout << "SDL failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow("SOKOBAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		cout << "Window failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		cout << "Renderer failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	levelManager = new LevelManager();
	levelManager->LoadLevel("Levels/level1.txt");

	return true;
}

void Game::GameLoop() {
	while (isRunning) {
		HandleEvents();
		Update();
		Draw();
	}
}

void Game::HandleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}
}

void Game::Update() {

}

void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderClear(renderer);

	for (int r = 0; r < TILE_ROWS; r++) {
		for (int c = 0; c < TILE_COLS; c++) {
			SDL_Rect rect = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };

			if (levelManager->levelMap[c][r] == 'x') {
				SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}

			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_RenderPresent(renderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}