//Game.h
#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
class Game {
public:
	Game();
	~Game();
	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool ttf_init(); // Initializes the SDL_ttf library and loads the example textures 
	void render();
	void update();
	void handleEvents();
	void clean();
	bool isRunning();
	bool isClickableTextureClicked(SDL_Texture* t, SDL_Rect* r, int xDown, int yDown, int xUp, int yUp);
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool running;
	
	// 4 textures to hold the 4 pieces of text we want to render on the window
	SDL_Texture *textTextureFont1,
				*textTextureFont2,
				*textTextureFont1Wrapped,
				*textTextureFont2Wrapped;
	
	// 4 destination rectangles for each texture,
	// used to render the text in the right place and with the right size on the window
	SDL_Rect dRectFont1, dRectFont2, dRectFont1Wrapped, dRectFont2Wrapped;
	
	// One more texture which will act as a button,
	// clicking on it needs to be handled
	SDL_Texture* clickableTexture;
	
	// The destination rectangle for the texture above
	SDL_Rect clickableRect;

	// used to hold a record of the x,y coordinates when clicking the mouse
	int mouseDownX, mouseDownY;
};
