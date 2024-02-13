#include "SDL_Handler.h"
#include "Asset_Manager.h"
#include "Image_Render.h"
#include "Spritesheet_Handler.h"
#include "KeyboardHandler.h"
#include <iostream>
#include "Blackjack.h"
#include "MouseHandler.h"
#include "CollisionDetector.h"

int main(int argc, char* argv[])
{
	// creating a handler to manage all user input in the program
	SDL_Handler handler(640, 360); //window width, window height

	// base path of the program
	std::string basePath = (std::string)SDL_GetBasePath();


	//filePath Array
	static const int AssetCount = 7;  //amount of assets
	std::string filePaths[AssetCount] = {
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\playerSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\finalfloor00.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\Blackjack_Screen.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\cardSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\chipSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\DealerTurn.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\PlayerTurn.png"

	};

	// asset manager
	Asset_Manager Asset_Manager(filePaths, handler.renderer);
	Asset_Manager.LoadAssets();

	// Spritesheet Handler
	Spritesheet_Handler playerSpritesheet(256, 512, 4, 8); //width, height, rows, columns
	Spritesheet_Handler cardSpritesheet(164, 826, 4, 14);
	Spritesheet_Handler chipSpritesheet(170, 17, 10, 1);

	// Screen variables
	static int ScreenWidth = handler.screenWidth;
	static int ScreenHeight = handler.screenHeight;

	// player variables
	const static int playerWidth = playerSpritesheet.spriteWidth;
	const static int playerHeight = playerSpritesheet.spriteHeight;
	int playerState = 1;
	int playerDirection = 1;

	// different coordinates of the player
	struct {
		SDL_Point ScreenCoordinates = { 
			(ScreenWidth / 2) - (playerWidth / 2), 
			(ScreenWidth / 2) - (playerHeight / 2)
		};
		SDL_Point TrueCoordinates = { 820, 15 };

	} PlayerCoordinates;

	// coordinates of pot guy to test npc interactions
	SDL_Point PotGuyCoordinates = {500, 500};


	// Collision variables
	CollisionDetector collisionDetector(playerWidth, playerHeight);

	int consecutiveFramesHeld = 0;

	Image_Render player(&handler, playerWidth, playerHeight);
	Image_Render background(&handler, ScreenWidth * 2, ScreenHeight * 2);
	KeyboardHandler keyboardHandler(&handler);

	// blackjack variables
	Image_Render icons(&handler, 1280, 720);
	Blackjack blackjack(&handler, &keyboardHandler, &Asset_Manager, &cardSpritesheet, &chipSpritesheet);

	// fps regulators
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart{};
	int frameTime;

	bool gameRunning = true;

	while (gameRunning) {

		frameStart = SDL_GetTicks();

		if (SDL_PollEvent(&handler.event)) {
			// checking what event is occuring
			switch (handler.event.type) {
			case (SDL_QUIT):
				gameRunning = false;
				goto quit;
				break;
			case (SDL_WINDOWEVENT):
				if (handler.event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					handler.ResizeWindow(handler.event.window.data1, handler.event.window.data2);
				}
			}
		}


		//updating the background

		handler.ClearRenderer();

		// dealing with keyboard inputs
		keyboardHandler.UpdateKeyStates();

		if (keyboardHandler.E_key)
		{
			if (PotGuyCoordinates.x - 50 < PlayerCoordinates.TrueCoordinates.x &&
				PotGuyCoordinates.x + 50 > PlayerCoordinates.TrueCoordinates.x &&
				PotGuyCoordinates.y - 50 < PlayerCoordinates.TrueCoordinates.y &&
				PotGuyCoordinates.y + 50 > PlayerCoordinates.TrueCoordinates.y)
			{
				std::cout << "npc interacted with, blackjack game started.\n";
				blackjack.newGame();
				blackjack.isRunning = true;
				blackjack.gameLoop();
				
			}
		}

		if (keyboardHandler.DirectionalKeyPressed)
		{
			keyboardHandler.DirectionalKey(playerDirection);
		}

		// rendering the background
		SDL_Rect backgroundRect = { 0, 0, ScreenWidth * 2, ScreenHeight * 2 };

		// checking to move either background or player
		keyboardHandler.CheckBackgroundLimits();

		// dealing with collision checks
		if (collisionDetector.isColliding(PlayerCoordinates.TrueCoordinates.x, PlayerCoordinates.TrueCoordinates.y, 0))
		{
			keyboardHandler.BackgroundSpeedX = 0;
			keyboardHandler.BackgroundSpeedY = 0;
		}
		// X axis movement
		keyboardHandler.MoveBackgroundX(PlayerCoordinates.ScreenCoordinates.x, PlayerCoordinates.TrueCoordinates.x, ScreenWidth, playerWidth);

		// Y axis movement
		keyboardHandler.MoveBackgroundY(PlayerCoordinates.ScreenCoordinates.y, PlayerCoordinates.TrueCoordinates.y, ScreenHeight, playerHeight);


		background.render(Asset_Manager.Assets[1], backgroundRect, keyboardHandler.BackgroundX, keyboardHandler.BackgroundY);

		// true coordinate debugging
		//std::cout << "(" << PlayerCoordinates.TrueX << "," << PlayerCoordinates.TrueY << ")\n";

		// dealing with the players animations
		keyboardHandler.UpdatePlayerAnimation(consecutiveFramesHeld, playerState);
		keyboardHandler.HandleEdgeCaseDirections(playerDirection);

		// reseting the speed
		keyboardHandler.BackgroundSpeedX = 0;
		keyboardHandler.BackgroundSpeedY = 0;

		// rendering the player
		player.render(Asset_Manager.Assets[0], playerSpritesheet.getSprite(playerState, playerDirection), PlayerCoordinates.ScreenCoordinates.x, PlayerCoordinates.ScreenCoordinates.y);


		// reseting the speed
		keyboardHandler.BackgroundSpeedX = 0;
		keyboardHandler.BackgroundSpeedY = 0;

		// rendering the player
		player.render(Asset_Manager.Assets[0], playerSpritesheet.getSprite(playerState, playerDirection), PlayerCoordinates.ScreenCoordinates.x, PlayerCoordinates.ScreenCoordinates.y);

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}

		// breaking out of the WaitEvent
		if (!gameRunning) {
			break;
		}


		//updating the renderer
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }
		//breaking out of the WaitEvent
		if (!gameRunning) { break; }

	}
	// destroying assets
	Asset_Manager.~Asset_Manager();
	return 0;
quit:
	Asset_Manager.~Asset_Manager();
	return 0;

}



