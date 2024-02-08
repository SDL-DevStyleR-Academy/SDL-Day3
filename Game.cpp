//Game.cpp
#include "Game.h"
#include <iostream>

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL init success\n";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != 0) //window init success
		{
			std::cout << "window creation success\n";
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) //renderer init success
			{
				std::cout << "renderer creation success\n";
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			}
			else {
				std::cout << "renderer init failed\n";
				return false;
			}
		}
		else {
			std::cout << "window init failed\n";
			return false;
		}
	}
	else {
		std::cout << "SDL init fail\n";
		return false;
	}
	std::cout << "init success\n";
	running = true;
	return true;
}

bool Game::ttf_init() {
	if (TTF_Init() == -1) { // If the initialization of ttf fails, return false
		return false;
	}
	
	// You can find fonts at C:/WINDOWS/Fonts
	// or download them online
	TTF_Font* font1 = TTF_OpenFont("fonts/Arcade.ttf", 48);
	TTF_Font* font2 = TTF_OpenFont("fonts/segoepr.ttf", 72);


	if (font1 == NULL || font2 == NULL) { // return false if either font fails to load
		return false;
	}

	SDL_Surface* tempSurfaceText = NULL;

	// Try substituting 'TTF_RenderText_Blended()' with one of the following:
	// TTF_RenderText_LCD(), TTF_RenderText_Shaded, TTF_RenderText_Solid
	tempSurfaceText = TTF_RenderText_Blended(font1, "Hello World!", { 0, 0, 255, 255 });
	textTextureFont1 = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended(font2, "Hello World!", { 255, 0, 0, 255 });
	textTextureFont2 = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended_Wrapped(font1, "Hello World!\nThis wraps the text.", 
													{ 0, 255, 0, 255 }, 300);
	textTextureFont1Wrapped = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended_Wrapped(font2, "Hello World!\nThis wraps the text.",
													{ 255, 0, 255, 255 }, 500);
	textTextureFont2Wrapped = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended(font1, "CLICK", { 0, 0, 0, 255 });
	clickableTexture = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	int tw, th;
	// the texture created from the text has certain width and height,
	// get that width and height and give it to the destination rectangle
	// so the text does not appear weird and stretched on the window
	SDL_QueryTexture(textTextureFont1, 0, 0, &tw, &th); 
	dRectFont1 = { 10, 10, tw, th };

	SDL_QueryTexture(textTextureFont2, 0, 0, &tw, &th);
	dRectFont2 = { 10, 320, tw, th };

	SDL_QueryTexture(textTextureFont1Wrapped, 0, 0, &tw, &th);
	dRectFont1Wrapped = { 650, 10, tw, th };

	SDL_QueryTexture(textTextureFont2Wrapped, 0, 0, &tw, &th);
	dRectFont2Wrapped = { 650, 320, tw, th };

	SDL_QueryTexture(clickableTexture, 0, 0, &tw, &th);
	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh); // position the texture at the center of the window
	clickableRect = { (ww / 2 - tw / 2), (wh / 2 - th / 2), tw, th };

	SDL_FreeSurface(tempSurfaceText);
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);

	return true;
}

void Game::render() {
	SDL_RenderClear(renderer);

	// Render all of the textures created in the function above
	SDL_RenderCopy(renderer, textTextureFont1, NULL, &dRectFont1);
	SDL_RenderCopy(renderer, textTextureFont2, NULL, &dRectFont2);
	SDL_RenderCopy(renderer, textTextureFont1Wrapped, NULL, &dRectFont1Wrapped);
	SDL_RenderCopy(renderer, textTextureFont2Wrapped, NULL, &dRectFont2Wrapped);
	SDL_RenderCopy(renderer, clickableTexture, NULL, &clickableRect);

	SDL_RenderPresent(renderer);
}

// returns whether the texture 't' with rect 'r' has been clicked by the cursor 
bool Game::isClickableTextureClicked(SDL_Texture* t, SDL_Rect* r, int xDown, int yDown, int xUp, int yUp) {
	int tw, th;
	SDL_QueryTexture(t, 0, 0, &tw, &th);
	
	// 't' is rendered on the screen and lies within the coordinates below
	//(r->x)--------(r->x + tw)
	//     |        |
	//     |        |
	//(r->y)--------(r->y + th)
	
	// checks if the cursor position during the click time is inside the coordinates
	if ((xDown > r->x && xDown < (r->x + tw)) && (xUp > r->x && xUp < (r->x + tw)) &&
		(yDown > r->y && yDown < (r->y + th)) && (yUp > r->y && yUp < (r->y + th))) {
		// if all coordinates fall inside the rendered texture, the texture is clicked
		return true;
	}
	// the click was outside the texture
	return false;
}

void Game::handleEvents() {
	SDL_Event event; // https://wiki.libsdl.org/SDL2/SDL_Event
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT: running = false; break;
			case SDL_MOUSEBUTTONDOWN: {
				int msx, msy;
				if (event.button.button == SDL_BUTTON_LEFT) {
					// When the left mouse button is pressed down, record the cursor position
					// to be used when checking if a texture was clicked;
					SDL_GetMouseState(&msx, &msy);
					mouseDownX = msx;
					mouseDownY = msy;
				}
			}; break;
			case SDL_MOUSEBUTTONUP: {
				int msx, msy;
				if (event.button.button == SDL_BUTTON_LEFT) {
					// When the left mouse button is let go, check if the texture was pressed and log the result
					SDL_GetMouseState(&msx, &msy);
					std::cout << (isClickableTextureClicked(clickableTexture, &clickableRect, mouseDownX, mouseDownY, msx, msy) ? "CLICKED" : "NOT CLICKED");
					
				}
			}; break;
			case SDL_KEYDOWN: {
				// move the first texture with the arrow keys
				if (event.key.keysym.sym == SDLK_LEFT) {
;					dRectFont1.x--;
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					dRectFont1.x++;
				}
				if (event.key.keysym.sym == SDLK_UP) { // NOTE: lower y values are higher up on the screen
					dRectFont1.y--;
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					dRectFont1.y++;
				}
			}; break;
			case SDL_KEYUP: {

			}; break;
			case SDL_MOUSEMOTION: {
				// logs the x,y cordinates of the cursor when the mouse moves
				std::cout << event.motion.x << ":" << event.motion.y << std::endl;
				
				// move the second texture with the mouse
				//dRectFont2.x = event.motion.x - dRectFont2.w / 2;
				//dRectFont2.y = event.motion.y - dRectFont2.h / 2;
			}; break;

			default: break;
		}

	}
}

void Game::update() {

}

void Game::clean() {
	std::cout << "cleaning game\n";
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool Game::isRunning() {
	return Game::running;
}

Game::Game() {
	Game::window = NULL;
	Game::renderer = NULL;
	Game::running = true;
}

Game::~Game() {
	delete window;
	delete renderer;
}