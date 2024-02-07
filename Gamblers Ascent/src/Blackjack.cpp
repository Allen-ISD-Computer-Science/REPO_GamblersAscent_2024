#include "Blackjack.h"


Blackjack::Blackjack(SDL_Handler* handler, KeyboardHandler* keyboardHandler, Asset_Manager* Asset_Manager, Spritesheet_Handler* cardSpritesheet, Spritesheet_Handler* chipSpritesheet) : userTurn(true), betPhase(false), turnCount(0), 
	cardX(0), chipX(0), chipY(0), dealerChipX(0), dealerChipY(0), chipSrcRect({0,0, 0, 0}), isRunning(true), frameStart(0), frameTime(0), botWait(0),
	m_handler(handler), m_keyboardHandler(keyboardHandler), m_Asset_Manager(Asset_Manager), m_cardSpritesheet(cardSpritesheet), m_chipSpritesheet(chipSpritesheet) {
	
	cards = new Image_Render(m_handler, 41, 59);
	chips = new Image_Render(m_handler, 17, 17);
	blackjackScreen = new Image_Render(m_handler, 640, 360);
	icons = new Image_Render(m_handler, 32, 32);
};
Blackjack::~Blackjack() {
	delete cards;
	delete chips;
	delete blackjackScreen;
	delete icons;
}
// Starts the game of blackjack
void Blackjack::newGame() {

	// Defines the two players and their starting cash
	Player *m_player1 = new Player("Will", false, 1500);
	Player *m_player2 = new Player("Tinman", true, 1500);
	player1 = *m_player1;
	player2 = *m_player2;
	// Shuffles and merges the two decks
	deck1.shuffle(); deck2.shuffle();
	deck1.mergeDecks(deck2);
	

	// Starts the first turn
	newTurn();
}
void Blackjack::newTurn() {
	// Resets the player's bet amount and deals them two cards
	std::cout << "New turn, dealing cards" << std::endl;
	player1.hand.addCard(deck1.dealCard());
	player2.hand.addCard(deck1.dealCard());
	player1.hand.addCard(deck1.dealCard());
	player2.hand.addCard(deck1.dealCard());
	// Displays the balance of both players
	std::cout << player1.name << " balance: " << player1.money << std::endl;
	std::cout << player2.name << " balance: " << player2.money << std::endl;
	betPhase = true;
	userTurn = (player1.isDealer) ? false : true;
}
// The main game loop. Runs 30 frames a second, so checks like the player's hand status shouldn't be done here unless updated with sdl events
void Blackjack::gameLoop() {
	while (isRunning) {
		frameStart = SDL_GetTicks();

		// Game logic
		if (!userTurn && player2.isDealer) {
			dealerPhase(&player1, &player2);
		}
		else if (!userTurn && !player2.isDealer) {
			playerPhase(&player2, &player1);
		}
		else {
			// Checks for events
			if (SDL_PollEvent(&m_handler->event)) {
				//checking what event is occuring
				switch (m_handler->event.type) {
				case (SDL_QUIT):
					isRunning = false;
					return;
				case (SDL_WINDOWEVENT):
					if (m_handler->event.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						m_handler->ResizeWindow(m_handler->event.window.data1, m_handler->event.window.data2);
						//			keyboardHandler.screenCenterX = static_cast<float>(handler.screenWidth / 2) - (playerWidth / static_cast<float>(2));
						//			keyboardHandler.screenCenterY = static_cast<float>(handler.screenHeight / 2) - (playerHeight / static_cast<float>(2));
					}
				case (SDL_MOUSEBUTTONDOWN):
					if (m_handler->event.button.button == SDL_BUTTON_LEFT) {
						mouseClick({ m_handler->event.button.x, m_handler->event.button.y });
					}
					break;
				case (SDL_MOUSEBUTTONUP):
					break;
				}
			}
		}

		
		render();



		//Delaying the frame rate
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }

	}
}

