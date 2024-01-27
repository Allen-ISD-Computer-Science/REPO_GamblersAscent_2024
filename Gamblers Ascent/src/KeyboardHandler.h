#pragma once
#include "SDL_Handler.h"
#include "SDL_Image.h"
#include <string>

class KeyboardHandler
{
public:

	// background movement
	float backgroundSpeedX;
	float backgroundSpeedY;

	// player movement
	float playerSpeedX;
	float playerSpeedY;

	// background position
	float backgroundX;
	float backgroundY;

	// player position
	float playerX;
	float playerY;

	// Center of screen
	float screenCenterX;
	float screenCenterY;

	// Center of the map. Used to determine which section of the map the player is in
	float mapCenterX;
	float mapCenterY;

	// Key direction
	bool up, down, left, right, upRight, upLeft, downRight, downLeft, interact, blackjackBattle;
	
private:
	SDL_Handler m_handler;
	const float maxX = 0.0f;
	const float maxY = 0.0f;
	const float minX = -640;
	const float minY = -360;
	bool playerCenteredX;
	bool playerCenteredY;

public:
	// Constructor
	KeyboardHandler(SDL_Handler* handler);

	// Destructor
	~KeyboardHandler();

	// Run code based off of key presses
	void updateKeyboardState();
	// Updates background movement speed
	void updateBackground(int& playerDirection);
	// Fixes background and moves player
	void ClampBackgroundPosition();
	

private:

	bool inRange(unsigned low, unsigned high, unsigned x);

};