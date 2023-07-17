#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <stdbool.h>

#define INITIALCARDS 4

#include "cards.h"


typedef struct player{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	char name[20];

	//Money!
	int funds;

	int cardsInHand;

	int cardCapacity;

	card *hand[];
}player;

player *createPlayer(char *playerName, int initialFunds);

void killPlayer(player *playerPtr);

void receiveCard(player *playerPtr, card *newCard);

void removeCards(player *playerPtr);

void printCards(player *playerPtr);
		
#endif
