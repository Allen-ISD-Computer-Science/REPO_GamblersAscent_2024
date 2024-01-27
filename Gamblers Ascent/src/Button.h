#pragma once
#include "SDL_Handler.h"

// This class is used to create button objects. Rendering will still be handled with Image_Rendering class. This class is used to check if the button is clicked or not.
class Button {
public:
	Button(int x, int y, int w, int h);

	SDL_Rect rect;
	
	void handleEvents(SDL_Event* event);

	bool isClicked;
};