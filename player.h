#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <stdbool.h>

#include "cards.h"

typedef struct player{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	char name[20];

	//Money!
	int funds;

	int cardsInHand;

	int cardCapacity;

	int bet;

	int cardSum;

	card *hand[];
}player;

player *createPlayer(char *playerName, int initialFunds);

void killPlayer(player *playerPtr);

player *receiveCard(player *playerPtr, card *newCard);

void removeCards(player *playerPtr);

void printCards(player *playerPtr);

void makeABet(player *playerPtr, int bet);

int loseBet(player *playerPtr);
		
void winBet(player *playerPtr, int awardedMoney);

int getBet(player *playerPtr);

void increaseBet(player *playerPtr, int increaseAmount);

#endif
