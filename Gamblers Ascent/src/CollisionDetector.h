#pragma once
#include "SDL_Handler.h"


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
	bool isColliding(SDL_Point* mouse) const;
private:
	int defaultPlayerWidth;
	int defaultPlayerHeight;

	const SDL_Rect floorBoundaries[15][5] = {
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
	},
	{
		{},
		{},
		{},
		{}
	}
	};
};