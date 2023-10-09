#pragma once
#include "SDL_Handler.h"

class Spritesheet_Handler {
public:
	const int spriteWidth;
	const int spriteHeight;

public:
	//constructor
	Spritesheet_Handler(int width, int height, int rows, int columns);

	SDL_Rect getSprite(int x, int y);

private:
	//amount of rows on Spritesheet
	const int m_rows;

	//amount of columns on Spritesheet
	const int m_columns;

	//width of Spritesheet in pixels
	const int m_width;

	//height of Spritesheet in pixels
	const int m_height;
};