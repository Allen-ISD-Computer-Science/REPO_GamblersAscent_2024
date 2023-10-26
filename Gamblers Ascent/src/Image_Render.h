#pragma once
#include "SDL_Handler.h"

class Image_Render {
public:
	//constructor
	Image_Render(SDL_Handler* handler, int playerWidth, int playerHeight);

	//destructor
	~Image_Render();

	//renders a tile object
	void render(SDL_Texture* texture, SDL_Rect srcrect, int imageX, int imageY);

private:
	const int m_imageWidth;
	const int m_imageHeight;

protected:
	SDL_Texture* m_texture;

	SDL_Handler &m_handler;
};