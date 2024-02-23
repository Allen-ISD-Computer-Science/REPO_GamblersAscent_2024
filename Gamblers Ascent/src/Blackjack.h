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
#include "BlackjackAI.h"
// Forward declaration of classes
class BlackjackAI;


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
	std::string name;
	Player() : money(500), isDealer(false), betAmount(0), currentChips({}), name("No name") {};
	Player(std::string name, bool isDealer, int initialMoney) : money(initialMoney), betAmount(0), isDealer(isDealer), name(name) {};
	Player(int initialMoney) : money(initialMoney), isDealer(false), betAmount(0){};
	// Bet placement
	unsigned int betAmount;
	std::vector<Chip> currentChips;
	// Add more player attributes as needed

};



class Blackjack {
public:

	Blackjack(SDL_Handler* handler, KeyboardHandler* keyboardHandler, Asset_Manager* Asset_Manager, Spritesheet_Handler* cardSpritesheet, Spritesheet_Handler* chipSpritesheet);
	~Blackjack();
// 	Deck playerDeck1, playerDeck2;
	Deck botDeck, botDeck2;
	Deck playerDeck, playerDeck2;
	bool userTurn;

	bool isRunning;

	bool betPhase; // True if the player is placing a bet. Mouse inputs disabled during this phase besides the bet buttons
	int turnCount;
	int cardX;
	int botWait;
	// Rendering the cards
	const int cardSpacing = 24; // Adjust this value as needed
	std::vector<SDL_Rect> cardSrcRects;
	SDL_Rect cardDestRect;
	// Rendering the chips
	const int chipSpacing = 9; const int chipStartingX = 255; const int chipStartingY = 663; const int dealerChipStartingX = 1380; const int dealerChipStartingY = 285;  // Adjust this value as needed
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
	const int startX = 606; // Adjust this value as needed
	const int cardY = 762; // Adjust this value as needed

	const int dealerStartX = 1191; // Adjust this value as needed
	const int dealerCardY = 141; // Adjust this value as needed


	// Tinman's AI
	BlackjackAI* botAI;
	SDL_Handler* m_handler;
	KeyboardHandler* m_keyboardHandler;
	Asset_Manager* m_Asset_Manager;
	Spritesheet_Handler* m_cardSpritesheet;
	Spritesheet_Handler* m_chipSpritesheet;
	SDL_Rect cardSrcRect;
	Player player, bot;
	Image_Render* blackjackScreen;
	Image_Render* cards;
	Image_Render* chips;
	Image_Render* icons;
	const SDL_Rect buttonRects[15] = {
		{ 1116, 456, 123, 177 }, // Hit
		{ 336, 858, 120, 120 }, // Stand
		{ 492, 831, 63, 63 },  // Double
		{ 492, 939, 63, 63 }, // Split
		{ 258, 666, 45, 45 }, // Chip_1
		{ 318, 666, 45, 45 },// Chip_2
		{ 378, 666, 45, 45 },// Chip_5
		{ 438, 666, 45, 45 },// Chip_10
		{ 498, 666, 45, 45 },// Chip_25
		{ 258, 726, 45, 45 }, // Chip_50
		{ 318, 726, 45, 45 },// Chip_100
		{ 378, 726, 45, 45 }, // Chip_200
		{ 438, 726, 45, 45 }, // Chip_500
		{ 498, 726, 45, 45 }, // Chip_1000
		{ 933, 639, 45, 45 } // Revert bet
	};
	Uint32 frameStart{};
	int frameTime;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
public:
	void newGame();
	void newTurn();
	void gameLoop();
	void playerPhase(Player* player, Player* dealer);
	void dealerPhase(Player* player, Player* dealer);
	void endTurn();
	void endRound(Player* winner, Player* loser);
	void endRound();
	void endGame();
	void render();
	void calculateCardPositions(Player* player, Player* dealer);
	void renderCardsNChips(Player* player, Player* dealer);
	void cardUpdate();
	void chipInput(Chip c, Player* p);
	void moneyToChip(Player* p) const;
	void mouseClick(SDL_Point pos);  
	void hit(Player* p);
	void doubleDown(Player* p);
	void split(Player* p);
	void turnCalculation(Player* player, Player* dealer);
	

};