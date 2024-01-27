#include "Button.h"

Button::Button(int x, int y, int w, int h) : rect({ x, y, w, h }), isClicked(false) {}

void Button::handleEvents(SDL_Event* e) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		SDL_Point p = { x, y };
		if (SDL_PointInRect(&p, &rect)) {
			isClicked = true;
		}
	}
};