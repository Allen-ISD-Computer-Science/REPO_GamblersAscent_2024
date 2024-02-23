#pragma once
#include<iostream>
#include<vector>
#include<random>

class Card {
public:
	enum class suit_t : unsigned char
	{
		spades = 1,
		clubs = 2,
		diamonds = 3,
		hearts = 4
	};
	enum class rank_t : unsigned char
	{
		ace = 1,
		two = 2,
		three = 3,
		four = 4,
		five = 5,
		six = 6,
		seven = 7,
		eight = 8,
		nine = 9,
		ten = 10,
		jack = 11,
		queen = 12,
		king = 13
	};
	suit_t suit;
	rank_t rank;

	// Constructor 
	Card(suit_t s, rank_t r) : suit(s), rank(r) {}

	// Methods
	auto get_value() -> unsigned;
	auto to_string(suit_t s) -> std::string;
	
	// Operator Functions
	bool operator<(const Card& c) const
	{
		return rank < c.rank;
	}
	friend int operator+(const Card& c1, const Card& c2);
	// Overloaded Operators
	friend std::ostream& operator<<(std::ostream& os, Card& c);
};