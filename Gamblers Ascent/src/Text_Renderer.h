#pragma once
#include "SDL_Handler.h"
#include "SDL_ttf.h"

class TextRenderer {
public:

	//opens the font upon creation of the class
	TextRenderer(SDL_Handler* handler, std::string fontPath, int size);

	//closes the font to prevent memory leaks
	~TextRenderer();

	void renderString(std::string string, int x, int y, SDL_Color color);
private:
	SDL_Texture* createTexture(const char* text, SDL_Color color);

private:
	TTF_Font* font;
	SDL_Handler &m_handler;
};