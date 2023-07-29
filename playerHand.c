#include "playerHand.h"

//The amount of cards a player has in their hand will vary. According to this
//very scientific website: https://www.blackjackincolor.com/cardsperround.htm
//"The books tell us that the average hand in Blackjack (including the dealer 
//hands) contains about 2.7 cards"
//Maybe it's not the most precise thing in the world, but the number makes 
//sense. We'll assume it to be a bit bigger, just to simplify things and avoid
//the amount of times we need to realloc memory
#define INITIALPLAYERCARDS 4

playerHand *createPlayerHand() {
	int initialSize;
	initialSize = sizeof(playerHand) + INITIALPLAYERCARDS * sizeof(card *);

	playerHand *pokerHand;
	pokerHand = (playerHand *) malloc(initialSize); 
	pokerHand->cardsInHand = 0;
	pokerHand->cardCapacity = INITIALPLAYERCARDS;
	/* pokerHand->bet = 0; */
	pokerHand->cardSum = 0;
	
	return pokerHand;
}

void removeHand(playerHand *playerHandPtr) {
	free(playerHandPtr);
}

void saveCardSum(playerHand *playersHand, int newSum) {
	playersHand-> cardSum = newSum;
}

card **getCards(playerHand *playersHand) {
	return playersHand->hand;
}

int getAmountOfCardsInHand(playerHand *playersHand) {
	return playersHand->cardsInHand;
}

int getHandSum(playerHand *playersHand) {
	return playersHand->cardSum;
}

playerHand *resizeHand(playerHand *playerHandPtr) {
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
		playerHandPtr->cardCapacity *= 2; //Multiply equal (fancy notation)
	}
		
	playerHand *resizedHand;
	int newSize = sizeof(playerHand) + 
			playerHandPtr->cardCapacity * sizeof(card *);


	resizedHand = realloc(playerHandPtr, newSize);
	return resizedHand;
}

static void resetIndex(playerHand *playerHandPtr) {
	//We simply move the index. The pointer will simply get removed by
	//the receiveCard function (it will overwrite the value)
	playerHandPtr->cardsInHand = 0;
}

card *removeSpecificCard(playerHand *playerHandPtr, int whichPosition) {
	card *removedCard;
	removedCard = playerHandPtr->hand[whichPosition];

	//If you want to remove the last card, then the for loop is never 
	//entered and you simply move the index by -one
	for (int currentCard = whichPosition; currentCard < playerHandPtr->cardsInHand; currentCard++) {
		//Swap between the current one and the next
		playerHandPtr->hand[currentCard] = playerHandPtr->hand[currentCard + 1];
	}
	playerHandPtr->cardsInHand -= 1;

	return removedCard;
}
void removeCards(playerHand *playerHandPtr) {
	resetIndex(playerHandPtr);
	//TODO add resize function call here
}
