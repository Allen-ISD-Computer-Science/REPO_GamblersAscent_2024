#include "CollisionDetector.h"
#include <iostream>

CollisionDetector::CollisionDetector()
{
}
CollisionDetector::CollisionDetector(int defaultPlayerWidth, int defaultPlayerHeight) : defaultPlayerWidth(defaultPlayerWidth), defaultPlayerHeight(defaultPlayerHeight)
{
}
bool CollisionDetector::isColliding(SDL_Rect player, int floor) const
{
	player.x /= 2;
	player.y /= 2;
	// Iterates throught the array of floor boundaries to determine if the player has collided with the floor using SDL_HasIntersection.
	for (int i = 0; i < sizeof(floorBoundaries[floor]) / sizeof(floorBoundaries[floor][0]); i++)
	{
		if (SDL_HasIntersection(&player, &floorBoundaries[floor][i]))
		{
			return true;
		}
	}
	return false;
}

bool CollisionDetector::isColliding(int x, int y, int floor) const
{
	x /= 2;
	y /= 2; 
	// Iterates throught the array of floor boundaries to determine if the player has collided with the floor using SDL_HasIntersection.
	SDL_Rect player = { x, y, defaultPlayerWidth, defaultPlayerHeight };
	for (int i = 0; i < sizeof(floorBoundaries[floor]) / sizeof(floorBoundaries[floor][0]); i++)
	{
		if (SDL_HasIntersection(&player, &floorBoundaries[floor][i]))
		{
//			std::cout << "collision detected\n";
			return true;
		}
//		std::cout << "no collision detected, location: " << x << ", " << y << "\n";
	}
	return false;
}

bool CollisionDetector::isColliding(SDL_Rect player, SDL_Rect* intersection) const
{
	return false;
}

bool CollisionDetector::isColliding(SDL_Point* mouse) const
{
	return false;
}
