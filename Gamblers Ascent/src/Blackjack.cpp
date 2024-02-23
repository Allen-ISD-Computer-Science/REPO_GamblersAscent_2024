#include "Blackjack.h"


Blackjack::Blackjack(SDL_Handler* handler, KeyboardHandler* keyboardHandler, Asset_Manager* Asset_Manager, Spritesheet_Handler* cardSpritesheet, Spritesheet_Handler* chipSpritesheet) : userTurn(true), betPhase(false), turnCount(0), 
	cardX(0), chipX(0), chipY(0), dealerChipX(0), dealerChipY(0), isRunning(true), frameStart(0), frameTime(0), botWait(0),
	chipSrcRect({ 0,0, 0, 0 }), cardSrcRect({ 0,0, 0, 0 }), cardDestRect({ 0,0, 0, 0 }),
	m_handler(handler), m_keyboardHandler(keyboardHandler), m_Asset_Manager(Asset_Manager), m_cardSpritesheet(cardSpritesheet), m_chipSpritesheet(chipSpritesheet) {
	
	cards = new Image_Render(m_handler, 123, 177);
	chips = new Image_Render(m_handler, 51, 51);
	blackjackScreen = new Image_Render(m_handler, 1920, 1080);
	icons = new Image_Render(m_handler, 96, 96);
	botAI = new BlackjackAI(5, 5, 5, 10);
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
	player = *m_player1;
	bot = *m_player2;
	// Shuffles and merges the two player decks
	playerDeck.shuffle(); playerDeck2.shuffle();
	playerDeck.mergeDecks(playerDeck2);
	
	// Shuffles and merges the two bot decks
	botDeck.shuffle(); botDeck2.shuffle();
	botDeck.mergeDecks(botDeck2);

	// Starts the bet phase
	betPhase = true;
}
void Blackjack::newTurn() {
	// Resets the player's bet amount and deals them two cards
	std::cout << "New turn, dealing cards" << std::endl;
	player.hand.addCard(playerDeck.dealCard());
	bot.hand.addCard(botDeck.dealCard());
	player.hand.addCard(playerDeck.dealCard());
	bot.hand.addCard(botDeck.dealCard());
	// Displays the balance of both players
	std::cout << player.name << " balance: " << player.money + player.betAmount << std::endl;
	std::cout << bot.name << " balance: " << bot.money + player.betAmount << std::endl;

	// Checks for early blackjacks
	if (player.hand.isBlackjack()) {
		endRound(&player, &bot);
	}

	// If the player is the dealer, they will start the game after the bot
	userTurn = (player.isDealer) ? false : true;
}
// The main game loop. Runs 30 frames a second, so checks like the player's hand status shouldn't be done here unless updated with sdl events
void Blackjack::gameLoop() {
	while (isRunning) {
		frameStart = SDL_GetTicks();

		// Game logic
		if (!userTurn && bot.isDealer) {
			dealerPhase(&player, &bot);
		}
		else if (!userTurn && !bot.isDealer) {
			playerPhase(&bot, &player);
		}
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
				if (m_handler->event.button.button == SDL_BUTTON_LEFT && userTurn) {
					mouseClick({ m_handler->event.button.x, m_handler->event.button.y });
				}
				break;
			case (SDL_MOUSEBUTTONUP):
				break;
			}
		}
		m_keyboardHandler->UpdateKeyStates();

		if (m_keyboardHandler->ESC_key) {
			isRunning = false;
		}
	
		render();



		//Delaying the frame rate
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }

	}
}
// Renders the game. The game is rendered in the following order: Background, Player's hand, Dealer's hand, Player's chips, Dealer's chips, Player's icon, Dealer's icon
void Blackjack::render() {


	m_handler->ClearRenderer();
	SDL_Rect backgroundRect = { 0, 0, 1920, 1080 };
	// rendering the background
	blackjackScreen->render(m_Asset_Manager->Assets[2], backgroundRect, 0, 0);


	// Render each card in the player and dealer's hand
	renderCardsNChips(&player, &bot);

	// Renders the player/dealer icon
	if (player.isDealer) 
		icons->render(m_Asset_Manager->Assets[5], { 0, 0, 96, 96 }, 120, 138);
	else 
		icons->render(m_Asset_Manager->Assets[6], { 0, 0, 96, 96 }, 120, 138);

	SDL_RenderPresent(m_handler->renderer);

	SDL_UpdateWindowSurface(m_handler->m_window);
	
}
void Blackjack::calculateCardPositions(Player* player, Player* dealer)
{

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
				chipX = chipStartingX + (i * (m_chipSpritesheet->spriteWidth * 3 + chipSpacing));
				chipY = chipStartingY;
			}
			else {
				chipX = chipStartingX + ((i - 5) * (m_chipSpritesheet->spriteWidth * 3  + chipSpacing));
				chipY = chipStartingY + 60;
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
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, 933, 639);
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
			if (i == 1 && userTurn && bot.isDealer ) {
				cardSrcRect = m_cardSpritesheet->getSprite(1, 14);
			}
			else {
				cardSrcRect = m_cardSpritesheet->getSprite(
					static_cast<int>(dealer->hand.cards[i].suit),
					static_cast<int>(dealer->hand.cards[i].rank)
				);
			}
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
				dealerChipX = dealerChipStartingX + (i * (m_chipSpritesheet->spriteWidth * 3 + chipSpacing));
				dealerChipY = dealerChipStartingY;
			}
			else {
				dealerChipX = dealerChipStartingX + ((i - 5) * (m_chipSpritesheet->spriteWidth * 3 + chipSpacing));
				dealerChipY = dealerChipStartingY + 60;
			}
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, dealerChipX, dealerChipY);
		}
	}
	// Render the topmost chip in the dealers's bet
	if (dealer->betAmount > 0 && dealer->currentChips.size() > 0) {
		auto chipIterator = std::find(std::begin(chipValues), std::end(chipValues), dealer->currentChips.front());

		if (chipIterator != std::end(chipValues)) {
			// Element found, get its index
			std::ptrdiff_t chipIndex = std::distance(std::begin(chipValues), chipIterator);

			// Use the index to get the sprite
			chipSrcRect = m_chipSpritesheet->getSprite(chipIndex + 1, 1);
			chips->render(m_Asset_Manager->Assets[4], chipSrcRect, 933, 438);
		}
	}
}


