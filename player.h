#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "cards.h"

typedef struct player{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	char * name;

	//Money!
	int funds;

	int cardsInHand;

	card *hand[];
}player;

player *createPlayer(char *playerName, int initialFunds);
void killPlayer(player *playerPtr);

#endif