void Blackjack::render() {
	m_handler->ClearRenderer();
	SDL_Rect backgroundRect = { 0, 0, 640, 360 };
	// rendering the background
	blackjackScreen->render(m_Asset_Manager->Assets[2], backgroundRect, 0, 0);

	// Rendering the Dealer's hand

	// Render each card in the player's hand
	renderCardsNChips(&player1, &player2);

	// Renders the player/dealer icon
	if (player1.isDealer) 
		icons->render(m_Asset_Manager->Assets[5], { 0, 0, 32, 32 }, 40, 46);
	else 
		icons->render(m_Asset_Manager->Assets[6], { 0, 0, 32, 32 }, 40, 46);
	
}
void Blackjack::renderCardsNChips(Player* player, Player* dealer) {
	if (!betPhase) {
		for (size_t i = 0; i < player->hand.cards.size(); i++)
		{
			// Calculate the position for the current card
			cardX = startX + (i * (m_cardSpritesheet->spriteWidth + cardSpacing));

			// Use Spritesheet_Handler to get the SDL_Rect for the current card's image
			cardSrcRect = m_cardSpritesheet->getSprite(
				static_cast<int>(player->hand.cards[i].suit),
				static_cast<int>(player->hand.cards[i].rank)
			);

			// Render the card
			cards->render(m_Asset_Manager->Assets[3], cardSrcRect, cardX, cardY);
		}
	}
	// Render the chips if the player can afford them. Checks from highest to lowest value
	for (int i = 9; i >= 0; i--) {
		if (player->money >= static_cast<int>(chipValues[i])) {
			// Use Spritesheet_Handler to get the SDL_Rect for the current chip's image
			chipSrcRect = m_chipSpritesheet->getSprite(i + 1, 1);
			// Calculate the position for the current chip. If the chip is Chip_50 or higher it will be renderd in the next row 20 pixels below the previous column and starting back at the first row
			if (i < 5) {
				chipX = chipStartingX + (i * (m_chipSpritesheet->spriteWidth + chipSpacing));
				chipY = chipStartingY;
			}
			else {
				chipX = chipStartingX + ((i - 5) * (m_chipSpritesheet->spriteWidth + chipSpacing));
				chipY = chipStartingY + 20;
			}
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, chipX, chipY);
		}
	}
	// Render the topmost chip in the player's bet
	if (player->betAmount > 0) {
		auto chipIterator = std::find(std::begin(chipValues), std::end(chipValues), player->currentChips.back());

		if (chipIterator != std::end(chipValues)) {
			// Element found, get its index
			std::ptrdiff_t chipIndex = std::distance(std::begin(chipValues), chipIterator);

			// Use the index to get the sprite
			chipSrcRect = m_chipSpritesheet->getSprite(chipIndex + 1, 1);
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, 311, 213);
		}
	}
	//    ---------------DEALER----------------
	// Render the Dealer's graphics. Exactly the same as the player, except the first card is flipped (accessed at getSprite(0, 14)) and the x and y locations are flipped, as well as the graphis being rendered at 180 degrees using SDL_RenderCopyEx()
	if (!betPhase) {
		for (size_t i = 0; i < dealer->hand.cards.size(); i++)
		{
			// Calculate the position for the current card
			int dealerCardX = dealerStartX - (i * (m_cardSpritesheet->spriteWidth + cardSpacing));
			SDL_Point centerPoint = { dealerCardX, dealerCardY };
			// Use Spritesheet_Handler to get the SDL_Rect for the current card's image
			cardSrcRect = m_cardSpritesheet->getSprite(
				static_cast<int>(dealer->hand.cards[i].suit),
				static_cast<int>(dealer->hand.cards[i].rank)
			);
			cardDestRect = { dealerCardX , dealerCardY, m_cardSpritesheet->spriteWidth, m_cardSpritesheet->spriteHeight };
			// Render the card, then flips it using SDL_RenderCopyEx()
				cards->render(m_Asset_Manager->Assets[3], cardSrcRect, dealerCardX, dealerCardY);
		}
	}// Render the chips if the dealer can afford them. Checks from highest to lowest value
	for (int i = 9; i >= 0; i--) {
		if (dealer->money >= static_cast<int>(chipValues[i])) {
			// Use Spritesheet_Handler to get the SDL_Rect for the current chip's image
			chipSrcRect = m_chipSpritesheet->getSprite(i + 1, 1);
			// Calculate the position for the current chip. If the chip is Chip_50 or higher it will be renderd in the next row 20 pixels below the previous column and starting back at the first row
			if (i < 5) {
				dealerChipX = dealerChipStartingX + (i * (m_chipSpritesheet->spriteWidth + chipSpacing));
				dealerChipY = dealerChipStartingY;
			}
			else {
				dealerChipX = dealerChipStartingX + ((i - 5) * (m_chipSpritesheet->spriteWidth + chipSpacing));
				dealerChipY = dealerChipStartingY + 20;
			}
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, dealerChipX, dealerChipY);
		}
	}
	// Render the topmost chip in the dealers's bet
	if (dealer->betAmount > 0) {
		auto chipIterator = std::find(std::begin(chipValues), std::end(chipValues), dealer->currentChips.back());

		if (chipIterator != std::end(chipValues)) {
			// Element found, get its index
			std::ptrdiff_t chipIndex = std::distance(std::begin(chipValues), chipIterator);

			// Use the index to get the sprite
			chipSrcRect = m_chipSpritesheet->getSprite(chipIndex + 1, 1);
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, 311, 146);
		}
	}
}


