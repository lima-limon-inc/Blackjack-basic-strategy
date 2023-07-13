#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "cards.h"

typedef struct player{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	char * name;

	//Money!
	int funds;

	card *hand[];
}player;

player *createPlayer(char *playerName, int initialFunds);

#endif
