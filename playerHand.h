#ifndef PLAYERHAND_HEADER
#define PLAYERHAND_HEADER

#include "cards.h"

typedef struct playerHand{

	int cardsInHand;

	int cardCapacity;

	/* int bet; */
	//TODO: Remove bet

	int cardSum;

	card *hand[];

}playerHand;

playerHand *createPlayerHand();

playerHand *resizeHand(playerHand *playerHandPtr);

void removeHand(playerHand *playerHandPtr);

/* void saveCardSum(playerHand *playersHand, int newSum); */
 
card **getCards(playerHand *playersHand);

int getAmountOfCards(playerHand *playersHand);

int getHandSum(playerHand *playersHand);

card *removeSpecificCard(playerHand *playerHandPtr, int whichPosition);

playerHand *resetHand(playerHand *playerHandPtr);

playerHand *receiveCard(playerHand *playerHandPtr, card *newCard);

#endif
