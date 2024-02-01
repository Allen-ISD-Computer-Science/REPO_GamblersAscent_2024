#pragma once
#include<iostream>
#include <string>
#include <sstream>
#include<vector>
#include<random>
#include<memory>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "SDL_Handler.h"
#include "SDL_Image.h"
#include "Image_Render.h"
#include "MouseHandler.h"
#include <vector>
#include "KeyboardHandler.h"
#include "Asset_Manager.h"
#include "Spritesheet_Handler.h"

enum class Chip : int {
	Chip_1 = 1,
	Chip_2 = 2,
	Chip_5 = 5,
	Chip_10 = 10,
	Chip_25 = 25,
	Chip_50 = 50,
	Chip_100 = 100,
	Chip_200 = 200,
	Chip_500 = 500,
	Chip_1000 = 1000,
	// Add more chip values as needed
};
class Player {
public:
	Hand hand;
	unsigned int money;
	bool isDealer;
	Player() : money(500), isDealer(false), betAmount(0), currentChips({}) {};
	Player(bool isDealer, int initialMoney) : money(initialMoney), betAmount(0), isDealer(isDealer) {};
	Player(int initialMoney) : money(initialMoney), isDealer(false), betAmount(0){};
	// Bet placement
	unsigned int betAmount;
	std::vector<Chip> currentChips;
	// Add more player attributes as needed
};


enum class Action : char {
	Hit = 'H',
	Stand = 'S',
	Double = 'D',
	Split = 'C',
	Insurance = 'I',
	// Add more actions as needed
};

class Blackjack {
public:
	
	Blackjack(SDL_Handler* handler, KeyboardHandler* keyboardHandler, Asset_Manager* Asset_Manager, Spritesheet_Handler* cardSpritesheet, Spritesheet_Handler* chipSpritesheet);
	~Blackjack();
	Deck deck1, deck2;
	bool userTurn;

	bool isRunning;

	bool betPhase; // True if the player is placing a bet. Mouse inputs disabled during this phase besides the bet buttons
	int turnCount;
	int cardX;
	int botWait;
	// Rendering the cards
	const int cardSpacing = 8; // Adjust this value as needed
	std::vector<SDL_Rect> cardSrcRects;
	SDL_Rect cardDestRect;
	// Rendering the chips
	const int chipSpacing = 3; const int chipStartingX = 85; const int chipStartingY = 221; const int dealerChipStartingX = 460; const int dealerChipStartingY = 95;  // Adjust this value as needed
	int chipX, chipY;
	int dealerChipX, dealerChipY;
	SDL_Rect chipSrcRect;
	const Chip chipValues[10] = {
		Chip::Chip_1,
		Chip::Chip_2,
		Chip::Chip_5,
		Chip::Chip_10,
		Chip::Chip_25,
		Chip::Chip_50,
		Chip::Chip_100,
		Chip::Chip_200,
		Chip::Chip_500,
		Chip::Chip_1000
	
	};

	// Calculate the starting x position to center the cards
	const int startX = 202; // Adjust this value as needed
	const int cardY = 254; // Adjust this value as needed

	const int dealerStartX = 397; // Adjust this value as needed
	const int dealerCardY = 47; // Adjust this value as needed

	SDL_Handler* m_handler;
	KeyboardHandler* m_keyboardHandler;
	Asset_Manager* m_Asset_Manager;
	Spritesheet_Handler* m_cardSpritesheet;
	Spritesheet_Handler* m_chipSpritesheet;
	SDL_Rect cardSrcRect;
	Player player1, player2;
	Image_Render* blackjackScreen;
	Image_Render* cards;
	Image_Render* chips;
	Image_Render* icons;
	const SDL_Rect buttonRects[15] = {
		{ 372, 152, 41, 59 }, // Hit
		{ 112, 286, 40, 40 }, // Stand
		{ 164, 277, 21, 21},  // Double
		{ 164, 313, 21, 21 }, // Split
		{86, 222, 15, 15}, // Chip_1
		{106, 222, 15, 15},// Chip_2
		{126, 222, 15, 15},// Chip_5
		{146, 222, 15, 15},// Chip_10
		{166, 222, 15, 15},// Chip_25
		{86, 242, 15, 15}, // Chip_50
		{106, 242, 15, 15},// Chip_100
		 {126, 242, 15, 15}, // Chip_200
		{146, 242, 15, 15}, // Chip_500
		{166, 242, 15, 15}, // Chip_1000
		{ 311, 213, 15, 15 } // Revert bet
	};
	Uint32 frameStart{};
	int frameTime;
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;
public:
	void newGame();
	void newTurn();
	void gameLoop();
	void playerPhase(Player* player, Player* dealer);
	void dealerPhase(Player* player, Player* dealer);
	void betPhaseUpdate();
	void endTurn();
	void endEarly(Player* p);
	void endRound(Player* winner, Player* loser);
	void endGame();
	void render();
	void renderCardsNChips(Player* player, Player* dealer);
	void cardUpdate();
	void chipInput(Chip c, Player* p);
	void moneyToChip(Player* p) const;
	void mouseClick(SDL_Point pos);  
	void hit(Player* p);
	void doubleDown(Player* p);
	void split(Player* p);
	void turnCalculation(Player* player, Player* dealer);

	friend std::istream& operator>>(std::istream& is, Action& response);
	

};