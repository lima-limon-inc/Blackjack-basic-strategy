#include "player.h"


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
		removeHand(getSpecificHand(playerPtr, i));
	}
	free(playerPtr);
}


void receiveCard(player *playerPtr, card *newCard, int whichHand) {
	playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand);

	activePlayerHand = resizeHand(activePlayerHand);
	playerPtr->playerHands[whichHand] = activePlayerHand;

	activePlayerHand->hand[activePlayerHand->cardsInHand] = newCard;
	activePlayerHand->cardsInHand += 1;
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
