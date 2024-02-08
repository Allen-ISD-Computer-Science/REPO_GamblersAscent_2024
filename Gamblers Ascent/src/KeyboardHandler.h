#pragma once
#include "SDL_Handler.h"
#include "SDL_Image.h"

class KeyboardHandler
{
public:

	// background position
	int BackgroundX;
	int BackgroundY;

	// background speed
	int BackgroundSpeedX;
	int BackgroundSpeedY;

	// true if the background cannot be moved further without going off the screen
	bool RightLimitReached;
	bool LeftLimitReached;
	bool TopLimitReached;
	bool BottomLimitReached;

	// checks all the keys we use and is true if any are pressed
	bool DirectionalKeyPressed;

	// keys states
	bool E_key;
	bool B_key;



private:

	SDL_Handler m_handler;

	// keys states
	const Uint8* KeyStates;
	bool W_key;
	bool A_key;
	bool S_key;
	bool D_key;

public:

	// Constructor
	KeyboardHandler(SDL_Handler* handler);

	void UpdateKeyStates();
	void DirectionalKey(int& playerDirection);
	void UpdatePlayerAnimation(int& consecutiveFramesHeld, int& playerState);
	void HandleEdgeCaseDirections(int& playerDirection);

	void MoveBackgroundX(int& ScreenX, int& TrueX, int& ScreenWidth, int playerWidth);
	void MoveBackgroundY(int& ScreenY, int& TrueY, int& ScreenHeight, int playerHeight);

	void CheckBackgroundLimits();
private:

	//determines if players moving in more than 1 direction then adjusts the speeds so they correctly align with the path
	void CalculateCustomDiagonals(int& playerDirection);

	//sets BackgroundX and BackgroundY to 0, is called after every render of the background in main
	void ResetBackgroundSpeed();
};