#include "dealer.h"

dealer *createDealer(int initialFunds) {
	int initialSize;
	initialSize = sizeof(dealer);

	shoe *newPokerShoe;
	newPokerShoe = createShuffledShoe();

	dealer *newDealer;
	newDealer = (dealer *) malloc (initialSize);

	playerHand *newDealersHand;
	newDealersHand = createPlayerHand();

	newDealer->funds = initialFunds;
	newDealer->pokerShoe = newPokerShoe;
	newDealer->dealersHand = newDealersHand;

	return newDealer;
}

void killDealer(dealer *dealerPtr){
	destroyShoe(dealerPtr->pokerShoe);
	removeHand(getSpecificHandDealer(dealerPtr));
	free (dealerPtr);
}

card *dealACard(const dealer *dealerPtr){
	card *cardAtTheTop = getCardAtTheTop(dealerPtr->pokerShoe);
	return cardAtTheTop;
}

/* static dealer *resizeDealer(dealer *dealerPtr) { */
/* 	//If there's still margin, we don't realloc memory */
/* 	if (dealerPtr->cardCapacity > dealerPtr->cardsInHand) { */
/* 		/1* printf("DIDN'T RESIZE\n"); *1/ */
/* 		return dealerPtr; */
/* 	} */

/* 	//We will downsize if it's way to big */
/* 	//                   8                             2      * 2 */
/* 	else if (dealerPtr->cardCapacity > dealerPtr->cardsInHand * 2) { */
/* 		printf("DOWNSIZE\n"); */

/* 		//Set the new capacity */
/* 		dealerPtr->cardCapacity /= 2; //Divided equal (fancy notation) */
/* 	} */

/* 	//We need more memory */
/* 	else { */
/* 		printf("UPSIZE\n"); */

/* 		//Set the new capacity */
/* 		dealerPtr->cardCapacity *= 2; //Divided equal (fancy notation) */
/* 	} */
		
/* 	dealer *resizedDealer; */
/* 	int newSize = sizeof(dealer) + */ 
/* 			dealerPtr->cardCapacity * sizeof(card *); */


/* 	resizedDealer = realloc(dealerPtr, newSize); */
/* 	return resizedDealer; */
/* } */

static playerHand *getDealersHand(dealer *dealerPtr) {
	return dealerPtr->dealersHand;
}

void dealDealersHand(dealer *dealerPtr, card *newCard) {
	playerHand *newDealersHand = getDealersHand(dealerPtr);
	newDealersHand = resizeHand(newDealersHand);

	newDealersHand->hand[newDealersHand->cardsInHand] = newCard;
	newDealersHand->cardsInHand += 1;

	dealerPtr->dealersHand = newDealersHand;

	/* playerHand *activePlayerHand = getSpecificHand(playerPtr, whichHand); */

	/* activePlayerHand = resizeHand(activePlayerHand); */
	/* playerPtr->playerHands[whichHand] = activePlayerHand; */

	/* activePlayerHand->hand[activePlayerHand->cardsInHand] = newCard; */
	/* activePlayerHand->cardsInHand += 1; */
	



/* 	dealer *resizedDealer = resizeDealer(dealerPtr); */

/* 	resizedDealer->hand[dealerPtr->cardsInHand] = newCard; */
/* 	resizedDealer->cardsInHand += 1; */

/* 	return resizedDealer; */
}

/* void removeDealersCards(dealer *dealerPtr) { */
/* 	//We simply move the index. The pointer will simply get removed by */
/* 	//the receiveCard function (it will overwrite the value) */
/* 	dealerPtr->cardsInHand = 0; */
/* } */

void takeMoney(dealer *dealerPtr, int money) {
	dealerPtr->funds += money;
}

void removeMoneyFromFunds(dealer *dealerPtr, int moneyToPlayer) {
	dealerPtr->funds -= moneyToPlayer;
}

playerHand *getSpecificHandDealer(dealer *dealersPtr) {
	return dealersPtr->dealersHand;
}
