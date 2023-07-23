#include "pokerTable.h"
#include "player.h"
#include "graphics.h"
#include <unistd.h>

#define INITIALCAPACITY 5
#define INITIALCARDCOUNT 2
#define CARDSUMBEFOREBUST 21
#define BLACKJACK 21
#define DEALERLIMIT 17
#define AMOUNTOFCARDSFORBLACKJACK 2
#define SLEEPAMOUNT 1.5

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

	/* printf("Kill player\n"); */
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

#define AMOUNTOFCARDSNEEDEDFORSPLIT 2
static inline bool checkForSplit(card *cards[], int amountOfCards) {
	bool areTheyTheSameRank;

	if (amountOfCards != AMOUNTOFCARDSNEEDEDFORSPLIT) {
		return false;
	}

	//This is a bit hacky, but you can only split 2 cards.
	//The previous if statement should discard all the non 2 card hands
	areTheyTheSameRank = (cards[0]->rank == cards[1]->rank);

	return areTheyTheSameRank;
}

#define AMOUNTOFCARDSNEEDEDFORDOUBLEDOWN 2
static inline bool checkForDoubleDown(int amountOfCards) {
	bool canDoubleDown;
	canDoubleDown = (amountOfCards == AMOUNTOFCARDSNEEDEDFORDOUBLEDOWN);

	return canDoubleDown;
}

#define ASKFORDECISION "What do you do? (H)it, (S)tand or (D)ouble Down "
#define NOSPLIT                                                        ": "
#define CANSPLIT                                                       "or S(P)lit: "
typedef enum playerDecision {Hit, Stand, DoubleDown, Split} playerDecision;
static inline playerDecision askForDecision(player *activePlayer) {
	bool canSplit;
	canSplit = checkForSplit(activePlayer->hand, activePlayer->cardsInHand);

	bool canDoubleDown;
	canDoubleDown = checkForDoubleDown(activePlayer->cardsInHand);

	char userInput[10];

	bool validAnswer;
	validAnswer = false;

	playerDecision playersDecision; 
	while (validAnswer == false) {
		//Not the prettiest way to handle it, but it works
		printf(ASKFORDECISION);
		if (canSplit == false) {
			printf(NOSPLIT);
		} 
		else {
			printf(CANSPLIT);
		}
		scanf("%s", userInput);

		if (strcmp(userInput, "H") == 0) {
			playersDecision = Hit;
			validAnswer = true;
		}
		else if (strcmp(userInput, "S") == 0) {
			playersDecision = Stand;
			validAnswer = true;
		}
		else if (canDoubleDown == true && strcmp(userInput, "D") == 0) {
			playersDecision = DoubleDown;
			validAnswer = true;
		}
		else if (canSplit == true && strcmp(userInput, "P") == 0) {
			playersDecision = Split;
			validAnswer = true;
		}
	}

	return playersDecision;
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

static inline void printDealersCards(dealer *pokerDealer, bool showAllCards) {
	int howManyCards;
	if (showAllCards == false) {
		howManyCards = 1;
	}
	else {
		howManyCards = pokerDealer->cardsInHand;
	}
	printf("DEALER: \n");
	asciiRepresentation(pokerDealer->hand, howManyCards);
}

static inline void printVisualRepresentation(player *activePlayer, dealer *pokerDealer, bool showAllDealerCards) {
	//TODO: Maybe change? It gets the job done

	printDealersCards(pokerDealer, showAllDealerCards);
	printf("%s:\n", activePlayer->name);
	asciiRepresentation(activePlayer->hand, activePlayer->cardsInHand);
}


#define ENDACTIVEPLAYERTURN saveCardSum(activePlayer, playersSum);\
			   return activePlayer;
static inline player *activePlayerTurn(player *activePlayer, dealer *pokerDealer) {
	bool playersTurnContinues;
	playersTurnContinues = true;

	playerDecision playersDecision;

	card *topCard;

	int playersSum;
	playersSum = sumCards(activePlayer->hand, activePlayer->cardsInHand);

	system("clear");
	printVisualRepresentation(activePlayer, pokerDealer, false);

	//If the player has Blackjack, then automatically skips input
	if (playersSum == BLACKJACK && activePlayer->cardsInHand == 2) {
		sleep(SLEEPAMOUNT);
		ENDACTIVEPLAYERTURN
	}

	//Main player turn loop
	while (playersTurnContinues == true && playersSum < CARDSUMBEFOREBUST) {
		system("clear");
		printVisualRepresentation(activePlayer, pokerDealer, false);

		//If the player busts, then he lost his turn
		if (playersSum > CARDSUMBEFOREBUST) {
			break;
		}

		playersDecision = askForDecision(activePlayer);
		switch (playersDecision) {
			case Hit:
				topCard = dealACard(pokerDealer);
				/* printf("\nNew card :%d, %d \n", topCard->rank, */ 
				/* 		topCard->suit); */

				//May need to re size a player struct
				activePlayer = receiveCard(activePlayer, topCard);
				break;
			case Stand:
				playersTurnContinues = false;
				break;
			case DoubleDown:
				int increaseAmount;
				increaseAmount = getBet(activePlayer);
				increaseBet(activePlayer, increaseAmount);
				//Once the bet is increased, the player can 
				//only get one more card, so we deal a card and 
				//exit the loop
				topCard = dealACard(pokerDealer);
				activePlayer = receiveCard(activePlayer, topCard);
				playersTurnContinues = false;
				break;
			case Split:
				break;
		}

		playersSum = sumCards(activePlayer->hand, activePlayer->cardsInHand);
	}
	system("clear");
	printVisualRepresentation(activePlayer, pokerDealer, false);

	sleep(SLEEPAMOUNT);

	ENDACTIVEPLAYERTURN
}
#undef ENDACTIVEPLAYERTURN

static inline void playersTurns(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		
		player *resizedPlayer;
		resizedPlayer = activePlayerTurn(activePlayer, pokerDealer);
		pokerTablePtr->players[i] = resizedPlayer;

	}
}

static inline dealer *dealersTurn(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	int dealersSum;
	dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
	/* printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[0]->rank, */ 
	/* 		pokerDealer->hand[0]->suit); */
	/* printf("\nNew dealers card :%d, %d \n", pokerDealer->hand[1]->rank, */ 
	/* 		pokerDealer->hand[1]->suit); */

	/* printf("DEALER: \n"); */
	/* asciiRepresentation(pokerDealer->hand, pokerDealer->cardsInHand); */
	system("clear");
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		printf("%s's sum: %d\n", pokerTablePtr->players[i]->name,
				pokerTablePtr->players[i]->cardSum);
	}
	printDealersCards(pokerDealer, true);
	sleep(SLEEPAMOUNT);

	while (dealersSum < DEALERLIMIT) {
		system("clear");
		for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
			printf("%s's sum: %d\n", pokerTablePtr->players[i]->name,
					pokerTablePtr->players[i]->cardSum);
		}
		card *topCard = dealACard(pokerDealer);
		/* printf("\nNew dealers card :%d, %d \n", topCard->rank, */ 
		/* 		topCard->suit); */

		//This operation may require us to resize the dealer struct
		pokerDealer = dealDealersHand(pokerDealer,  topCard);
		dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
		printDealersCards(pokerDealer, true);
		sleep(SLEEPAMOUNT);
		/* asciiRepresentation(pokerDealer->hand, pokerDealer->cardsInHand); */
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

		printf("%s: ", activePlayer->name);
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
	pokerDealer = dealersTurn(pokerTablePtr, pokerDealer);
	losersAndWiners(pokerTablePtr, pokerDealer);
}
