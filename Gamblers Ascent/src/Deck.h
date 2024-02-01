#pragma once
#include "Card.h"
#include<iostream>
#include<string>
#include<vector>
#include<random>

class Deck {

public:
	// Creates an unshuffled deck of 52 cards
	Deck();

	// Vector of cards
	std::vector<Card> cards;

	// Shuffles the deck
	void shuffle();

	// Returns the top card of the deck
	Card peekTop();

	// Returns the number of cards left in the deck
	int cardsLeft();

	// Returns the top card of the deck and removes it from the deck
	Card dealCard();

	// Merges two decks together
	void mergeDecks(Deck& d1);

private:
	// Random number generator
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<> dis{ 0, 51 };
};
