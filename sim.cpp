#include<SDL3/SDL.h>
#include<iostream>

using namespace std;

#define e 0.8

struct Ball {

	double vx, vy, Afric = 0.99, Vres;
	int gravity = 600;
};

struct Floor {

	float x, y, width, height;
};


int main() {

	//check if it initialize or not
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Error initializing SDL3", nullptr);
		return 1;
	}

	//create a window
	int height = 800;
	int width = 600;
	//creat a window
	SDL_Window* win = SDL_CreateWindow("Collision Simulation", height, width, 0);
	if (!win) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error in creating window", nullptr);
		SDL_Quit();
		return 1;
	}

	//creat a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(win, NULL);
	SDL_FRect rect;
	rect.w = 50;
	rect.h = 50;
	rect.x = 75;
	rect.y = 5;

	SDL_FRect rect2;
	rect.w = 50;
	rect.h = 50;
	rect.x = 105;
	rect.y = 5;

	//render the stuff
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderFillRect(renderer, &rect);
	SDL_RenderFillRect(renderer, &rect2);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_FRect floor = { 50,346,650,50 };
	SDL_RenderFillRect(renderer, &floor);
	SDL_RenderPresent(renderer);

	Ball ball{};
	bool jump = false;

	//game loop
	bool running = true;
	SDL_Event event;
	Uint64 lastTime = SDL_GetTicks();
	bool onground = false;

	while (running) {

		//time managment
		Uint64 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;
		jump = false;

		//event management
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				SDL_Log("goodbye");
			}
			//controller
			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (event.key.key == SDLK_SPACE) {
					jump = true;
				}
				if (event.key.key == SDLK_D) {
					ball.vx += 100;
				}
				if (event.key.key == SDLK_A) {
					ball.vx = -100;
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

		//falling motion
		ball.vy += ball.gravity * deltaTime;
		rect.y += ball.vy * deltaTime;

		//friction
		if (ball.vx > 0 && onground){
			rect.x += ball.vx * deltaTime;
			ball.vx = ball.vx*ball.Afric;
		}
		if (ball.vx < 0 && onground) {
			rect.x += ball.vx * deltaTime;
			ball.vx = ball.vx * ball.Afric;
		}

		//collision
		if (rect.y + rect.h >= floor.y && rect.x < floor.x + floor.w) {
			rect.y = 346 - rect.h;
			onground = true;
			float vybefColl = ball.vy;
			if (ball.vy > 0) {
				ball.vy = -vybefColl*e;
			}
			if (jump && onground) {
				ball.vy = -500;
				jump = false;
			}

		}
	}

	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 1;
}

