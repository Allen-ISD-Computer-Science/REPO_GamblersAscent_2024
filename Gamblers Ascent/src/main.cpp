#include "SDL_Handler.h"
#include "Asset_Manager.h"
#include "Image_Render.h"
#include "Spritesheet_Handler.h"
#include "KeyboardHandler.h"
#include <iostream>
#include "Blackjack.h"
#include "MouseHandler.h"
#include "Text_Renderer.h"
#include "CollisionDetector.h"

int main(int argc, char* argv[])
{
	// creating a handler to manage all user input in the program
	SDL_Handler handler(1920, 1080); //window width, window height

	// base path of the program
	std::string basePath = (std::string)SDL_GetBasePath();


	//filePath Array
	static const int AssetCount = 11;  //amount of assets
	std::string filePaths[AssetCount] = {
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\playerSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\floor 001.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\Blackjack_Screen.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\cardSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\chipSpriteSheet.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\DealerTurn.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\PlayerTurn.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\elevatorUI.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\floor 001.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\dialogueboxbottom.png",
		basePath + "..\\..\\..\\Gamblers Ascent\\res\\dialogueboxtop.png"
	};
	// asset manager
	Asset_Manager Asset_Manager(filePaths, handler.renderer);
	Asset_Manager.LoadAssets();

	//text renderer
	TextRenderer textRenderer(&handler, basePath + "..\\..\\..\\Gamblers Ascent\\res\\fonts\\04B_30__.ttf", 18);

	//floors 
	SDL_Texture* floorTextures[2] = {
		Asset_Manager.Assets[1], //lobby
		Asset_Manager.Assets[8] //floor 1
	};
	SDL_Texture* activeFloor = floorTextures[0];

	// Spritesheet Handler
	Spritesheet_Handler playerSpritesheet(256, 512, 4, 8); //width, height, rows, columns
	Spritesheet_Handler cardSpritesheet(164, 826, 4, 14);
	Spritesheet_Handler chipSpritesheet(170, 17, 10, 1);

	// Screen variables
	static int ScreenWidth = handler.screenWidth;
	static int ScreenHeight = handler.screenHeight;

	// Background variables
	SDL_Rect backgroundRect = { 0, 0, ScreenWidth * 2, ScreenHeight * 2 };
	SDL_Rect startScreenRect = { 0, 0, ScreenWidth, ScreenHeight };

	// player variables
	const static int playerWidth = playerSpritesheet.spriteWidth * 3;
	const static int playerHeight = playerSpritesheet.spriteHeight * 3;
	int playerState = 1;
	int playerDirection = 1;

	// different coordinates of the player
	struct {
		SDL_Point ScreenCoordinates = { 
			ScreenWidth / 2, 
			ScreenHeight / 2
		};
		SDL_Point TrueCoordinates = { 1440, 810}; // Starting coordinates of the player
		// 960, 540
	} PlayerCoordinates;

	// coordinates of pot guy to test npc interactions
	SDL_Point PotGuyCoordinates = {1336, 308};

	// Collision variables
	CollisionDetector collisionDetector(playerWidth, playerHeight);
	
	// coordinates of the elevator
	SDL_Point elevatorCoordinates = { 840, 415 };

	// coordinates of blanket head to use the dialogue box
	SDL_Point blanketHeadCoordinates = { 430, 450 };
	//elevator related variables
	bool elevatorScreenActive = false;
	SDL_Rect elevatorUIRect = {230, 5, 185, 350};
	SDL_Rect elevatorUIButtonsRects[6]{
		{310, 305, 70, 25}, // lobby
		{310, 250, 70, 25}, // floor 1
		{310, 195, 70, 25}, // floor 2
		{310, 140, 70, 25}, // floor 3
		{310, 85, 70, 25}, // floor 4
		{310, 30, 70, 25} // floor 5
	};
	int consecutiveFramesHeld = 0;

	bool dialogueBoxAnimationStarted = false;
	bool dialogueBoxAnimationComplete = false;
	int dialogueBoxAnimationFrame = 0;
	Image_Render player(&handler, playerWidth, playerHeight);
	Image_Render background(&handler, ScreenWidth * 2, ScreenHeight * 2);
	Image_Render elevatorUI(&handler, ScreenWidth, ScreenHeight);
	Image_Render dialogueboxtop(&handler, ScreenWidth, ScreenHeight);
	Image_Render dialogueboxbottom(&handler, ScreenWidth, ScreenHeight);

	Image_Render homeScreen(&handler, ScreenWidth, ScreenHeight);

	// keyboard handler
	KeyboardHandler keyboardHandler(&handler);
	MouseHandler mouseHandler(&handler);
	bool leftMouseClicked = false;
	SDL_Point mousePosition;

	// blackjack variables
	Image_Render icons(&handler, 1920, 1080);
	Blackjack blackjack(&handler, &keyboardHandler, &Asset_Manager, &cardSpritesheet, &chipSpritesheet);

	// fps regulators
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart{};
	int frameTime;

	bool gameRunning = true;

	// Start Menu Game Loop
	while (false) {
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
			case (SDL_MOUSEBUTTONDOWN):
				if (handler.event.button.button == SDL_BUTTON_LEFT)
				{
					leftMouseClicked = true;
				}
			}
		}

		//updating the background
		handler.ClearRenderer();
		homeScreen.render(Asset_Manager.Assets[7], startScreenRect, 0, 0);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }
		//breaking out of the WaitEvent
		if (!gameRunning) { break; }
	}


	// Main Game Loop
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

		// dealing with keyboard inputs
		keyboardHandler.UpdateKeyStates();

		if (keyboardHandler.E_key)
		{
			if (PotGuyCoordinates.x - 150 < PlayerCoordinates.TrueCoordinates.x &&
				PotGuyCoordinates.x + 150 > PlayerCoordinates.TrueCoordinates.x &&
				PotGuyCoordinates.y - 150 < PlayerCoordinates.TrueCoordinates.y &&
				PotGuyCoordinates.y + 150 > PlayerCoordinates.TrueCoordinates.y)
			{
				std::cout << "npc interacted with, blackjack game started.\n";
				blackjack.newGame();
				blackjack.isRunning = true;
				blackjack.gameLoop();
				
			}
			if (elevatorCoordinates.x - 50 < PlayerCoordinates.TrueX &&
				elevatorCoordinates.x + 50 > PlayerCoordinates.TrueX &&
				elevatorCoordinates.y - 50 < PlayerCoordinates.TrueY &&
				elevatorCoordinates.y + 50 > PlayerCoordinates.TrueY)
			{
				elevatorScreenActive = true;
			}
			if (blanketHeadCoordinates.x - 50 < PlayerCoordinates.TrueX &&
				blanketHeadCoordinates.x + 50 > PlayerCoordinates.TrueX &&
				blanketHeadCoordinates.y - 50 < PlayerCoordinates.TrueY &&
				blanketHeadCoordinates.y + 50 > PlayerCoordinates.TrueY)
			{
				std::cout << "yay dialogue box\n";
				dialogueBoxAnimationStarted = true;
			}
		}

		if (keyboardHandler.DirectionalKeyPressed)
		{
			keyboardHandler.DirectionalKey(playerDirection);
		}



		// checking to move either background or player
		keyboardHandler.CheckBackgroundLimits();

		
		// X axis movement
		
		keyboardHandler.MoveBackgroundX(PlayerCoordinates.ScreenCoordinates.x, PlayerCoordinates.TrueCoordinates.x, ScreenWidth, playerWidth);

		// Y axis movement
		keyboardHandler.MoveBackgroundY(PlayerCoordinates.ScreenCoordinates.y, PlayerCoordinates.TrueCoordinates.y, ScreenHeight, playerHeight);
		
		// dealing with collision checks
		if (collisionDetector.isColliding(PlayerCoordinates.TrueCoordinates.x, PlayerCoordinates.TrueCoordinates.y, 0))
		{
		//	PlayerCoordinates.TrueCoordinates.x -= keyboardHandler.BackgroundSpeedX;
		//	PlayerCoordinates.TrueCoordinates.y -= keyboardHandler.BackgroundSpeedY;
		}
		// dealing with the players animations
		keyboardHandler.UpdatePlayerAnimation(consecutiveFramesHeld, playerState);
		keyboardHandler.HandleEdgeCaseDirections(playerDirection);

		// reseting the speed
		keyboardHandler.BackgroundSpeedX = 0;
		keyboardHandler.BackgroundSpeedY = 0;


		handler.ClearRenderer();

		background.render(Asset_Manager.Assets[1], backgroundRect, keyboardHandler.BackgroundX, keyboardHandler.BackgroundY);


		// rendering the player
		player.render(Asset_Manager.Assets[0], playerSpritesheet.getSprite(playerState, playerDirection), PlayerCoordinates.ScreenCoordinates.x, PlayerCoordinates.ScreenCoordinates.y);


		// reseting the speed
		keyboardHandler.BackgroundSpeedX = 0;
		keyboardHandler.BackgroundSpeedY = 0;

		

		if (elevatorScreenActive)
		{
			if (!(ElevatorCoordinates.x - 150 < PlayerCoordinates.TrueX &&
				ElevatorCoordinates.x + 150 > PlayerCoordinates.TrueX &&
				ElevatorCoordinates.y - 150 < PlayerCoordinates.TrueY &&
				ElevatorCoordinates.y + 150 > PlayerCoordinates.TrueY))
			{
				elevatorScreenActive = false;
			}
			elevatorUI.render(Asset_Manager.Assets[7], backgroundRect, 0, 0);
			if (leftMouseClicked)
			{
				mousePosition = mouseHandler.getPosition();
				if (!SDL_PointInRect(&mousePosition, &elevatorUIRect))
				{
					elevatorScreenActive = false;
				}
				else if (SDL_PointInRect(&mousePosition, &elevatorUIButtonsRects[0]))
				{
					activeFloor = floorTextures[0];
					elevatorScreenActive = false;
				}
				else if (SDL_PointInRect(&mousePosition, &elevatorUIButtonsRects[1]))
				{
					activeFloor = floorTextures[1];
					elevatorScreenActive = false;
				}
				std::cout << "Mouse position " << mousePosition.x << "," << mousePosition.y << "\n";
				leftMouseClicked = false;
			}
		}
		if (dialogueBoxAnimationStarted)
		{
			if (!(BlanketHeadCoordinates.x - 150 < PlayerCoordinates.TrueX &&
				BlanketHeadCoordinates.x + 150 > PlayerCoordinates.TrueX &&
				BlanketHeadCoordinates.y - 150 < PlayerCoordinates.TrueY &&
				BlanketHeadCoordinates.y + 150 > PlayerCoordinates.TrueY))
			{
				dialogueBoxAnimationStarted = false;
			}
			if (dialogueBoxAnimationFrame <= 5)
			{
				dialogueboxtop.render(Asset_Manager.Assets[10], { 0, 0, ScreenWidth, ScreenHeight }, 0, -90 + (dialogueBoxAnimationFrame * 6));
				dialogueboxbottom.render(Asset_Manager.Assets[9], { 0, 0, ScreenWidth, ScreenHeight }, 0, 375 - (dialogueBoxAnimationFrame * 25));
				dialogueBoxAnimationFrame++;
			}
			else {
				dialogueBoxAnimationStarted = false;
				dialogueBoxAnimationComplete = true;
				dialogueBoxAnimationFrame = 0;
			}
		}
		else if (dialogueBoxAnimationComplete)
		{
			if (!(BlanketHeadCoordinates.x - 150 < PlayerCoordinates.TrueX &&
				BlanketHeadCoordinates.x + 150 > PlayerCoordinates.TrueX &&
				BlanketHeadCoordinates.y - 150 < PlayerCoordinates.TrueY &&
				BlanketHeadCoordinates.y + 150 > PlayerCoordinates.TrueY))
			{
				dialogueBoxAnimationComplete = false;
			}
			dialogueboxtop.render(Asset_Manager.Assets[10], { 0, 0, ScreenWidth, ScreenHeight }, 0, 0);
			dialogueboxbottom.render(Asset_Manager.Assets[9], { 0, 0, ScreenWidth, ScreenHeight }, 0, 0);
			textRenderer.renderString("I'm nahom tadele and I got hoes", 195, 900, { 0,0,0 });
		}
		// updating the renderer
		SDL_RenderPresent(handler.renderer);

		SDL_UpdateWindowSurface(handler.m_window);


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



