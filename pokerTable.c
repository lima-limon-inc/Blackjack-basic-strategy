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

	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		killPlayer(pokerTablePtr->players[i]);
	}
	free(pokerTablePtr);
}

pokerTable *addPlayer(pokerTable *pokerTablePtr, char name[], int initialFunds) {
	player *newPlayer = createPlayer(name,  initialFunds);

	pokerTablePtr->players[pokerTablePtr->playerAmount] = newPlayer;
	pokerTablePtr->playerAmount += 1;

	//TODO: Add resize down function
	if (pokerTablePtr->playerAmount == pokerTablePtr->playerCapacity) {
		printf("REALLOC\n");
		int newSize = sizeof(pokerTable) +
			 pokerTablePtr->playerCapacity * 2 * sizeof(player *);
		pokerTablePtr = realloc(pokerTablePtr, newSize);
	}
	
	return pokerTablePtr;
}

static player *getPlayerAtPosition(pokerTable *pokerTablePtr, int position) {
	return pokerTablePtr->players[position];
}

static inline void dealInitialCards(pokerTable *pokerTablePtr, int initialCard, dealer *pokerDealer) {
	for (int i = 0; i < initialCard; i++) {
		for (int j = 0; j < pokerTablePtr->playerAmount; j++) {
			player *activePlayer = pokerTablePtr->players[j];

			card *topCard = dealACard(pokerDealer);

			//0 Because it's the initial hand
			receiveCard(activePlayer,  topCard, 0);
		}
		card *dealersCard = dealACard(pokerDealer);

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

#define FULLINPUTMESSAGE "What do you do? (H)it, (S)tand or (D)ouble Down or S(P)lit: "
/* #define ASKFORDECISION "What do you do? (H)it, (S)tand or (D)ouble Down" */
/* #define NOSPLIT                                                        ": " */
/* #define CANSPLIT                                                       " */
static inline char *askForPlayerMessage(bool canDoubleDown, bool canSplit) {
	char *message = malloc(sizeof(FULLINPUTMESSAGE));
	
	strcpy(message, "What do you do? (H)it, (S)tand");

	//IF you can't double down, then you can't split.
	//Both are only possible on the first hand
	if (canDoubleDown == false) {
		strcat(message, ": ");
		return message;
	}
	char doubleDownConnector[sizeof(" or (D)ouble Down or S(P)lit: ")];
	//If you can't split, then you can only double down
	if (canSplit == false) {
		strcpy(doubleDownConnector, " or (D)ouble Down: ");
	}
	else {
		strcpy(doubleDownConnector, ", (D)ouble Down or S(P)lit: ");
	}
	strcat(message, doubleDownConnector);

	return message;
}

typedef enum playerDecision {Hit, Stand, DoubleDown, Split} playerDecision;
static inline playerDecision askForDecision(playerHand *activePlayerHand) {
	bool canSplit;
	canSplit = checkForSplit(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));
	/* canSplit = checkForSplit(activePlayer->hand, activePlayer->cardsInHand); */

	bool canDoubleDown;
	canDoubleDown = checkForDoubleDown(getAmountOfCardsInHand(activePlayerHand));
	/* canDoubleDown = checkForDoubleDown(activePlayer->cardsInHand); */

	char userInput[10];

	bool validAnswer;
	validAnswer = false;

	playerDecision playersDecision; 
	while (validAnswer == false) {
		//Not the prettiest way to handle it, but it works
		char *askForDecisionMessage;
		askForDecisionMessage = askForPlayerMessage(canDoubleDown, canSplit);
		printf(askForDecisionMessage);
		free(askForDecisionMessage);
		
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

static inline void askPlayerForBet(player *activePlayer, int whichHand) {
	printf("%s, what's your bet?\n", activePlayer->name); 

	int playerBet;
	scanf("%d", &playerBet);

	makeABet(activePlayer, playerBet, whichHand);
}

static inline void asksPlayerForBet(pokerTable *pokerTablePtr) {
	/* for (int i = 0; i < pokerTablePtr->playerAmount; i++) { */
	for (int position = 0; position < pokerTablePtr->playerAmount; position++) {
		/* activePlayer = pokerTablePtr->players[i]; */
		player *activePlayer;
		activePlayer = getPlayerAtPosition(pokerTablePtr, position);
		/* for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) { */
			//                   0 because it's the initial dealing
		askPlayerForBet(activePlayer, 0);
		/* } */
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

static inline void printVisualRepresentation(player *activePlayer, int whichHand, dealer *pokerDealer, bool showAllDealerCards) {
	playerHand *activePlayerHand;
	activePlayerHand = getSpecificHand(activePlayer, whichHand);

	printDealersCards(pokerDealer, showAllDealerCards);
	printf("%s:\n", activePlayer->name);
	asciiRepresentation(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));
}


//TODO: Remove macro and simply write text
#define ENDACTIVEPLAYERTURN saveCardSum(activePlayerHand, playersSum); \
			   return;
static inline void activePlayerTurn(player *activePlayer, dealer *pokerDealer) {
	playerDecision playersDecision;

	card *topCard;

	int totalNumberOfHands;
	totalNumberOfHands = getNumberOfHands(activePlayer);

	int playersSum;

	playerHand *activePlayerHand;

	bool firstHand;

	for (int currentHand = 0; currentHand < totalNumberOfHands; currentHand++) {
		bool playersTurnContinues;
		playersTurnContinues = true;

		activePlayerHand = getSpecificHand(activePlayer, currentHand);
		//                                                  0 because it's the 
		//                                                  initial dealing
		playersSum = sumCards(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));

		//You can only get a Blackjack in you first hand when you
		//are originally dealt cards
		if (firstHand == true && playersSum == BLACKJACK) {
			system("clear");
			printVisualRepresentation(activePlayer, 0, pokerDealer, false);
			sleep(SLEEPAMOUNT);
			/* printf("TEST"); */
			ENDACTIVEPLAYERTURN
		}

		//Main player turn loop
		while (playersTurnContinues == true && playersSum < CARDSUMBEFOREBUST) {
			system("clear");
			printVisualRepresentation(activePlayer, currentHand, pokerDealer, false);

			//If the player busts, then he lost his turn
			if (playersSum > CARDSUMBEFOREBUST) {
				break;
			}

			playersDecision = askForDecision(activePlayerHand);
			switch (playersDecision) {
				case Hit:
					topCard = dealACard(pokerDealer);

					receiveCard(activePlayer, topCard, currentHand);
					break;
				case Stand:
					playersTurnContinues = false;
					break;
				case DoubleDown:
					int increaseAmount;
					increaseAmount = getBet(activePlayer, currentHand);
					increaseBet(activePlayer, increaseAmount, currentHand);
					//Once the bet is increased, the player can 
					//only get one more card, so we deal a card and 
					//exit the loop
					topCard = dealACard(pokerDealer);
					receiveCard(activePlayer, topCard, currentHand);
					playersTurnContinues = false;
					break;
				case Split:
					break;
			}

			activePlayerHand = getSpecificHand(activePlayer, currentHand);
			playersSum = sumCards(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));
		}
	//This will over write the value everytime. This is fine (I think)
	firstHand = false;

	system("clear");
	printVisualRepresentation(activePlayer, currentHand, pokerDealer, false);
	}

	sleep(SLEEPAMOUNT);

	ENDACTIVEPLAYERTURN
}
#undef ENDACTIVEPLAYERTURN

static inline void playersTurns(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];
		
		activePlayerTurn(activePlayer, pokerDealer);
	}
}

