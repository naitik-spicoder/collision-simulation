#include<SDL3/SDL.h>
#include<SDL3_image/SDL_image.h>

using namespace std;

struct Ball {

	double vx, vy;
	int gravity = 600;
};

struct Floor {

	float x, y, width, height;
};


int main(){

	//check if it initialize or not
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Error initializing SDL3", nullptr);
		return 1;
	}
	
	//create a window
	int height = 800;
	int width = 600;
	//creat a window
	SDL_Window* win = SDL_CreateWindow("first window", height, width, 0);
	if (!win) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error in creating window", nullptr);
		SDL_Quit();
		return 1;
	}

	//creat a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL);
	SDL_Texture* texture = IMG_LoadTexture(renderer, "mypng.jpg");
	if (!texture) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Texture Error", "Texture not found", nullptr);
		SDL_Quit();
	}
	SDL_FRect rect;
	rect.w = 50;
	rect.h = 50;
	rect.x = 75;
	rect.y = 5;

	//render the stuff
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderFillRect(renderer, &rect);
	//SDL_RenderTexture(renderer, texture, NULL, &rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect floor = { 50,346,650,50 };
	SDL_RenderFillRect(renderer, &floor);
	SDL_RenderPresent(renderer);

	Ball ball{};
	bool jump = false;

	//game loop
	bool running = true;
	SDL_Event e;
	Uint64 lastTime = SDL_GetTicks();
	bool onground = false;

	while (running) {

		//time managment
		Uint64 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;
		bool applyfric = false;

		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_EVENT_QUIT) {
				running = false;
				SDL_Log("goodbye");
			}
			//controller
			if (e.type == SDL_EVENT_KEY_DOWN) {
				if (e.key.key == SDLK_SPACE) {
					jump = true;
				}
				if (e.key.key == SDLK_D) {
					ball.vx += 100 * deltaTime;
				}
			}

			if (e.type == SDL_EVENT_KEY_UP) {
				if (e.key.key == SDLK_D) {
					applyfric = true;
					SDL_Log("kam kar");
				}
			}

		}

		//updating screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_FRect floor = { 50,346,650,50 };
		SDL_RenderFillRect(renderer, &floor);
		SDL_RenderPresent(renderer);

		//movement
		ball.vy += ball.gravity * deltaTime;
		rect.y += ball.vy * deltaTime;
		//rect.x += ball.vx * deltaTime;

		//collision
		if (rect.y + rect.h >= floor.y && rect.x < floor.x + floor.w) {
			rect.y = 346 - rect.h;
			onground = true;
			float vybefColl = ball.vy;
			if (ball.vy > 0) {
				ball.vy = -vybefColl / 2;
			}
			if (jump && onground) {
				ball.vy = -250;
				jump = false;
			}

		}
	}

	SDL_DestroyWindow(win);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 1;
}
