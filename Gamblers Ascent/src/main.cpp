#include "SDL_Handler.h"
#include "Asset_Manager.h"
#include "Image_Render.h"
#include "Spritesheet_Handler.h"
#include "KeyboardHandler.h"
#include "CollisionDetector.h"
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

	//creating the player object
	const int playerWidth = playerSpritesheet.spriteWidth;
	const int playerHeight = playerSpritesheet.spriteHeight;
	int playerState = 1;
	int playerDirection = 1;
	float playerX = static_cast<float>(((handler.screenWidth) / 2) - (playerWidth / 2));
	float playerY = static_cast<float>(((handler.screenHeight) / 2) - (playerHeight / 2));
	int consecutiveFramesHeld = 0;

	Image_Render player(&handler, playerWidth, playerHeight);
	Image_Render background(&handler, 1280, 720);
	KeyboardHandler keyboardHandler(&handler);
	keyboardHandler.playerX = static_cast<float>(handler.screenWidth / 2) - (playerWidth / static_cast<float>(2));
	keyboardHandler.playerY = static_cast<float>(handler.screenHeight / 2) - (playerHeight / static_cast<float>(2));
	CollisionDetector collision(keyboardHandler);
	collision.centerX = 130;
	collision.centerY = 43;
	//fps regulators
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart{};
	int frameTime;

	bool gameRunning = true;

	while (gameRunning) {

		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&handler.event)) {
			//checking what event is occuring
			switch (handler.event.type) {
			case (SDL_QUIT):
				gameRunning = false;
				break;

			case (SDL_WINDOWEVENT):
				if (handler.event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					handler.ResizeWindow(handler.event.window.data1, handler.event.window.data2);
					keyboardHandler.screenCenterX = static_cast<float>(handler.screenWidth / 2) - (playerWidth / static_cast<float>(2));
					keyboardHandler.screenCenterY = static_cast<float>(handler.screenHeight / 2) - (playerHeight / static_cast<float>(2));

				}
			case (SDL_KEYDOWN):
				keyboardHandler.updateKeyboardState();
				break;
			case (SDL_KEYUP):
				keyboardHandler.updateKeyboardState();
				break;
			}
		}
		handler.ClearRenderer();

		SDL_Rect backgroundRect = { 0, 0, 1280, 720 };

		//updating the background
		keyboardHandler.updateBackground(playerDirection);
		
		if (keyboardHandler.backgroundSpeedX != 0 || keyboardHandler.backgroundSpeedY != 0) {
			consecutiveFramesHeld++;
		}
		else {
			consecutiveFramesHeld = 0;
		}

		//rendering the player
		if (((consecutiveFramesHeld != 0) && ((consecutiveFramesHeld % 9) == 0)) || (consecutiveFramesHeld == 3)) {
			if (playerState < 4) {
				playerState++;
			}
			else {
				playerState = 1;
			}
		}
		
		// clamping the background position
		keyboardHandler.ClampBackgroundPosition();

		// rendering the background
		background.render(Asset_Manager.Assets[1], backgroundRect, keyboardHandler.backgroundX, keyboardHandler.backgroundY);

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