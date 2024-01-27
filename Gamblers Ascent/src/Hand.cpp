#include "Hand.h"


Hand::Hand() : isSoft(false), cards()
{

}

void Hand::addCard(Card c)
{
	cards.push_back(c);
}
int Hand::handValue()
{
	int result = 0;
	isSoft = false;
	int numAces = 0;

	for (const Card& card : cards) {
		int cardValue = static_cast<int>(card.rank);

		if (card.rank == Card::rank_t::ace) {
			numAces++;
		}
		else { result += cardValue; }
	}

	// Check for aces and adjust their values if necessary
	while (numAces > 0 && result + 10 <= 21) {
		result += 10;
		numAces--;
	}

	// Update "isSoft" status for the hand
	isSoft = (numAces > 0);

	return result;

}
void Hand::clearHand()
{
	cards.clear();
}

int Hand::cardsInHand()
{
	return cards.size();
}
bool Hand::isPair()
{
	return cards[0].get_value() == cards[1].get_value();
}
bool Hand::isBust()
{
	return (handValue() > 21);
}
bool Hand::isBlackjack()
{
	return (handValue() == 21 || (cards.size() >= 5 && handValue() < 22));
}
// turns isSoft true if the hand is a soft hand. A soft hand is a hand that contains an ace that is counted as 11.
void Hand::updateSoftStatus() {
	isSoft = false; // Reset "isSoft" to false.
	for (const Card& card : cards) {
		if (card.rank == Card::rank_t::ace && handValue() <= 11) {
			isSoft = true; // Set "isSoft" to true if there's an ace with a value of 11.
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& os, Hand& h)
{
	for (int i = 0; i < h.cards.size(); i++)
	{
		os << h.cards[i] << std::endl;
	}
	return os;

}
