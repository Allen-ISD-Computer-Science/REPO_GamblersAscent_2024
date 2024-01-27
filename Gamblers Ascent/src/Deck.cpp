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
			std::cout << "added " << i << " cards \n";
		}
	}
}
// Shuffles the deck
void Deck::shuffle()
{
	std::shuffle(cards.begin(), cards.end(), gen);
}
Card Deck::peekTop()
{
	return cards.front();
}
Card Deck::dealCard()
{
	Card topCard = cards.front();
	cards.erase(cards.begin());
	return topCard;
}
int Deck::cardsLeft()
{
	return cards.size();
}

void Deck::mergeDecks(Deck& d)
{
	cards.insert(cards.end(), d.cards.begin(), d.cards.end());
}