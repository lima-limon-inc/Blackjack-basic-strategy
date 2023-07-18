#include "pokerTable.h"
#include "player.h"

#define INITIALCAPACITY 5
#define INITIALCARDCOUNT 2
#define CARDSUMBEFOREBUST 21
#define DEALERLIMIT 17

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

static inline bool askForNewCard() {
	printf("Want another card? Y/N");
	char userInput[10];
	scanf("%s", userInput);


	bool wantsAnotherCard; 
	if (strcmp(userInput, "Y") == 0) {
		wantsAnotherCard = true;
		/* printf("QUIERO"); */
	}
	else {
		wantsAnotherCard = false;
	}

	return wantsAnotherCard;
}

static inline void askPlayerForBet(player *activePlayer) {
	printf("%s, what's your bet?\n", activePlayer->name); 

	int playerBet;
	scanf("%d", &playerBet);

	makeABet(activePlayer, playerBet);
}

static inline void asksPlayerForBet(pokerTable *pokerTablePtr) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		askPlayerForBet(activePlayer);

	}
}

static inline void activePlayerTurn(player *activePlayer, dealer *pokerDealer) {
	int playersSum;
	playersSum = 0;

	/* askPlayerForBet(activePlayer); */

	bool wantsNewCard;
	wantsNewCard = true;

	//Main player turn loop
	while (true) {// && wantsNewCard == true) {
		printCards(activePlayer);

		playersSum = sumCards(activePlayer->hand, activePlayer->cardsInHand);
		printf("Player's sum: %d\n", playersSum);
		//If the player busts, then he lost his turn
		if (playersSum > CARDSUMBEFOREBUST) {
			break;
		}

		wantsNewCard = askForNewCard(activePlayer);
		//If the player does not want a new card, then we want to exit
		//the while loop immediately.
		if (wantsNewCard == false) {
			break;
		}

		card *topCard = dealACard(pokerDealer);
		printf("\nNew card :%d, %d \n", topCard->rank, 
				topCard->suit);

		receiveCard(activePlayer,  topCard);

	}
}

static inline void playersTurns(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		activePlayerTurn(activePlayer, pokerDealer);

	}
}

static inline void dealersTurn(dealer *pokerDealer) {
	int dealersSum;
	dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
	printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[0]->rank, 
			pokerDealer->hand[0]->rank);
	printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[1]->rank, 
			pokerDealer->hand[1]->rank);

	while (dealersSum < DEALERLIMIT) {
		card *topCard = dealACard(pokerDealer);
		printf("\nNew dealers card :%d, %d \n", topCard->rank, 
				topCard->suit);

		//This operation may require us to resize the dealer struct
		pokerDealer = dealDealersHand(pokerDealer,  topCard);
		dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
	}

}

void pokerRound(pokerTable *pokerTablePtr) {
	dealer *pokerDealer = pokerTablePtr->pokerDealer;

	asksPlayerForBet(pokerTablePtr);
	dealInitialCards(pokerTablePtr, INITIALCARDCOUNT, pokerDealer);
	playersTurns(pokerTablePtr, pokerDealer);
	dealersTurn(pokerDealer);
}
