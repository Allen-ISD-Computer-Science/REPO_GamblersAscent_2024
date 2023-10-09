#include "Spritesheet_Handler.h"

Spritesheet_Handler::Spritesheet_Handler(int width, int height, int rows, int columns)
	:m_width(width), m_height(height), m_rows(rows), m_columns(columns), spriteWidth(width / rows), spriteHeight(height / columns) {
}

SDL_Rect Spritesheet_Handler::getSprite(int x, int y) {
	SDL_Rect srcrect = { (x - 1) * spriteWidth, (y - 1) * spriteHeight , spriteWidth, spriteHeight };
	return srcrect;
}
