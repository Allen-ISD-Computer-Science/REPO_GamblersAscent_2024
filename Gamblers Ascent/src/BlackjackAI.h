#pragma once
#include "SDL_Handler.h"


// Base class for the Blackjack AI, which will be used to determine the actions of the npc players.
class BlackjackAI
{
public:
	

private:
	// AI attributes. These will be used to determine the actions of the AI.

	int aggro; // How safe the bot is willing to hit as the dealer.
	int defense; // How safe the bot is willing to hit as the player.
	int risk; // how much risks an NPC is willing to take with their betting, as well as Doubles and Splits.
	int luck; // The odds of an enemy getting what they need for a good hand. This will be added in order to balance out the user having abilities while the NPC's do not.


public:
	BlackjackAI(int aggro, int defense, int risk, int luck);
	~BlackjackAI();

private:


};