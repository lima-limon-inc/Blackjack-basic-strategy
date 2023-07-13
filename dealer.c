#include "dealer.h"

dealer *createDealer(shoe *givenPokerShoe, int initialFunds){
	dealer *newDealer;
	newDealer = (dealer *) malloc (sizeof(dealer));

	newDealer->pokerShoe = givenPokerShoe;
	newDealer->funds = initialFunds;

	return newDealer;
}

void killDealer(dealer *dealerPtr){
	free (dealerPtr);
}

