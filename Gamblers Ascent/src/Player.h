#pragma once
#include "SDL_Handler.h"

class Player {
public:
	//constructor
	Player(SDL_Handler* handler, int playerWidth, int playerHeight);

	//destructor
	~Player();

	//renders a tile object
	void render(SDL_Texture* texture, SDL_Rect srcrect);

private:
	const int m_playerWidth;
	const int m_playerHeight;

protected:
	SDL_Texture* m_texture;

	SDL_Handler &m_handler;
};