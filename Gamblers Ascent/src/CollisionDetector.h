//#pragma once
//#include "SDL_Handler.h"
//#include "SDL_Image.h"
//#include "KeyboardHandler.h"
//#include <array>

//struct CollisionLine {
	//int x1;
	//int y1;
	//int x2;
	//int y2;
//};

//class CollisionDetector
//{
//public:
	// create a std Vector of a group of CollisionLines that represent objects in the game. Split into 4 sections of the map 
	// to reduce the amount of Lines that need to be checked for collision
	// Adjust the Lines to fit the map by changing the numbers with respect to -500, -345 
	//CollisionLine objectBoundaries[4][5] =
	//{
		//{
			//{},
			//{},
			//{},
			//{},
			//{},
		//},
		//{
			//{-238, 7, -200, 27},
			//{-198, 27, 46, -95},
			//{},
			//{},
			//{},
		//},
		//{
			//{-370, 45, -252, 103},
			//{-252, 103, -252, 131},
			//{-250, 111, 128, 319},
			//{},
			//{},
		//},
		//{
			//{},
			//{},
			//{},
			//{},
			//{},
		//}
	//};
	// Center of the screen to base the getSection() function off of
	//float centerX;
	//float centerY;

	// create a array of a group of CollisionLine that represents the boundaries of the map as well as walls
	//CollisionLine mapBoundaries[4] =
	//{
		//{-500, -345, 500, -345},
		//{-500, -345, -500, 345},
		//{-500, 345, 500, 345},
		//{500, -345, 500, 345}
	//};
	 

	
//private:
	//KeyboardHandler keyH;
//public:

	//CollisionDetector(KeyboardHandler keyH);
	//~CollisionDetector();
	// Check if the player is colliding with any of the CollisionLine in the Array matrix based on one of the 4 sections of the map the player is in
	//bool checkCollision(SDL_Rect* rect, int section);

//private:
	// Function that defines the section of the map the player is located in

	//int getSection();


//};