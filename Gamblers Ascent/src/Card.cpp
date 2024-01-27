#include "Card.h"

auto Card::to_string(Card::suit_t s) -> std::string
{
	switch (s)
	{
	case suit_t::hearts:
		return "hearts";
	case suit_t::diamonds:
		return "diamonds";
	case suit_t::clubs:
		return "clubs";
	case suit_t::spades:
		return "spades";
	}
}

auto Card::get_value() -> unsigned {
	switch (rank)
	{
	case rank_t::ace:
		return 11;
	case rank_t::two:
		return 2;
	case rank_t::three:
		return 3;
	case rank_t::four:
		return 4;
	case rank_t::five:
		return 5;
	case rank_t::six:
		return 6;
	case rank_t::seven:
		return 7;
	case rank_t::eight:
		return 8;
	case rank_t::nine:
		return 9;
	case rank_t::ten:
		return 10;
	case rank_t::jack:
		return 10;
	case rank_t::queen:
		return 10;
	case rank_t::king:
		return 10;
	}
}


int operator+(const Card& c1, const Card& c2)
{
	return static_cast<int>(c1.rank) + static_cast<int>(c2.rank);
}

std::ostream& operator<<(std::ostream& os, Card& c)
{
	os << "The " << c.get_value() << " of " << c.to_string(c.suit) << '\n';
	return os;
}