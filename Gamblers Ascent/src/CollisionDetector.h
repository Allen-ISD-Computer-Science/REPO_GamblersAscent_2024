#pragma once
#include "SDL_Handler.h"
#include <vector>


class CollisionDetector
{
public:
	CollisionDetector();
	CollisionDetector(int defaultPlayerWidth, int defaultPlayerHeight);
	// This function will be used to determine if the player has collided with an object.
	bool isColliding(SDL_Rect player, int floor) const;
	// This function will be used to determine if the player has collided with an object.
	bool isColliding(int x, int y, int floor) const;
	// This function will be used to determine if the player has collided with an object, and map the intersection.
	bool isColliding(SDL_Rect player, SDL_Rect* intersection) const;
	// This function will be used to determine if the mouse is located within the bounds of a rectangle.
	bool isColliding(SDL_Point* mouse, int location) const;
private:
	int defaultPlayerWidth;
	int defaultPlayerHeight;
	// The boundaries of the floor. These will be used to determine if the player has collided with the floor. The floor count is self explainatory.
	const std::vector<SDL_Rect> floorBoundaries[5] = {
	{
		{0, 0, 1, 360},
		{0, 359, 640, 1},
		{639, 0, 1, 360},
		{0, 0, 640, 1}
	},
	{
		{},
		{},
		{},
		{}
	},
	{
		{},
		{},
		{},
		{}
	},
	{
		{},
		{},
		{},
		{}
	},
	{
		{},
		{},
		{},
		{}
	}
	};
	// The boundaries of the buttons.
	const SDL_Rect startScreenButtons[5] = 
	{

	};
	const SDL_Rect pauseScreenButtons[3] = 
	{

	};// Pre scale: 640x360
    const SDL_Rect blackjackScreenButtons[15] = {
        { 1116, 456, 123, 177 }, // Hit
        { 336, 858, 120, 120 }, // Stand
        { 492, 831, 63, 63 },  // Double
        { 492, 939, 63, 63 }, // Split
        { 258, 666, 45, 45 }, // Chip_1
        { 318, 666, 45, 45 },// Chip_2
        { 378, 666, 45, 45 },// Chip_5
        { 438, 666, 45, 45 },// Chip_10
        { 498, 666, 45, 45 },// Chip_25
        { 258, 726, 45, 45 }, // Chip_50
        { 318, 726, 45, 45 },// Chip_100
        { 378, 726, 45, 45 }, // Chip_200
        { 438, 726, 45, 45 }, // Chip_500
        { 498, 726, 45, 45 }, // Chip_1000
        { 933, 639, 45, 45 } // Revert bet
    };
};    

// Old: 640x360
// New: 1920x1080
// 3x scale
