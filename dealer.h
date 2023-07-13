#ifndef DEALER_HEADER
#define DEALER_HEADER

#include "cards.h"

typedef struct dealer{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	//Money!
	int funds;

	shoe *pokerShoe;

}dealer;

dealer *createDealer(shoe *pokerShoe, int initialFunds);
void killDealer(dealer *dealerPtr);

#endif
