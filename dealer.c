#include "dealer.h"

dealer *createDealer(int initialFunds) {
	shoe *newPokerShoe;
	newPokerShoe = createShuffledShoe();

	dealer *newDealer;
	newDealer = (dealer *) malloc (sizeof(dealer));

	newDealer->pokerShoe = newPokerShoe;
	newDealer->funds = initialFunds;

	return newDealer;
}

void killDealer(dealer *dealerPtr){
	destroyShoe(dealerPtr->pokerShoe);
	free (dealerPtr);
}

card *dealACard(const dealer *dealerPtr){
	card *cardAtTheTop = getCardAtTheTop(dealerPtr->pokerShoe);
	return cardAtTheTop;
}
