#include "MouseHandler.h"

MouseHandler::MouseHandler(SDL_Handler* handler) : m_handler(handler), m_position({ -1, -1 })
{

};

SDL_Point MouseHandler::getPosition()
{
	SDL_GetMouseState(&m_position.x, &m_position.y);
	return { m_position.x, m_position.y };
}


