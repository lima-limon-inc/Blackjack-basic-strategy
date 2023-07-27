#ifndef PLAYERHAND_HEADER
#define PLAYERHAND_HEADER

#include "cards.h"

typedef struct playerHand{

	int cardsInHand;

	int cardCapacity;

	int bet;

	int cardSum;

	card *hand[];

}playerHand;

playerHand *createPlayerHand();

playerHand *resizeHand(playerHand *playerHandPtr);

void removeHand(playerHand *playerHandPtr);

void saveCardSum(playerHand *playersHand, int newSum);
 
card **getCards(playerHand *playersHand);

int getAmountOfCardsInHand(playerHand *playersHand);

int getHandSum(playerHand *playersHand);

#endif
