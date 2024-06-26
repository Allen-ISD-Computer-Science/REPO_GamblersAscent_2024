#pragma once
#include "SDL.h"
#include "SDL_Image.h"
#include <string>

class SDL_Handler {
public:
	//screen size
	int screenWidth;
	int screenHeight;

	//looks for user input
	SDL_Event event{};

	//renderer
	SDL_Renderer* renderer;

	//window
	SDL_Window* m_window;

public:
	//constructor creates the window
	SDL_Handler(int screenWidth, int screenHeight);

	//destructor destroys window
	~SDL_Handler();

	//resizes the window
	void ResizeWindow(int windowWidth, int windowHeight);

	//clears the renderer
	int ClearRenderer();

	// renders the source rectangle of the texture to dest rectangle
	void DrawRectangle(SDL_Rect srcrect, SDL_Rect destrect, SDL_Texture* text);

private:


	//surface
	SDL_Surface* m_screen_surface;
};