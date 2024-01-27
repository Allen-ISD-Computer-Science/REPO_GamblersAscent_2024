#include "CollisionDetector.h"

CollisionDetector::CollisionDetector(KeyboardHandler keyH)
	: centerX(0.0f), centerY(0.0f), keyH(keyH)
{
}

CollisionDetector::~CollisionDetector()
{
}

bool CollisionDetector::checkCollision(SDL_Rect* rect1, int section)
{
	// check if the player is colliding with any of the SDL_Rects in the vector matrix based on one of the 4 sections of the map the player is in
	for (int i = 0; i < 4; i++) {
		if (SDL_IntersectRectAndLine(rect1, &objectBoundaries[section][i].x1, &objectBoundaries[section][i].y1, &objectBoundaries[section][i].x2, &objectBoundaries[section][i].y2)) {
			return true;
		}
	}
	return false;
}

int CollisionDetector::getSection() const
{
	// Function that defines the section of the map the player is located in
	if (keyH.playerX < centerX && keyH.playerY < centerY) {
		return 1;
	}
	else if (keyH.playerX >= centerX && keyH.playerY < centerY) {
		return 2;
	}
	else if (keyH.playerX < centerX && keyH.playerY >= centerY) {
		return 3;
	}
	else if (keyH.playerX >= centerX && keyH.playerY >= centerY) {
		return 4;
	}
	else {
		return 0;
	}
}
