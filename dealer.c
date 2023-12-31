#include "dealer.h"

dealer *createDealer(int initialFunds) {
	int initialSize;
	initialSize = sizeof(dealer);

	shoe *newBlackjackShoe;
	newBlackjackShoe = createShuffledShoe();

	dealer *newDealer;
	newDealer = (dealer *) malloc (initialSize);

	playerHand *newDealersHand;
	newDealersHand = createPlayerHand();

	newDealer->funds = initialFunds;
	newDealer->blackjackShoe = newBlackjackShoe;
	newDealer->dealersHand = newDealersHand;

	return newDealer;
}

playerHand *getSpecificHandDealer(dealer *dealersPtr) {
	return dealersPtr->dealersHand;
}

void killDealer(dealer *dealerPtr){
	destroyShoe(dealerPtr->blackjackShoe);
	removeHand(getSpecificHandDealer(dealerPtr));
	free (dealerPtr);
}

card *dealACard(const dealer *dealerPtr){
	card *cardAtTheTop = getCardAtTheTop(dealerPtr->blackjackShoe);
	return cardAtTheTop;
}

static playerHand *getDealersHand(dealer *dealerPtr) {
	return dealerPtr->dealersHand;
}

void dealDealersHand(dealer *dealerPtr, card *newCard) {
	playerHand *newDealersHand = getDealersHand(dealerPtr);
	
	newDealersHand = receiveCard(newDealersHand, newCard);

	dealerPtr->dealersHand = newDealersHand;

	/* newDealersHand = resizeHand(newDealersHand); */

	/* newDealersHand->hand[newDealersHand->cardsInHand] = newCard; */
	/* newDealersHand->cardsInHand += 1; */

	/* dealerPtr->dealersHand = newDealersHand; */
}

void takeMoney(dealer *dealerPtr, int money) {
	dealerPtr->funds += money;
}

void removeMoneyFromFunds(dealer *dealerPtr, int moneyToPlayer) {
	dealerPtr->funds -= moneyToPlayer;
}

void resetDealer(dealer *dealersPtr) {
	playerHand *currentDealerHand;
	currentDealerHand = getSpecificHandDealer(dealersPtr);

	playerHand *resizeHand;
	resizeHand = resetHand(currentDealerHand);

	dealersPtr->dealersHand = resizeHand;
}
