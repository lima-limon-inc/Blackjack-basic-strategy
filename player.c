#include "player.h"

//The amount of cards a player has in their hand will vary. According to this
//very scientific website: https://www.blackjackincolor.com/cardsperround.htm
//"The books tell us that the average hand in Blackjack (including the dealer 
//hands) contains about 2.7 cards"
//Maybe it's not the most precise thing in the world, but the number makes 
//sense. We'll assume it to be a bit bigger, just to simplify things and avoid
//the amount of times we need to realloc memory
#define INITIALPLAYERCARDS 4

static playerHand *createPlayerHand() {
	int initialSize;
	initialSize = sizeof(playerHand) + INITIALPLAYERCARDS * sizeof(card *);

	playerHand *pokerHand;
	pokerHand = (playerHand *) malloc(initialSize); 
	pokerHand->cardsInHand = 0;
	pokerHand->cardCapacity = INITIALPLAYERCARDS;
	pokerHand->bet = 0;
	pokerHand->cardSum = 0;
	
	return pokerHand;
}

player *createPlayer(char *playerName, int initialFunds) {
	int initialSize;
	initialSize = sizeof(player);

	player *pokerPlayer;
	pokerPlayer = (player *) malloc(initialSize); 

	playerHand *playersHand;
	playersHand = createPlayerHand();

	strcpy(pokerPlayer->name, playerName);
	pokerPlayer->funds = initialFunds;
	pokerPlayer->playerHands[0] = playersHand;
	pokerPlayer->howManyHands = 1;

	return pokerPlayer;
}

//TODO: Change function name if I want to get PEGI 13
void killPlayer(player *playerPtr){
	for (int i = 0; i < playerPtr->howManyHands; i++) {
		free(playerPtr->playerHands[i]);
	}
	free(playerPtr);
}

static playerHand *resizeHand(playerHand *playerHandPtr) {
	//If there's still margin, we don't realloc memory
	if (playerHandPtr->cardCapacity > playerHandPtr->cardsInHand) {
		/* printf("DIDN'T RESIZE\n"); */
		return playerHandPtr;
	}

	//We will downsize if it's way to big
	//                   8                             2      * 2
	else if (playerHandPtr->cardCapacity > playerHandPtr->cardsInHand * 2) {
		printf("DOWNSIZE\n");

		//Set the new capacity
		playerHandPtr->cardCapacity /= 2; //Divided equal (fancy notation)
	}

	//We need more memory
	else {
		printf("UPSIZE\n");

		//Set the new capacity
		playerHandPtr->cardCapacity *= 2; //Divided equal (fancy notation)
	}
		
	playerHand *resizedPlayer;
	int newSize = sizeof(playerHand) + 
			playerHandPtr->cardCapacity * sizeof(card *);


	resizedPlayer = realloc(playerHandPtr, newSize);
	return resizedPlayer;
}

void receiveCard(player *playerPtr, card *newCard, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);

	activePlayerHand = resizeHand(activePlayerHand);

	activePlayerHand->hand[activePlayerHand->cardsInHand] = newCard;
	activePlayerHand->cardsInHand += 1;
}

void removeCards(player *playerPtr, int whichHand) {
	//We simply move the index. The pointer will simply get removed by
	//the receiveCard function (it will overwrite the value)
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	activePlayerHand->cardsInHand = 0;
}

void printCards(player *playerPtr, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	for (int i = 0; i < activePlayerHand->cardsInHand; i++) {
		printf("%s's card: ", playerPtr->name); 
		printf("%d, %d \n", activePlayerHand->hand[i]->rank, 
				activePlayerHand->hand[i]->suit);
	}
}

void makeABet(player *playerPtr, int bet, int whichHand) {
	//TODO: Create if statement to check if you have enough funds
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	activePlayerHand->bet += bet;
	playerPtr->funds -= bet;
}

//This returns the money to symbolize the dealer taking it
int loseBet(player *playerPtr, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	int lostBet;
	lostBet = activePlayerHand->bet;

	activePlayerHand->bet = 0;
	return lostBet;
}

void winBet(player *playerPtr, int awardedMoney, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	playerPtr->funds += awardedMoney;

	activePlayerHand->bet = 0;
}

int getBet(player *playerPtr, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);
	return activePlayerHand->bet;
}

int getNumberOfHands(player *playerPtr) {
	return playerPtr->howManyHands;
}

//TODO: Make it return a copy? Maybe
playerHand *getSpecificHand(player *playerPtr, int whichHand) {
	return playerPtr->playerHands[whichHand];
}

void increaseBet(player *playerPtr, int increaseAmount, int whichHand) {
	makeABet(playerPtr, increaseAmount, whichHand);
}

void saveCardSum(playerHand *playersHand, int newSum) {
	playersHand-> cardSum = newSum;
}


/* PLAYERHAND FUNCTIONS */

card **getCards(playerHand *playersHand) {
	return playersHand->hand;
}

int getAmountOfCardsInHand(playerHand *playersHand) {
	return playersHand->cardsInHand;
}

int getHandSum(playerHand *playersHand) {
	return playersHand->cardSum;
}
