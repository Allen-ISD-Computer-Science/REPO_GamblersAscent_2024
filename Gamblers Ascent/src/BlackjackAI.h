#pragma once
#include "SDL_Handler.h"
#include "Blackjack.h"
#include <random>

// Forward declaration of Blackjack
class Blackjack;

enum Action : unsigned char {
	Hit = 'H',
	Stand = 'S',
	Double = 'D',
	Split = 'C',
	Insurance = 'I',
	// Add more actions as needed
};

enum Ability : unsigned char {
	RoyaleExecution,
	RemnantFlame,
	AbsoluteZero,
	None
	// Add more abilities as needed
};
// Base class for the Blackjack AI, which will be used to determine the actions of the npc players.
class BlackjackAI 
{
public:
	

private:
	// AI attributes. These will be used to determine the actions of the AI.

	const unsigned int aggro; // How safe the bot is willing to hit as the dealer.
	const unsigned int defense; // How safe the bot is willing to hit as the player.
	const unsigned int risk; // how much risks an NPC is willing to take with their betting, as well as Doubles and Splits.
	const unsigned int luck; // The odds of an enemy getting what they need for a good hand. This will be added in order to balance out the user having abilities while the NPC's do not.
	Ability ability; // Some AI's will have abilities that will influence their actions.

	// Formula Variables
	const int baseValue = 9;
	int cardInfluence; // The influence of the dealer's visible card.
	int handInfluence; // The influence of the player's hand.

	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937* gen; // Seed the generator.
	std::uniform_real_distribution<>* distr; // Define the range of the random number.
	std::uniform_int_distribution<>* luckdistr; // Define the range of the luck random number.

	std::random_device luckrd; // Obtain a random number from hardware for luck.
	int rngValue; // Random number generated from the random device.
	int luckRNG; // Random number generated from the random device for luck.

	int maxHand; // The maximum hand values that the AI is willing to hit on. 
public:
	BlackjackAI(int aggro, int defense, int risk, int luck);
	BlackjackAI(int aggro, int defense, int risk, int luck, const Ability ability);
	~BlackjackAI();

	// AI functions. These will be used to determine the actions of the AI.
	Action determineAction(Hand hand, Hand dealerHand);
	Action determineAction(Hand hand, Card dealerCard);
	unsigned int determineBet(unsigned int money) const;
	bool determineLuck() const; 
	
	// Add more AI functions as needed
private:


};
