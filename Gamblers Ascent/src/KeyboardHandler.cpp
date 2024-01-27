#include "KeyboardHandler.h"
#include "SDL_Handler.h"
#include <iostream>

KeyboardHandler::KeyboardHandler(SDL_Handler* handler)
	:m_handler(*handler),
	KeyStates(SDL_GetKeyboardState(NULL)), DirectionalKeyPressed(false), W_key(false), A_key(false), S_key(false), D_key(false), E_key(false),
	BackgroundSpeedX(0), BackgroundSpeedY(0), BackgroundX(-500), BackgroundY(-345),
	RightLimitReached(false),  LeftLimitReached(false), TopLimitReached(false), BottomLimitReached(false)
{
}

void KeyboardHandler::DirectionalKey(int& playerDirection)
{
	if (W_key){
		playerDirection = 4;
		BackgroundSpeedY += 4;
	}
	if (A_key) {
		playerDirection = 2;
		BackgroundSpeedX += 4;
	}
	if (S_key) {
		playerDirection = 1;
		BackgroundSpeedY -= 4;
	}
	if (D_key) {
		playerDirection = 3;
		BackgroundSpeedX -= 4;
	}
	CalculateCustomDiagonals(playerDirection);
}

void KeyboardHandler::ResetBackgroundSpeed()
{
	BackgroundSpeedX = 0;
	BackgroundSpeedY = 0;
}

void KeyboardHandler::UpdateKeyStates()
{
	KeyStates = SDL_GetKeyboardState(NULL);
	W_key = KeyStates[SDL_SCANCODE_W];
	A_key = KeyStates[SDL_SCANCODE_A];
	S_key = KeyStates[SDL_SCANCODE_S];
	D_key = KeyStates[SDL_SCANCODE_D];
	E_key = KeyStates[SDL_SCANCODE_E];

	DirectionalKeyPressed = W_key || A_key || S_key || D_key;
}

void KeyboardHandler::CalculateCustomDiagonals(int& playerDirection)
{
	if (BackgroundSpeedX > 0 && BackgroundSpeedY > 0)
	{
		playerDirection = 7;
		BackgroundSpeedX = 4;
		BackgroundSpeedY = 2;
	}
	else if (BackgroundSpeedX < 0 && BackgroundSpeedY > 0)
	{
		playerDirection = 8;
		BackgroundSpeedX = -4;
		BackgroundSpeedY = 2;
	}
	else if (BackgroundSpeedX > 0 && BackgroundSpeedY < 0)
	{
		playerDirection = 6;
		BackgroundSpeedX = 4;
		BackgroundSpeedY = -2;
	}
	else if (BackgroundSpeedX < 0 && BackgroundSpeedY < 0)
	{
		playerDirection = 5;
		BackgroundSpeedX = -4;
		BackgroundSpeedY = -2;
	}
}

void KeyboardHandler::CheckBackgroundLimits()
{
	//right border
	if (-m_handler.screenWidth > (BackgroundX + BackgroundSpeedX))
	{
		RightLimitReached = true;
	}
	else {
		RightLimitReached = false;
	}

	//left border
	if (0 < (BackgroundX + BackgroundSpeedX))
	{
		LeftLimitReached = true;
	}
	else {
		LeftLimitReached = false;
	}

	//bottom border
	if (-m_handler.screenHeight > (BackgroundY + BackgroundSpeedY))
	{
		BottomLimitReached = true;
	}
	else {
		BottomLimitReached = false;
	}

	//top border
	if (0 < (BackgroundY + BackgroundSpeedY))
	{
		TopLimitReached = true;
	}
	else {
		TopLimitReached = false;
	}
}