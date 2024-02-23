#include "Deck.h"

// Constructor
Deck::Deck()
{
	// Create a deck of 52 cards
	int i = 0;
	for (int s = 1; s < 5; s++)
	{
		for (int r = 1; r < 14; r++)
		{
			cards.push_back(Card(static_cast<Card::suit_t>(s), static_cast<Card::rank_t>(r)));
			i++;
		}
	}
}
// Shuffles the deck
void Deck::shuffle()
{
	std::shuffle(cards.begin(), cards.end(), gen);
}
void Deck::reshuffle()
{
	cards.insert(cards.end(), discardPile.begin(), discardPile.end()); 
	discardPile.clear(); 
	shuffle(); 
}
Card Deck::peekTop()
{
	return cards.front();
}
void Deck::replaceTop(Card c)
{
	Card topCard = cards.front(); 
	cards.erase(cards.begin()); 
	discardPile.push_back(topCard); 
	cards.insert(cards.begin(), c);
}
Card Deck::dealCard()
{
	if (cards.size() == 0)
	{
		reshuffle(); 
	}
	Card topCard = cards.front();
	cards.erase(cards.begin());
	discardPile.push_back(topCard); 
	return topCard;
}
int Deck::cardsLeft() const
{
	return cards.size();
}

void Deck::mergeDecks(Deck& d)
{
	cards.insert(cards.end(), d.cards.begin(), d.cards.end());
}