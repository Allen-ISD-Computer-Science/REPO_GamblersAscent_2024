#include "MouseHandler.h"

MouseHandler::MouseHandler(SDL_Handler* handler) : m_handler(handler), m_X(0), m_Y(0), m_position({ -1, -1 })
{

};

SDL_Point MouseHandler::getPosition()
{
	SDL_GetMouseState(&m_X, &m_Y);
	return { m_X, m_Y };
}


