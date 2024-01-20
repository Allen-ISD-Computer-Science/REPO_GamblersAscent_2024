#pragma once
#include "SDL_Handler.h"


class MouseHandler {
public:
	MouseHandler(SDL_Handler *handler);
	// Function that updates the mouse events
	// Should be called once per frame
	SDL_Point getPosition();

private:
	SDL_Handler *m_handler;

	SDL_Point m_position;
	int m_X, m_Y;
	int buttonCount;
	SDL_Rect* m_buttons[];
	
};