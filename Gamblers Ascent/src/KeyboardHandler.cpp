#include "KeyboardHandler.h"
#include "SDL_Handler.h"
#include <iostream>

KeyboardHandler::KeyboardHandler(SDL_Handler* handler)

	:m_handler(*handler),
	KeyStates(SDL_GetKeyboardState(NULL)), DirectionalKeyPressed(false), W_key(false), A_key(false), S_key(false), D_key(false), E_key(false), B_key(false),
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

void KeyboardHandler::MoveBackgroundX(int& ScreenX, int& TrueX, int& ScreenWidth, int playerWidth)
{
	// X axis movement
	if ((!(LeftLimitReached || RightLimitReached)) && (ScreenX == (ScreenWidth / 2) - (playerWidth / 2)))
	{
		BackgroundX += BackgroundSpeedX;
		TrueX -= BackgroundSpeedX;
	}
	else {
		ScreenX -= BackgroundSpeedX;
		TrueX -= BackgroundSpeedX;
	}

	// recentering X axis if it gets off
	if ((BackgroundX < -(ScreenWidth / 2)) && (ScreenX < (ScreenWidth / 2) - (playerWidth / 2)))
	{
		ScreenX = (ScreenWidth / 2) - (playerWidth / 2);
		std::cout << "recentered\n";
	}
	else if ((BackgroundX > -(ScreenWidth / 2)) && (ScreenX > (ScreenWidth / 2) - (playerWidth / 2))) {
		ScreenX = (ScreenWidth / 2) - (playerWidth / 2);
		std::cout << "recentered\n";
	}
}

void KeyboardHandler::MoveBackgroundY(int& ScreenY, int& TrueY, int& ScreenHeight, int playerHeight)
{
	// Y axis movement
	if ((!(TopLimitReached || BottomLimitReached)) && (ScreenY == (ScreenHeight / 2) - (playerHeight / 2)))
	{
		BackgroundY += BackgroundSpeedY;
		TrueY += BackgroundSpeedY;
	}
	else {
		ScreenY -= BackgroundSpeedY;
		TrueY += BackgroundSpeedY;
	}

	// recentering Y axis if it gets off
	if ((BackgroundY < -(ScreenHeight / 2)) && (ScreenY < (ScreenHeight / 2) - (playerHeight / 2)))
	{
		ScreenY = (ScreenHeight / 2) - (playerHeight / 2);
	}
	else if ((BackgroundY > -(ScreenHeight / 2)) && (ScreenY > (ScreenHeight / 2) - (playerHeight / 2))) {
		ScreenY = (ScreenHeight / 2) - (playerHeight / 2);
	}
}

void KeyboardHandler::UpdatePlayerAnimation(int& consecutiveFramesHeld, int& playerState)
{
	if (BackgroundSpeedX != 0 || BackgroundSpeedY != 0) {
		consecutiveFramesHeld++;
	}
	else {
		consecutiveFramesHeld = 0;
		playerState = 1;
	}

	if (((consecutiveFramesHeld != 0) && ((consecutiveFramesHeld % 9) == 0)) || (consecutiveFramesHeld == 3)) {
		if (playerState < 4) {
			playerState++;
		}
		else {
			playerState = 1;
		}
	}
}

void KeyboardHandler::HandleEdgeCaseDirections(int& playerDirection)
{
	if (W_key && A_key && S_key && !D_key) //Moving left, top and bottom cancel
	{
		playerDirection = 2;
	}
	else if (W_key && A_key && S_key && D_key) //All keys pressed, face forward
	{
		playerDirection = 1;
	}
	else if (W_key && A_key && !S_key && D_key) //Moving up, left and right cancel
	{
		playerDirection = 4;
	}
	else if (!W_key && A_key && S_key && D_key) //Moving down, left and right cancel
	{
		playerDirection = 1;
	}
	else if (!W_key && A_key && !S_key && D_key) //left and right cancel, face forward
	{
		playerDirection = 1;
	}
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