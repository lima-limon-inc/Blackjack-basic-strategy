#include "dealer.h"

dealer *createDealer(int initialFunds, shoe *givenPokerShoe){
	dealer *newDealer;
	newDealer = (dealer *) malloc (sizeof(dealer));

	newDealer->pokerShoe = givenPokerShoe;
	newDealer->funds = initialFunds;

	return newDealer;
}

void killDealer(dealer *dealerPtr){
	free (dealerPtr);
}

card *dealACard(const dealer *dealerPtr){
	card *cardAtTheTop = getCardAtTheTop(dealerPtr->pokerShoe);
	return cardAtTheTop;
}
