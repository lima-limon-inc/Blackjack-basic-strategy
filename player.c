#include "player.h"


player *createPlayer(char *playerName, int initialFunds) {
	int initialSize;
	initialSize = sizeof(player);

	player *blackjackPlayer;
	blackjackPlayer = (player *) malloc(initialSize); 

	playerHand *playersHand;
	playersHand = createPlayerHand();

	strcpy(blackjackPlayer->name, playerName);
	blackjackPlayer->funds = initialFunds;
	blackjackPlayer->howManyHands = 1;
	blackjackPlayer->playerHands[blackjackPlayer->howManyHands - 1] = playersHand;
	blackjackPlayer->bets[0] = 0;

	return blackjackPlayer;
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
	playerPtr->bets[whichHand] = bet;
	playerPtr->funds -= bet;
}

bool canMakeABet(player *playerPtr, int bet) {
	int playersFunds;
	playersFunds = getFunds(playerPtr);

	bool canMakeSpecificBet;
	canMakeSpecificBet = (playersFunds >= bet);

	return canMakeSpecificBet;
}

static void resetBet(player *playerPtr, int whichHand) {
	playerPtr->bets[whichHand] = 0;

}

//This returns the money to symbolize the dealer taking it
int loseBet(player *playerPtr, int whichHand) {
	int lostBet;
	lostBet = getBet(playerPtr, whichHand);

	resetBet(playerPtr, whichHand);
	return lostBet;
}

void winBet(player *playerPtr, int awardedMoney, int whichHand) {
	playerPtr->funds += awardedMoney;

	resetBet(playerPtr, whichHand);
}

int getBet(player *playerPtr, int whichHand) {
	return playerPtr->bets[whichHand];
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

void splitCards(player *playerPtr, int whichHand) {
	playerHand *newHand;	
	newHand = createPlayerHand();
	//TODO: Add if statement to check if the hand is already created

	//We remove the last card from the hand you want to split
	card *cardForNextHand;
	
	playerHand *handToRemoveFrom;
	handToRemoveFrom = playerPtr->playerHands[whichHand];

	//cardForNextHand and cardToBeRemoved SHOULD be the same
	cardForNextHand = removeSpecificCard(handToRemoveFrom, whichHand);

	playerPtr->playerHands[playerPtr->howManyHands] = newHand;
	receiveCard(playerPtr, cardForNextHand, playerPtr->howManyHands);

	playerPtr->howManyHands += 1;
}

int getFunds(player *playerPtr) {
	return playerPtr->funds;
}

void resetPlayer(player *playerPtr) {
	int amountOfHands;
	amountOfHands = getNumberOfHands(playerPtr);

	for (int currentHand = 0; currentHand < amountOfHands; currentHand++) {
		playerPtr->bets[currentHand] = 0;

		playerHand *currentPlayerHand;
		currentPlayerHand = getSpecificHand(playerPtr, currentHand);

		playerHand *resizeHand;
		resizeHand = resetHand(currentPlayerHand);

		playerPtr->playerHands[currentHand] = resizeHand;
	}

	playerPtr->howManyHands = 1;
}


