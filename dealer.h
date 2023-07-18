#ifndef DEALER_HEADER
#define DEALER_HEADER

#include "cards.h"

typedef struct dealer{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	//Money!
	int funds;

	shoe *pokerShoe;

	int cardsInHand;

	int cardCapacity;
	
	int cardSum;

	card *hand[];

}dealer;

dealer *createDealer(int initialFunds);

void killDealer(dealer *dealerPtr);

card *dealACard(const dealer *dealerPtr);

//TODO: Use _Generic macro to do fancy function overloading
//with player struct
dealer *dealDealersHand(dealer *dealerPtr, card *newCard);

void removeDealersCards(dealer *dealerPtr);

void takeMoney(dealer *dealerPtr, int money);

void removeMoneyFromFunds(dealer *dealerPtr, int moneyToPlayer);

#endif