// Updates the card rendering positions. Not used right now.
void Blackjack::cardUpdate() {
	cardSrcRects.clear();
	cardX = startX;
	for (size_t i = 0; i < player1.hand.cards.size(); i++)
	{
		// Calculate the position for the current card
		cardX = startX + (i * (static_cast<unsigned long long>(m_cardSpritesheet->spriteWidth) + cardSpacing));

		// Use Spritesheet_Handler to get the SDL_Rect for the current card's image
		cardSrcRects.push_back(m_cardSpritesheet->getSprite(
			static_cast<int>(player1.hand.cards[i].suit),
			static_cast<int>(player1.hand.cards[i].rank)
		));
	}
}

// Function that handles chip clicks. Uses the Chip enum to determine the value of the chip and adds it to the player's bet if they have enough money
void Blackjack::chipInput(Chip c, Player* p)
{
	if (p->money >= static_cast<int>(c)) {
		p->betAmount += static_cast<int>(c);
		p->money -= static_cast<int>(c);
		p->currentChips.push_back(c);
	}

}
void Blackjack::endTurn() 
{
	userTurn = !userTurn;
}
void Blackjack::endGame() {
	// Deletes the player objects and resets the game
	delete &player1;
	delete &player2;
	newGame();
}
void Blackjack::endRound(Player* winner, Player* loser) {
	// Adds the bet amount to the winner's money and resets the bet amount

	std::cout << winner->name << " won " << winner->betAmount << std::endl;
	if (winner->hand.isBlackjack() || winner->hand.cardsInHand() >= 5)
		winner->money += winner->betAmount*2.5;
	else 
		winner->money += winner->betAmount*2;
	render();
	SDL_Delay(3000);
	// Swaps player and dealer, and starts a new turn
	winner->hand.cards.clear(); loser->hand.cards.clear(); 
	winner->currentChips.clear(); loser->currentChips.clear();
	winner->betAmount = 0, loser->betAmount = 0;

	winner->isDealer = !winner->isDealer; loser->isDealer = !loser->isDealer;
	newTurn();
}
void Blackjack::moneyToChip(Player* p) const {
	// Converts the player's money into chips. Takes the player's bet amount and divides it by the highest chip value. Then it adds that chip to the player's currentChips vector and subtracts the chip value from the bet amount. It repeats this until the bet amount is 0.
	p->currentChips.clear();
	int tempBet = p->betAmount;

	for (int i = 0; i < sizeof(chipValues) / sizeof(chipValues[0]); ++i) {
		while (tempBet >= static_cast<int>(chipValues[i])) {
			p->currentChips.push_back(chipValues[i]);  // Assuming Chip is an enum with bit values
			tempBet -= static_cast<int>(chipValues[i]);
		}
	}
}

void Blackjack::turnCalculation(Player* player, Player* dealer) {
	// Compares the player's hand to the dealer's hand and determines the winner
	if (player->hand.isBlackjack() || player->hand.cardsInHand() >= 5) {
		endRound(player, dealer);
	}
	else if (dealer->hand.isBlackjack() || dealer->hand.cardsInHand() >= 5) {
		endRound(dealer, player);
	}
	else if (player->hand.handValue() > dealer->hand.handValue()) {
		endRound(player, dealer);
	}
	else if (player->hand.handValue() < dealer->hand.handValue()) {
		endRound(dealer, player);
	}
	else {
		endRound(dealer, player);
	}
}

