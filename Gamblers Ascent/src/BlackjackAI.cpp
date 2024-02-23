#include "BlackjackAI.h"



BlackjackAI::BlackjackAI(int aggro, int defense, int risk, int luck)
	: aggro(aggro), defense(defense), risk(risk), luck(luck), ability(Ability::None), 
	maxHand(0), cardInfluence(0), handInfluence(0)
{
	gen = new std::mt19937(rd()); // Seed the generator 

	// Define the range
	distr = new std::uniform_real_distribution<>(-1, 1);

	// Generate a random number
	rngValue = distr->operator()(*gen);

	// Generate the luck value
	luckdistr = new std::uniform_int_distribution<>(0, 99);
	luckRNG = luckdistr->operator()(*gen);

}

BlackjackAI::BlackjackAI(int aggro, int defense, int risk, int luck, const Ability ability)
	: aggro(aggro), defense(defense), risk(risk), luck(luck), ability(ability),
	maxHand(0), cardInfluence(0), handInfluence(0)
{
	gen = new std::mt19937(rd()); // Seed the generator 

	// Define the range
	distr = new std::uniform_real_distribution<>(-1, 1);

	// Generate a random number
	rngValue = distr->operator()(*gen);

	// Generate the luck value
	luckdistr = new std::uniform_int_distribution<>(1, 100);
	luckRNG = luckdistr->operator()(*gen);
}

BlackjackAI::~BlackjackAI()
{
}
// Function to determine the action of the AI when they are the dealer
Action BlackjackAI::determineAction(Hand hand, Hand playerHand)
{
	// Divides the hand value by 21 and multiplies by the defense value
	// If the result is greater than 1, the AI will stand
	// If the result is less than 1, the AI will hit

	handInfluence = (playerHand.handValue() > 17) ? 1 : -1;
	maxHand = baseValue + defense + handInfluence + rngValue;

	// Preconditions
	if (hand.handValue() > playerHand.handValue() || hand.handValue() >= 21) { return Action::Stand; }
	/* To be added when splitting is implemented
	 if (playerHand.isSplit) {
		if (hand.handValue() >= maxHand)
		{
			return Action::Stand;
		}
		else 
		{
			return Action::Hit;
		}
	 }
	 */
	// Since there isn't a reason for the dealer not not hit, the dealer will always hit unless the player splits
	if (hand.handValue() == playerHand.handValue() && defense >= 5)
	{
		return Action::Stand;
	}
	else
	{
		return Action::Hit;
	}

}
// Function to determine the action of the AI when they are the player
Action BlackjackAI::determineAction(Hand hand, Card dealerCard)
{
	cardInfluence = (dealerCard.get_value() > 5) ? 1 : -1 ;
	maxHand = baseValue + aggro + cardInfluence + rngValue;

	// Preconditions
	if (hand.handValue() >= 21) { return Action::Stand; }
	if (hand.handValue() < dealerCard.get_value() ) { return Action::Hit; }

	// Double & Split Calculation
	if (hand.cardsInHand() == 2)
	{
		// Splitting
		/* if (hand.cards[0].get_value() == hand.cards[1].get_value())
		{

			 return Action::Split;
		}
		*/
		// Doubling
		if (hand.handValue() == 11)
		{
			return Action::Double;
		}
		// Soft Hand 
		else if (hand.isSoft && hand.handValue() >= 16 && hand.handValue() <= 18)
		{
			return Action::Double;
		}

	} 

	// If the hand value is greater than the maxHand, the AI will stand
	if (hand.handValue() >= maxHand)
	{
		return Action::Stand;
	}
	else 
	{
		return Action::Hit;
	}

	return Action::Stand;
}

unsigned int BlackjackAI::determineBet(unsigned int money) const
{
	// Bet is determined by the risk value
	// The AI will bet a percentage of half thier money based on the risk value
	return ((money / static_cast<double>(2)) * (static_cast<double>(risk) / 10.0));
}

bool BlackjackAI::determineLuck() const
{

	// The luck attribute represents the % chance of the ai getting +-1 to the card value needed for a blackjack. If hand value is too small, then the ai will draw as normal
	if (luckRNG <= luck * 10)
	{
		std::cout << "The AI has been lucky!" << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}
