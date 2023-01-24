#include "Game.h"

bool Game::Init() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		cout << "SDL failed to initialize: " << SDL_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "SDL_Image failed to initialize: " << IMG_GetError() << endl;
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
	levelManager->LoadLevel();

	wallTexture = LoadTexture("Assets/wall.png");
	groundTexture = LoadTexture("Assets/ground.png");
	boxTexture = LoadTexture("Assets/box.png");
	goalTexture = LoadTexture("Assets/goal.png");

	player = new Player(this);

	InitLevel();

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

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				player->Move(1, 0);
				break;
			case SDLK_LEFT:
				player->Move(-1, 0);
				break;
			case SDLK_DOWN:
				player->Move(0, 1);
				break;
			case SDLK_UP:
				player->Move(0, -1);
				break;
			case SDLK_r:
				DestroyBoxes();
				InitLevel();
				break;
			case SDLK_s:
				GoToNextLevel();
				break;
			default:
				break;
			}
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
				SDL_RenderCopy(renderer, wallTexture, NULL, &rect);
			}
			else if (levelManager->levelMap[c][r] == 'g') {
				SDL_RenderCopy(renderer, goalTexture, NULL, &rect);
			}
			else {
				SDL_RenderCopy(renderer, groundTexture, NULL, &rect);
			}
		}
	}

	// Draw boxes
	for (int i = 0; i < boxes.size(); i++) {
		SDL_RenderCopy(renderer, boxTexture, NULL, boxes[i]->GetRect());
	}

	player->Draw(renderer);

	SDL_RenderPresent(renderer);
}

void Game::Shutdown() {
	SDL_DestroyTexture(wallTexture);
	SDL_DestroyTexture(groundTexture);
	SDL_DestroyTexture(boxTexture);
	SDL_DestroyTexture(goalTexture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* Game::LoadTexture(string path) {
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		cout << "Failed to load surface: " << IMG_GetError() << endl;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (newTexture == NULL) {
		cout << "Failed to conver to texture: " << SDL_GetError() << endl;
	}

	SDL_FreeSurface(tempSurface);

	return newTexture;
}

bool Game::HitWall(int x, int y) {
	return levelManager->levelMap[x][y] == 'x';
}

bool Game::CanPushBox(Box* box, int x, int y) {

	// Exit if trying to push into wall
	if (HitWall(x, y)) {
		return false;
	}

	// Exit if trying to push into another box
	for (int i = 0; i < boxes.size(); i++) {
		if (boxes[i] == box) {
			continue;
		}
		else if (x == boxes[i]->GetPos().x && y == boxes[i]->GetPos().y) {
			return false;
		}
	}

	return true;
}

bool Game::BoxUpdated(int moveX, int moveY, int pX, int pY) {
	Box* boxToPush = nullptr;

	// Find box attempting to push
	for (int i = 0; i < boxes.size(); i++) {
		if (pX == boxes[i]->GetPos().x && pY == boxes[i]->GetPos().y) {
			boxToPush = boxes[i];
		}
	}

	// Check if we can push the box, and if so, update the box
	if (boxToPush != nullptr) {
		int toPushX = pX + moveX;
		int toPushY = pY + moveY;

		if (CanPushBox(boxToPush, toPushX, toPushY)) {
			bool inGoal = HitGoal(toPushX, toPushY);
			boxToPush->Update(toPushX, toPushY, inGoal);
			if (AllGoalsComplete()) {
				GoToNextLevel();
				return false;
			}
		}
		else {
			return false;
		}
	}

	return true;
}

bool Game::HitGoal(int x, int y) {
	return levelManager->levelMap[x][y] == 'g';
}

bool Game::AllGoalsComplete() {
	for (int i = 0; i < boxes.size(); i++) {
		if (!boxes[i]->GetInGoal()) {
			return false;
		}
	}

	return true;
}

void Game::DestroyBoxes() {
	// Clean vector
	for (int i = 0; i < boxes.size(); i++)
	{
		delete boxes[i];
	}
	boxes.erase(boxes.begin(), boxes.end());
}

void Game::InitLevel() {
	// Reset Player and add new boxes
	for (int r = 0; r < TILE_ROWS; r++) {
		for (int c = 0; c < TILE_COLS; c++) {
			if (levelManager->levelMap[c][r] == 'p') {
				player->Reset(c, r);
			}
			else if (levelManager->levelMap[c][r] == 'b') {
				boxes.emplace_back(new Box(c, r));
			}
		}
	}
}

void Game::GoToNextLevel() {
	DestroyBoxes();

	// Go to next level
	levelManager->UpdateLevel();
	levelManager->LoadLevel();

	InitLevel();
}