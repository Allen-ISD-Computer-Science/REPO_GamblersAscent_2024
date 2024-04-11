#pragma once
#include "SDL_Handler.h"


class MouseHandler {
public:
	MouseHandler(SDL_Handler *handler);

	SDL_Point getPosition();

private:
	SDL_Handler *m_handler;

	SDL_Point m_position;

	int buttonCount;
	SDL_Rect* m_buttons[1];
	
};