// The AI's Player and Dealer Phases. The dealer will always hit until they have 17 or higher. The player will hit until they stand, double down, or bust.
void Blackjack::playerPhase(Player* player, Player* dealer)
{
	if (botWait < 60) {
		botWait++;
		return;
	}
	else {
		botWait = 0;
		if (betPhase) {
			player->betAmount += player->money/5 ;
			player->money -= player->betAmount;			
			moneyToChip(player);
			if (player->betAmount > dealer->money) {
				dealer->betAmount = dealer->money;
				dealer->money -= dealer->betAmount;
				moneyToChip(dealer);
			}
			else {
				dealer->betAmount += player->betAmount;
				dealer->money -= player->betAmount;
				moneyToChip(dealer);
			}
			std::cout << player->name <<" bet: " << player->betAmount << std::endl;
			betPhase = false;
		}
		else {
			if (player->hand.cardsInHand() == 2 && (player->hand.handValue() >= 9 && player->hand.handValue() <= 11)) {
				// Double down
				std::cout << player->name << " doubled down" << std::endl;
				player->hand.addCard(deck1.dealCard()); 
				player->betAmount *= 2;   
				player->money -= player->betAmount;  
				if (player->betAmount > dealer->money) {
					dealer->betAmount = dealer->money;
					dealer->money -= dealer->betAmount;
					moneyToChip(dealer);
				}
				else {
					dealer->betAmount += player->betAmount;
					dealer->money -= player->betAmount;
					moneyToChip(dealer);
				}
				moneyToChip(player); 
				if (player->hand.isBust())
					endRound(dealer, player);
				else if (player->hand.isBlackjack())
					endRound(player, dealer);
				else
					endTurn();

			}
			else if (player->hand.cardsInHand() == 2 && player->hand.cards[0].rank == player->hand.cards[1].rank) {
				// Split
			}
			if (player->hand.handValue() < 17) {
				std::cout << player->name << " hit" << std::endl;
				player->hand.addCard(deck1.dealCard());
				if (player->hand.isBust())
					endRound(dealer, player);
				else if (player->hand.isBlackjack())
					endRound(player, dealer);
			}
			else {
				std::cout << player->name << " stood" << std::endl;
				endTurn();
			}
		}
	}
	
}

void Blackjack::dealerPhase(Player* player, Player* dealer)
{
	if (botWait < 60) {
		botWait++;
		return;
	}
	else {
		botWait = 0;

		if (dealer->hand.handValue() < 17) {
			std::cout << dealer->name << " Bot hit" << std::endl;
			dealer->hand.addCard(deck1.dealCard());
		}
		else {
			std::cout << dealer->name << " Bot stood" << std::endl;
			turnCalculation(player, dealer);
		}
	}
}
// Handles mouse clicks. If the player clicks on a button it will call the appropriate function
void Blackjack::mouseClick(SDL_Point pos)
{
	// Checks if its not the players turn, and if it isnt then will check a different array of buttons
	if (!userTurn) {
		/*
			WIP
		*/
	}

	else {
		for (int i = 0; i < sizeof(buttonRects) / sizeof(buttonRects[0]); i++) {
			if (SDL_PointInRect(&pos, &buttonRects[i]) && !betPhase) {
				switch (i) {
				case 0:
					// Hit
					hit(&player1);

					break;
				case 1:
					// Stand
					if (!player1.isDealer) {
						endTurn();
					}
					else {
						turnCalculation(&player1, &player2);
					}
					break;
				case 2:
					// Double
					if (!player1.isDealer) { 
						doubleDown(&player1); 
					}
					break;
				case 3:
					// Split
					break;
				}
				cardUpdate();
			}
			else if (SDL_PointInRect(&pos, &buttonRects[i]) && betPhase) {
				switch (i) {
				case 0:
					break;
				case 1:
				case 2:
				case 3:
					// Starts the game
					betPhase = false;
					break;
				case 14:
					// Removes topmost chip from bet
					if (player1.currentChips.size() == 0) {
						break;
					}
					player1.money += static_cast<int>(player1.currentChips.back());
					player1.betAmount -= static_cast<int>(player1.currentChips.back());
					player2.betAmount -= static_cast<int>(player1.currentChips.back());
					player1.currentChips.pop_back();
					break;
				default:
					chipInput(chipValues[i - 4], &player1);
					if (player1.betAmount > player2.money) {
						player2.betAmount = player2.money;
						player2.money -= player2.betAmount;
						moneyToChip(&player2);
					}
					else {
						player2.betAmount += player1.betAmount;
						player2.money -= player1.betAmount;
						moneyToChip(&player2);
					}
					break;
				}
			}
		}
	}
}
// Hit, stand, doubledown, and split functions
void Blackjack::hit(Player* p) {
	Player* p2 = p == &player1 ? &player2 : &player1;
	p->hand.addCard(deck1.dealCard());
	if (p->hand.isBust())
		endRound(p2, p);
	else if (player1.hand.isBlackjack())
		endRound(p, p2);
}

void Blackjack::doubleDown(Player* p) {
	Player* p2 = p == &player1 ? &player2 : &player1;
	p->hand.addCard(deck1.dealCard());
	p->betAmount *= 2;
	p->money -= p->betAmount;
	moneyToChip(p);
	if (p->hand.isBust())
		endRound(p2, p);
	else if (player1.hand.isBlackjack())
		endRound(p, p2);
	else
		endTurn();

}
void Blackjack::split(Player* p) {
	// WIP
}


