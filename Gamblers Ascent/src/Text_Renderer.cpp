#include "Text_Renderer.h"
#include <iostream>

TextRenderer::TextRenderer(SDL_Handler* handler, std::string fontPath, int size)
	:m_handler(*handler)
{
	TTF_Init();
	font = TTF_OpenFont(fontPath.c_str(), size);
};

TextRenderer::~TextRenderer()
{
	TTF_CloseFont(font);
};

void TextRenderer::renderString(std::string string, int x, int y, SDL_Color color)
{
	int temp1 = 1;
	int temp2 = 2;
	int *stringWidth = &temp1;
	int *stringHeight = &temp2;

	if ((TTF_SizeUTF8(font, string.c_str(), stringWidth, stringHeight)) == 0)
	{
		SDL_Rect srcrect = {0, 0, *stringWidth, *stringHeight};
		SDL_Rect destrect = {x, y, *stringWidth, *stringHeight };
		SDL_Texture* texture = createTexture(string.c_str(), color);

		m_handler.DrawRectangle(srcrect, destrect, texture);
	}
};

SDL_Texture* TextRenderer::createTexture(const char* text, SDL_Color color)
{
	SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text, color);
	return SDL_CreateTextureFromSurface(m_handler.renderer, textSurface);
};