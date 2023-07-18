#include "dealer.h"

#define INITIALDEALERCARDS 4

dealer *createDealer(int initialFunds) {
	int initialSize;
	initialSize = sizeof(dealer) + INITIALDEALERCARDS * sizeof(card *);

	shoe *newPokerShoe;
	newPokerShoe = createShuffledShoe();

	dealer *newDealer;
	newDealer = (dealer *) malloc (initialSize);

	newDealer->funds = initialFunds;
	newDealer->pokerShoe = newPokerShoe;
	newDealer->cardsInHand = 0;
	newDealer->cardCapacity = INITIALDEALERCARDS;
	newDealer->cardSum = 0;

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

static dealer *resizeDealer(dealer *dealerPtr) {
	//If there's still margin, we don't realloc memory
	if (dealerPtr->cardCapacity > dealerPtr->cardsInHand) {
		/* printf("DIDN'T RESIZE\n"); */
		return dealerPtr;
	}

	//We will downsize if it's way to big
	//                   8                             2      * 2
	else if (dealerPtr->cardCapacity > dealerPtr->cardsInHand * 2) {
		printf("DOWNSIZE\n");

		//Set the new capacity
		dealerPtr->cardCapacity /= 2; //Divided equal (fancy notation)
	}

	//We need more memory
	else {
		printf("UPSIZE\n");

		//Set the new capacity
		dealerPtr->cardCapacity *= 2; //Divided equal (fancy notation)
	}
		
	dealer *resizedDealer;
	int newSize = sizeof(dealer) + 
			dealerPtr->cardCapacity * sizeof(card *);


	resizedDealer = realloc(dealerPtr, newSize);
	return resizedDealer;
}



dealer *dealDealersHand(dealer *dealerPtr, card *newCard) {
	dealer *resizedDealer = resizeDealer(dealerPtr);

	resizedDealer->hand[dealerPtr->cardsInHand] = newCard;
	resizedDealer->cardsInHand += 1;

	return resizedDealer;
	/* printf("%d \n", dealerPtr->cardsInHand); */
}

void removeDealersCards(dealer *dealerPtr) {
	//We simply move the index. The pointer will simply get removed by
	//the receiveCard function (it will overwrite the value)
	dealerPtr->cardsInHand = 0;
}

void takeMoney(dealer *dealerPtr, int money) {
	dealerPtr->funds += money;
}

void removeMoneyFromFunds(dealer *dealerPtr, int moneyToPlayer) {
	dealerPtr->funds -= moneyToPlayer;
}