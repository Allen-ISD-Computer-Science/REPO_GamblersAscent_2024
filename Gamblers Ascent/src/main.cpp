#include "SDL_Handler.h"
#include "Asset_Manager.h"
#include "Image_Render.h"
#include "Spritesheet_Handler.h"
#include "KeyboardHandler.h"
#include <iostream>

int main(int argc, char* argv[])
{
	//creating a handler to manage all user input in the program
	SDL_Handler handler(640, 360); //window width, window height

	//base path of the program
	std::string basePath = (std::string)SDL_GetBasePath();

	//filePath Array
	static const int AssetCount = 2;
	std::string filePaths[AssetCount] = {
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\playerSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\finalfloor00.png"
	};

	//asset manager
	Asset_Manager Asset_Manager(filePaths, handler.renderer);
	Asset_Manager.LoadAssets();

	//Spritesheet Handler
	Spritesheet_Handler playerSpritesheet(256, 512, 4, 8); //width, height, rows, columns

	//player variables
	const int playerWidth = playerSpritesheet.spriteWidth;
	const int playerHeight = playerSpritesheet.spriteHeight;
	int playerState = 1;
	int playerDirection = 1;
	int playerX = (handler.screenWidth / 2) - (playerWidth / 2);
	int playerY = (handler.screenHeight / 2) - (playerHeight / 2);
	int consecutiveFramesHeld = 0;

	Image_Render player(&handler, playerWidth, playerHeight);
	Image_Render background(&handler, handler.screenWidth * 2, handler.screenHeight * 2);
	KeyboardHandler keyboardHandler(&handler);

	//fps regulators
	const int FPS = 120;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart{};
	int frameTime;

	bool gameRunning = true;

	while (gameRunning) {

		frameStart = SDL_GetTicks();

		if (SDL_PollEvent(&handler.event)) {
			//checking what event is occuring
			switch (handler.event.type) {
			case (SDL_QUIT):
				gameRunning = false;
				break;

			case (SDL_WINDOWEVENT):
				if (handler.event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					handler.ResizeWindow(handler.event.window.data1, handler.event.window.data2);
				}
			}
		}
		handler.ClearRenderer();

		//dealing with keyboard inputs
		keyboardHandler.UpdateKeyStates();

		if (keyboardHandler.AnyKeyPressed)
		{
			keyboardHandler.KeyPressed(playerDirection);
		}

		SDL_Rect backgroundRect = {0, 0, handler.screenWidth * 2, handler.screenHeight * 2};

		// rendering the background

		//checking to move either background or player
		keyboardHandler.CheckBackgroundLimits();

		//X axis movement
		if ((!(keyboardHandler.LeftLimitReached || keyboardHandler.RightLimitReached)) && (playerX == (handler.screenWidth / 2) - (playerWidth / 2)))
		{
			keyboardHandler.BackgroundX += keyboardHandler.BackgroundSpeedX;
		}
		else {
			playerX -= keyboardHandler.BackgroundSpeedX;
		}

		//recentering X axis if it gets off
		if ((keyboardHandler.BackgroundX < -(handler.screenWidth / 2)) && (playerX < (handler.screenWidth / 2) - (playerWidth / 2)))
		{
			playerX = (handler.screenWidth / 2) - (playerWidth / 2);
			std::cout << "recentered\n";
		}
		else if ((keyboardHandler.BackgroundX > -(handler.screenWidth / 2)) && (playerX > (handler.screenWidth / 2) - (playerWidth / 2))) {
			playerX = (handler.screenWidth / 2) - (playerWidth / 2);
			std::cout << "recentered\n";
		}

		//Y axis movement
		if ((!(keyboardHandler.TopLimitReached || keyboardHandler.BottomLimitReached)) && (playerY == (handler.screenHeight / 2) - (playerHeight / 2)))
		{
			keyboardHandler.BackgroundY += keyboardHandler.BackgroundSpeedY;
		}
		else {
			playerY -= keyboardHandler.BackgroundSpeedY;
		}

		//recentering Y axis if it gets off
		if ((keyboardHandler.BackgroundY < -(handler.screenHeight / 2)) && (playerY < (handler.screenHeight / 2) - (playerHeight / 2)))
		{
			playerY = (handler.screenHeight / 2) - (playerHeight / 2);
		}
		else if ((keyboardHandler.BackgroundY > -(handler.screenHeight / 2)) && (playerY > (handler.screenHeight / 2) - (playerHeight / 2))) {
			playerY = (handler.screenHeight / 2) - (playerHeight / 2);
		}

		background.render(Asset_Manager.Assets[1], backgroundRect, keyboardHandler.BackgroundX, keyboardHandler.BackgroundY);

		//dealing with the players animations
		if (keyboardHandler.BackgroundSpeedX != 0 || keyboardHandler.BackgroundSpeedY != 0) {
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

		//reseting the speed
		keyboardHandler.BackgroundSpeedX = 0;
		keyboardHandler.BackgroundSpeedY = 0;

		// rendering the player
		player.render(Asset_Manager.Assets[0], playerSpritesheet.getSprite(playerState, playerDirection), playerX, playerY);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

		//breaking out of the WaitEvent
		if (!gameRunning) {
			break;
		}
	}
	//destroying assets
	Asset_Manager.~Asset_Manager();

	return 0;
}