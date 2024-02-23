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

	// Discard Pile
std::vector<Card> discardPile;

	// Shuffles the deck
	void shuffle();

	// Reshuffles the deck with discard pile
	void reshuffle();

	// Returns the top card of the deck
	Card peekTop();

	// Replaces the top card of the deck with a given card and discards the old top card
	void replaceTop(Card c);

	// Returns the number of cards left in the deck
	int cardsLeft() const;

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
