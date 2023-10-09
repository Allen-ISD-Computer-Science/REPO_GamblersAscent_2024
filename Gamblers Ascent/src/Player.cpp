#include "Player.h"

//constructor
Player::Player(SDL_Handler* handler, int playerWidth, int playerHeight)
	:m_handler(*handler), m_texture(nullptr), m_playerWidth(playerWidth), m_playerHeight(playerHeight) {
}

//destructor
Player::~Player() {
}

void Player::render(SDL_Texture* texture, SDL_Rect srcrect) {
	m_texture = texture;

	SDL_Rect destrect = { ((m_handler.screenWidth) / 2) - (m_playerWidth / 2), //topleft player position x axis (centered)
						  ((m_handler.screenHeight) / 2) - (m_playerHeight / 2), //topleft player position y axis (centered)
						  m_playerWidth, //player width
						  m_playerHeight }; //player height

	m_handler.DrawRectangle(srcrect, destrect, m_texture);

}