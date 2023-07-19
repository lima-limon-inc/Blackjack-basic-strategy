#include "pokerTable.h"
#include "player.h"

#define INITIALCAPACITY 5
#define INITIALCARDCOUNT 2
#define CARDSUMBEFOREBUST 21
#define BLACKJACK 21
#define DEALERLIMIT 17
#define AMOUNTOFCARDSFORBLACKJACK 2

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

	printf("Kill player\n");
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

static inline player *activePlayerTurn(player *activePlayer, dealer *pokerDealer) {
	int playersSum;
	playersSum = 0;

	//It may not need to resize the player struct
	/* player *resizedPlayer; */
	/* resizedPlayer = activePlayer; */

	bool wantsNewCard;
	wantsNewCard = true;

	//Main player turn loop
	while (true) {// && wantsNewCard == true) {
		printCards(activePlayer);
		printf("%d\n", activePlayer->cardsInHand);
		asciiRepresentation(activePlayer->hand, activePlayer->cardsInHand);

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

		//May need to re size a player struct
		activePlayer = receiveCard(activePlayer,  topCard);

	}
	activePlayer-> cardSum = playersSum;

	return activePlayer;
}

static inline void playersTurns(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		
		player *resizedPlayer;
		resizedPlayer = activePlayerTurn(activePlayer, pokerDealer);
		pokerTablePtr->players[i] = resizedPlayer;

	}
}

static inline dealer *dealersTurn(dealer *pokerDealer) {
	int dealersSum;
	dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
	printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[0]->rank, 
			pokerDealer->hand[0]->suit);
	printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[1]->rank, 
			pokerDealer->hand[1]->suit);

	while (dealersSum < DEALERLIMIT) {
		card *topCard = dealACard(pokerDealer);
		printf("\nNew dealers card :%d, %d \n", topCard->rank, 
				topCard->suit);

		//This operation may require us to resize the dealer struct
		pokerDealer = dealDealersHand(pokerDealer,  topCard);
		dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
	}

	pokerDealer->cardSum = dealersSum;
	printf("\nDealers sum :%d\n", dealersSum);
	return pokerDealer;

}

typedef enum playerRoundResult {Win, Lost, Tie, Blackjack} playerRoundResult;
static inline playerRoundResult roundEndedIn(player *activePlayer, dealer *dealerPtr) {
	int playersSum;
	playersSum = activePlayer->cardSum;

	int dealersSum;
	dealersSum = dealerPtr->cardSum;

	//Player lost, he busted
	if (playersSum > CARDSUMBEFOREBUST) {
		return Lost;
	}

	if (playersSum == BLACKJACK && activePlayer->cardsInHand == AMOUNTOFCARDSFORBLACKJACK) {
		return Blackjack;
	}

	//The dealer bust, in this case,  all the non busted player win
	else if (dealersSum > CARDSUMBEFOREBUST) {
		return Win;
	}

	//Player has better cards
	else if (playersSum > dealersSum) {
			return Win;
		}

	//Equal cards
	else if (playersSum == dealersSum) {
		return Tie;
	}

	//The dealer has better cards
	else if (dealersSum > playersSum) {
		return Lost;
	}

	else {
		printf("NOT CONSIDERED CASE");
		abort();
		return 1;
	}
}

static inline void losersAndWiners(pokerTable *pokerTablePtr, dealer *dealerPtr) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];

		playerRoundResult playerResult;
		playerResult = roundEndedIn(activePlayer, dealerPtr);

		printf("%s\n", activePlayer->name);
		switch (playerResult) {
			case Lost:
				printf("Lost\n");
				int lostMoney;
				lostMoney = loseBet(activePlayer);
				takeMoney(dealerPtr, lostMoney);
				break;
			case Win:
				printf("Win\n");
				int wonMoney;
				wonMoney = getBet(activePlayer);
				wonMoney *= 2;
				removeMoneyFromFunds(dealerPtr, wonMoney);
				winBet(activePlayer, wonMoney);
				break;
			case Tie:
				printf("Tie\n");
				int tieMoney;
				tieMoney = getBet(activePlayer);
				takeMoney(dealerPtr, tieMoney);
				winBet(activePlayer, tieMoney);
				break;
			case Blackjack:
				printf("Blackjack\n");
				int BlackjackMoneyu;
				BlackjackMoneyu = getBet(activePlayer);
				BlackjackMoneyu *= 3;
				removeMoneyFromFunds(dealerPtr, BlackjackMoneyu);
				winBet(activePlayer, BlackjackMoneyu);
				break;
		}
	}
}

void pokerRound(pokerTable *pokerTablePtr) {
	dealer *pokerDealer = pokerTablePtr->pokerDealer;

	asksPlayerForBet(pokerTablePtr);
	dealInitialCards(pokerTablePtr, INITIALCARDCOUNT, pokerDealer);
	playersTurns(pokerTablePtr, pokerDealer);
	pokerDealer = dealersTurn(pokerDealer);
	losersAndWiners(pokerTablePtr, pokerDealer);
}
