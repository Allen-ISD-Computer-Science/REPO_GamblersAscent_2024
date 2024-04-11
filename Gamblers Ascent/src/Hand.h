#pragma once
#include "Card.h"


class Hand {
public:
	// Creates an empty hand
	Hand();

	// Vector of cards
	std::vector<Card> cards;

	// True if the hand is a soft hand (contains an ace)
	bool isSoft;

	// True if the hand is a pair

	// Returns the number of cards in the hand
	int cardsInHand();

	// Returns the value of the hand
	int handValue();

	// Adds a card to the hand
	void addCard(Card c);

	// Prints the hand	
	friend std::ostream& operator<<(std::ostream& os, Hand& h);

	// Clears the hand
	void clearHand();

	// Returns true if the hand is a blackjack
	bool isBlackjack();

	// Returns true if the hand is a bust
	bool isBust();

	// Returns true if the hand is a pair
	bool isPair();

	// Updates the isSoft variable
	void updateSoftStatus();

};