static inline dealer *dealersTurn(pokerTable *pokerTablePtr, dealer *pokerDealer) {
	int dealersSum;
	dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);

	system("clear");
	for (int position = 0; position < pokerTablePtr->playerAmount; position++) {
		player *activePlayer;
		activePlayer = getPlayerAtPosition(pokerTablePtr, position);

		for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {

			playerHand *activePlayerHand;
			activePlayerHand = getSpecificHand(activePlayer, currentHand);
			printf("%s's sum: %d\n", activePlayer->name,
			      			getHandSum(activePlayerHand));
		}
	}
	printDealersCards(pokerDealer, true);
	sleep(SLEEPAMOUNT);

	while (dealersSum < DEALERLIMIT) {
		system("clear");
		for (int position = 0; position < pokerTablePtr->playerAmount; position++) {
			player *activePlayer;
			activePlayer = getPlayerAtPosition(pokerTablePtr, position);

			for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {

				playerHand *activePlayerHand;
				activePlayerHand = getSpecificHand(activePlayer, currentHand);
				printf("%s's sum: %d\n", activePlayer->name,
							getHandSum(activePlayerHand));
			}
		}
		card *topCard = dealACard(pokerDealer);

		//This operation may require us to resize the dealer struct
		pokerDealer = dealDealersHand(pokerDealer,  topCard);
		dealersSum = sumCards(pokerDealer->hand, pokerDealer->cardsInHand);
		printDealersCards(pokerDealer, true);
		sleep(SLEEPAMOUNT);
	}

	pokerDealer->cardSum = dealersSum;
	printf("\nDealers sum :%d\n", dealersSum);
	return pokerDealer;

}

