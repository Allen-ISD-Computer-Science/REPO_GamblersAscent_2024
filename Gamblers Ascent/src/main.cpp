#include "SDL_Handler.h"
#include "Asset_Manager.h"
#include "Player.h"
#include "Spritesheet_Handler.h"
#include <iostream>

int main(int argc, char* argv[])
{
	//creating a handler to manage all user input in the program
	SDL_Handler handler(960, 720); //window width, window height

	//base path of the program
	std::string basePath = (std::string)SDL_GetBasePath();

	//filePath Array
	static const int AssetCount = 1;
	std::string filePaths[AssetCount] = {
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\playerSpriteSheet.png",};

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
	int consecutiveFramesHeld = 0;
	Player player(&handler, playerWidth, playerHeight);

	//fps regulators
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	bool gameRunning = true;

	while (gameRunning) {
		while (SDL_PollEvent(&handler.event)) {
			//frame tracking
			frameStart = SDL_GetTicks();

			//clearing the window
			handler.ClearRenderer();

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

			//checking the keyboards state
			if (handler.keyPress(playerDirection)) {
				consecutiveFramesHeld++;
			} else {
				consecutiveFramesHeld = 0;
				playerState = 1;
			}

			//rendering the player
			if (((consecutiveFramesHeld != 0) && ((consecutiveFramesHeld % 9) == 0)) || (consecutiveFramesHeld == 3)) {
				if (playerState < 4) {
					playerState++;
				} else {
					playerState = 1;
				}
			}
			player.render(Asset_Manager.Assets[0], playerSpritesheet.getSprite(playerState, playerDirection));

			//breaking out of the WaitEvent
			if (!gameRunning) {
				break;
			}

			//frame tracking
			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime) {
				SDL_Delay(frameDelay - frameTime);
			}
		}
	}
	//destroying assets
	Asset_Manager.~Asset_Manager();

	return 0;
}