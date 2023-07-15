#include "pokerTable.h"
#include "player.h"

#define INITIALCAPACITY 5
#define INITIALCARDCOUNT 2

pokerTable *createPokerTable(int initialFunds) {
	dealer *dealerPtr = createDealer(initialFunds);

	pokerTable *newPokerTable;	
	newPokerTable =  (pokerTable *) malloc(sizeof(pokerTable) +
			INITIALCAPACITY * sizeof(player *));

	newPokerTable->pokerDealer = dealerPtr;
	newPokerTable->playerCapacity = INITIALCAPACITY;
	newPokerTable->playerAmount = 0;

	return newPokerTable;
}

void destroyPokerTable(pokerTable *pokerTablePtr) {
	killDealer(pokerTablePtr->pokerDealer);

	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		killPlayer(pokerTablePtr->players[i]);
	}
	free(pokerTablePtr);
}

pokerTable *addPlayer(pokerTable *pokerTablePtr, char name[], int initialFunds) {
	/* printf("ADDING PLAYER TO POKER TABLE \n"); */

	player *newPlayer = createPlayer(name,  initialFunds);

	pokerTablePtr->players[pokerTablePtr->playerAmount] = newPlayer;
	pokerTablePtr->playerAmount += 1;

	//TODO: Add resize down function
	if (pokerTablePtr->playerAmount == pokerTablePtr->playerCapacity) {
		printf("REALLOC\n");
		/* int newSize = pokerTablePtr->playerCapacity * 2; */
		int newSize = sizeof(pokerTable) +
			 pokerTablePtr->playerCapacity * 2 * sizeof(player *);
		pokerTablePtr = realloc(pokerTablePtr, newSize);
	}
	
	return pokerTablePtr;
}

static inline void dealInitialCards(pokerTable *pokerTablePtr, int initialCard, dealer *pokerDealer) {
	/* dealer *pokerDealer = pokerTablePtr->pokerDealer; */
	for (int i = 0; i < initialCard; i++) {
		for (int j = 0; j < pokerTablePtr->playerAmount; j++) {
			player *activePlayer = pokerTablePtr->players[j];

			card *topCard = dealACard(pokerDealer);

			receiveCard(activePlayer,  topCard);
		}
		card *dealersCard = dealACard(pokerDealer);

		/* printf("Dealer's card: "); */
		/* printf("%d %d\n", dealersCard->rank, dealersCard->suit); */
		dealDealersHand(pokerDealer, dealersCard);
	}
}

static inline void playersTurns(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	printf("Dealer's card: ");
	printf("%d %d\n", pokerDealer->hand[0]->rank, pokerDealer->hand[0]->suit);
	printf("Dealer's card: ");
	printf("%d %d\n", pokerDealer->hand[1]->rank, pokerDealer->hand[1]->suit);


	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		
		bool wantsNewCard;
		wantsNewCar = dealNewCardTo(activePlayer);
	}
}

void pokerRound(pokerTable *pokerTablePtr) {
	dealer *pokerDealer = pokerTablePtr->pokerDealer;

	dealInitialCards(pokerTablePtr, INITIALCARDCOUNT, pokerDealer);
	playersTurns(pokerTablePtr, pokerDealer);
}
