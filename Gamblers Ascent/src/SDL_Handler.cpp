#include "SDL_Handler.h"
#include <iostream>

//constructor
SDL_Handler::SDL_Handler(int width, int height)
	:m_screen_surface(NULL), screenWidth(width), screenHeight(height) {
	//initializing video & audio with error catching
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL video could not be initialized: " << SDL_GetError();
	} else {
		std::cout << "SDL video system is ready to go.\n";
	}

	//initializing ability to render PNG files
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cout << "SDL_image for PNG Files could not be initialized: " << IMG_GetError << "\n";
	}

	//creating window
	m_window = SDL_CreateWindow("Gamblers Ascent",	//window title
		SDL_WINDOWPOS_CENTERED,	//initial x position
		SDL_WINDOWPOS_CENTERED,	//initial y position
		screenWidth,	//width in pixels
		screenHeight,	//height in pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);	//Window Type / flags

	//creating renderer
	renderer = SDL_CreateRenderer(m_window, -1, 0);
}

//destructor
SDL_Handler::~SDL_Handler() {
	SDL_FreeSurface(m_screen_surface);
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

//window resizer
void SDL_Handler::ResizeWindow(int windowWidth, int windowHeight) {
	screenWidth = windowWidth;
	screenHeight = windowHeight;
	SDL_UpdateWindowSurface(m_window);
}

//renderer clearer
int SDL_Handler::ClearRenderer() {
	return SDL_RenderClear(renderer);
}

//assigning a rectangle to the file that was loaded
void SDL_Handler::DrawRectangle(SDL_Rect srcrect, SDL_Rect destrect, SDL_Texture* texture) {
	if (texture) {
		SDL_RenderCopy(renderer, texture, &srcrect, &destrect);

	} else {
		std::cout << "DrawRectangle: text was nullptr\n";
	}
}

