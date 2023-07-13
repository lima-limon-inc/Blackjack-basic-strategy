#ifndef POKERTABLE_HEADER
#define POKERTABLE_HEADER

#include "dealer.h"
#include "player.h"

typedef struct pokerTable {
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	dealer *pokerDealer;

	player *players[];
}pokerTable ;

#endif
