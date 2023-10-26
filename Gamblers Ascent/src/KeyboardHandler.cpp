#include "KeyboardHandler.h"
#include "SDL_Handler.h"
#include <iostream>
#include <algorithm>

KeyboardHandler::KeyboardHandler(SDL_Handler* handler)
	:m_handler(*handler), backgroundSpeedX(0), backgroundSpeedY(0), playerSpeedX(0), playerSpeedY(0),
    up(false), down(false), left(false), right(false),
    upRight(false), upLeft(false), downRight(false), downLeft(false), interact(false),
    screenCenterX(0), screenCenterY(0), playerCenteredX(true), playerCenteredY(true), mapCenterX(0), mapCenterY(0),
    backgroundX(-500.0f), backgroundY(-345.0f), playerX(0), playerY(0)
{

}

KeyboardHandler::~KeyboardHandler()
{
}

void KeyboardHandler::updateKeyboardState()
{
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    
    // Checking for movement inputs
    up = keyState[SDL_SCANCODE_W];
    down = keyState[SDL_SCANCODE_S];
    left = keyState[SDL_SCANCODE_A];
    right = keyState[SDL_SCANCODE_D];

    // Checking for interaction inputs
    interact = keyState[SDL_SCANCODE_E];

    // Check for diagonals
    upLeft = up && left;
    upRight = up && right;
    downLeft = down && left;
    downRight = down && right;
}

void KeyboardHandler::updateBackground(int& playerDirection)
{
    // checking the keyboards state and moving background based off WASD input and
    if (upLeft)
    {
		playerDirection = 7;
        backgroundSpeedX = 5;
		backgroundSpeedY = 2.5;
	}
    else if (upRight)
    {
        playerDirection = 8;
        backgroundSpeedX = -5;
        backgroundSpeedY = 2.5;
	}
    else if (downLeft)
    {
        playerDirection = 6;
        backgroundSpeedX = 5;
		backgroundSpeedY = -2.5;
	}
    else if (downRight)
    {
        playerDirection = 5;
        backgroundSpeedX = -5;
		backgroundSpeedY = -2.5;
	}
    else if (up)
    {
        playerDirection = 4;
        backgroundSpeedY = 5;
        backgroundSpeedX = 0;
	}
    else if (down)
    {
        playerDirection = 1;
        backgroundSpeedY = -5;
        backgroundSpeedX = 0;
           
	}
    else if (left)
    {
        playerDirection = 2;
		backgroundSpeedX = 5;
        backgroundSpeedY = 0;
	}
    else if (right)
    {
        playerDirection = 3;
        backgroundSpeedX = -5;
        backgroundSpeedY = 0;
	}
    else
    {
        backgroundSpeedX = 0;
		backgroundSpeedY = 0;
	}
    // Move background, and if it will clip the edge of the map ClampBackgroundPosition will correct it.
    backgroundX += backgroundSpeedX;
    backgroundY += backgroundSpeedY;

    // Move the center of the map
    mapCenterX += backgroundSpeedX;
    mapCenterY += backgroundSpeedY;

}  

void KeyboardHandler::ClampBackgroundPosition() {
    // Calculate the valid range for the background and
    // Ensures the background's position stays within the valid range
    
    float tempX = backgroundX;
    float tempY = backgroundY;

    backgroundX = std::max(minX, std::min(backgroundX, maxX));
    backgroundY = std::max(minY, std::min(backgroundY, maxY));
    // If the background's position is out of range, move the player instead of the background. Also won't
    // move the background if the player isn't centered yet
    if (playerCenteredX) {
        if (tempX != backgroundX) {
            playerSpeedX = -backgroundSpeedX;
            playerCenteredX = false;
        }
    }
    else {
        if (!inRange(screenCenterX - 2.5, screenCenterX + 2.5, playerX)) {
			playerSpeedX = -backgroundSpeedX;
            backgroundX -= backgroundSpeedX;
		}
        else {
			playerSpeedX = 0;
			playerCenteredX = true;
		}
        
	}
    if (playerCenteredY) {
		if (tempY != backgroundY) {
			playerSpeedY = -backgroundSpeedY;
			playerCenteredY = false;
		}
	}
    else {
        if (!inRange(screenCenterY-2.5, screenCenterY+2.5, playerY)) {
            playerSpeedY = -backgroundSpeedY;
            backgroundY -= backgroundSpeedY;
        }
        else {
            playerSpeedY = 0;
            playerCenteredY = true;
        }   
    }
    playerX += playerSpeedX;
    playerY += playerSpeedY;
}

bool KeyboardHandler::inRange(unsigned low, unsigned high, unsigned x)
{
    return  ((x - low) <= (high - low));
}