// Updates the card rendering positions. Not used right now.
void Blackjack::cardUpdate() {
	cardSrcRects.clear();
	cardX = startX;
	for (size_t i = 0; i < player.hand.cards.size(); i++)
	{
		// Calculate the position for the current card
		cardX = startX + (i * (static_cast<unsigned long long>(m_cardSpritesheet->spriteWidth) + cardSpacing));

		// Use Spritesheet_Handler to get the SDL_Rect for the current card's image
		cardSrcRects.push_back(m_cardSpritesheet->getSprite(
			static_cast<int>(player.hand.cards[i].suit),
			static_cast<int>(player.hand.cards[i].rank)
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
	delete &player;
	delete &bot;
	
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
	// Swaps player and dealer, and starts the next bet phase
	winner->hand.cards.clear(); loser->hand.cards.clear(); 
	winner->currentChips.clear(); loser->currentChips.clear();
	winner->betAmount = 0, loser->betAmount = 0;

	winner->isDealer = !winner->isDealer; loser->isDealer = !loser->isDealer;
	userTurn = (player.isDealer) ? false : true;
	betPhase = true;
}

void Blackjack::endRound() {
	// If the player and dealer have the same hand value, the player gets their bet back
	player.money += player.betAmount;
	bot.money += bot.betAmount;
	player.betAmount = 0;
	bot.betAmount = 0;
	player.currentChips.clear();
	bot.currentChips.clear();
	render();
	SDL_Delay(3000);

	// Swaps player and dealer, and starts the next bet phase
	player.hand.cards.clear(); bot.hand.cards.clear();
	player.currentChips.clear(); bot.currentChips.clear();
	player.betAmount = 0, bot.betAmount = 0;
	player.isDealer = !player.isDealer; bot.isDealer = !bot.isDealer;

	userTurn = (player.isDealer) ? false : true;
	betPhase = true;
}
void Blackjack::moneyToChip(Player* p) const {
	// Converts the player's money into chips. Takes the player's bet amount and divides it by the highest chip value. Then it adds that chip to the player's currentChips vector and subtracts the chip value from the bet amount. It repeats this until the bet amount is 0.
	p->currentChips.clear();
	int tempBet = p->betAmount;

	for (int i = sizeof(chipValues) / sizeof(chipValues[0]) - 1; i >= 0; --i) {
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
		endRound();
	}
}

// The AI's Player and Dealer Phases. Powered by BlackjackAI
void Blackjack::playerPhase(Player* player, Player* dealer)
{
	if (botWait < 60) {
		botWait++;
		return;
	}
	else {
		botWait = 0;
		if (betPhase) {
			player->betAmount += botAI->determineBet(player->money);
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
			newTurn();
		}
		else {
			switch (botAI->determineAction(player->hand, dealer->hand.cards[0])) 
			{
			case Hit:
				std::cout << player->name << " hit" << std::endl;
				if (botAI->determineLuck() && (player->hand.handValue() + static_cast<int>(botDeck.peekTop().get_value())) < 21) {
					// Guarentees the bot a card that will bring them to 20
					int neededValue = 20 - player->hand.handValue();
					Card tempCard(Card::suit_t::spades, static_cast<Card::rank_t>(neededValue));
					botDeck.replaceTop(tempCard);
				}
				player->hand.addCard(botDeck.dealCard());
				if (player->hand.isBust())
					endRound(dealer, player);
				else if (player->hand.isBlackjack())
					endRound(player, dealer);
				break;
			case Stand:
				std::cout << player->name << " stood" << std::endl;
				endTurn();
				break;
			case Double:
				std::cout << player->name << " doubled down" << std::endl;
				player->hand.addCard(botDeck.dealCard());
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
				break;
			case Split:
				// WIP
				break;
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
		switch (botAI->determineAction(dealer->hand, player->hand))
		{
		case Hit:
			if (botAI->determineLuck() && (player->hand.handValue() + static_cast<int>(botDeck.peekTop().get_value())) < 21) { 
				// Guarentees the bot a card that will bring them to 20 
				int neededValue = 20 - player->hand.handValue(); 
				Card tempCard(Card::suit_t::spades, static_cast<Card::rank_t>(neededValue)); 
				botDeck.replaceTop(tempCard); 
			} 
			std::cout << dealer->name << " Bot hit" << std::endl;
			dealer->hand.addCard(botDeck.dealCard());
			if (dealer->hand.isBust())
				endRound(player, dealer);
			else if (dealer->hand.isBlackjack())
				endRound(dealer, player);
			break;
		case Stand:
			std::cout << dealer->name << " Bot stood" << std::endl;
			turnCalculation(player, dealer);
			break;		
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
					hit(&player);

					break;
				case 1:
					// Stand
					if (!player.isDealer) {
						endTurn();
					} 
					else {
						turnCalculation(&player, &bot);
					}
					break;
				case 2:
					// Double
					if (!player.isDealer) { 
						doubleDown(&player); 
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
					newTurn();
					break;
				case 14:
					// Removes topmost chip from bet
					if (player.currentChips.size() == 0) { break; }

					player.money += static_cast<int>(player.currentChips.back());
					player.betAmount -= static_cast<int>(player.currentChips.back());

					// Repeats for Bot
					bot.money += static_cast<int>(player.currentChips.back());
					bot.betAmount -= static_cast<int>(player.currentChips.back());
					moneyToChip(&bot);

					player.currentChips.pop_back();

					break;
				default:
					// Adds the chip value to the player's bet if they can afford it
					if (static_cast<int>(chipValues[i - 4]) > player.money) { break; }
					chipInput(chipValues[i - 4], &player);
					if (player.betAmount > bot.money + bot.betAmount) {
						bot.betAmount = bot.money;
						bot.money = 0;
						moneyToChip(&bot);
					}
					else {
						bot.betAmount += static_cast<int>(player.currentChips.back());
						bot.money -= static_cast<int>(player.currentChips.back());
						moneyToChip(&bot);
					}
					break;
				}
			}
		}
	}
}
// Hit, stand, doubledown, and split functions
void Blackjack::hit(Player* p) {
	Player* p2 = p == &player ? &bot : &player;
	p->hand.addCard(playerDeck.dealCard());
	if (p->hand.isBust())
		endRound(p2, p);
	else if (player.hand.isBlackjack())
		endRound(p, p2); 
}

void Blackjack::doubleDown(Player* p) {
	Player* p2 = p == &player ? &bot : &player;
	p->hand.addCard(playerDeck.dealCard());
	p->betAmount *= 2;
	p->money -= p->betAmount;
	moneyToChip(p);
	if (p->hand.isBust())
		endRound(p2, p);
	else if (player.hand.isBlackjack())
		endRound(p, p2);
	else
		endTurn();

}

void Blackjack::split(Player* p) {
	// WIP
}