typedef enum playerRoundResult {Win, Lost, Tie, Blackjack} playerRoundResult;
static inline playerRoundResult roundEndedIn(playerHand *activeHand, dealer *dealerPtr) {
	int playersSum;
	playersSum = getHandSum(activeHand);
	/* playersSum = activePlayer->cardSum; */

	int dealersSum;
	dealersSum = dealerPtr->cardSum;

	int playersAmountOfCards;
	playersAmountOfCards = getAmountOfCardsInHand(activeHand);

	//Player lost, he busted
	if (playersSum > CARDSUMBEFOREBUST) {
		return Lost;
	}

	if (playersSum == BLACKJACK && playersAmountOfCards == AMOUNTOFCARDSFORBLACKJACK) {
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

	printf("NOT CONSIDERED CASE");
	abort();
	return Lost;
}

static inline void losersAndWiners(pokerTable *pokerTablePtr, dealer *dealerPtr) {
	for (int i = 0; i < pokerTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = pokerTablePtr->players[i];

		for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {
			playerHand *playersHand;
			playersHand = getSpecificHand(activePlayer, currentHand);

			playerRoundResult playerResult;
			playerResult = roundEndedIn(playersHand, dealerPtr);

			printf("%s: ", activePlayer->name);
			switch (playerResult) {
				case Lost:
					printf("Lost\n");
					int lostMoney;
					lostMoney = loseBet(activePlayer, currentHand);
					takeMoney(dealerPtr, lostMoney);
					break;
				case Win:
					printf("Win\n");
					int wonMoney;
					wonMoney = getBet(activePlayer, currentHand);
					wonMoney *= 2;
					removeMoneyFromFunds(dealerPtr, wonMoney);
					winBet(activePlayer, wonMoney, currentHand);
					break;
				case Tie:
					printf("Tie\n");
					int tieMoney;
					tieMoney = getBet(activePlayer, currentHand);
					takeMoney(dealerPtr, tieMoney);
					winBet(activePlayer, tieMoney, currentHand);
					break;
				case Blackjack:
					printf("Blackjack\n");
					int BlackjackMoney;
					BlackjackMoney = getBet(activePlayer, currentHand);
					BlackjackMoney *= 3;
					removeMoneyFromFunds(dealerPtr, BlackjackMoney);
					winBet(activePlayer, BlackjackMoney, currentHand);
					break;
			}
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
