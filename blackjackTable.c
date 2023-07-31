#include <unistd.h>

#include "blackjackTable.h"

#include "player.h"
#include "graphics.h"
#include "playerDecision.h"
#include "blackjackRules.h"
#include "basicStrategy.h"

#define MAXAMOUNTOFSPLITS 4
#define INITIALCAPACITY 5
#define INITIALCARDCOUNT 2
#define CARDSUMBEFOREBUST 21
#define BLACKJACK 21
#define DEALERLIMIT 17
#define AMOUNTOFCARDSFORBLACKJACK 2
#define SLEEPAMOUNT 1.5

blackjackTable *createBlackjackTable(int initialFunds) {
	dealer *dealerPtr = createDealer(initialFunds);

	blackjackTable *newBlackjackTable;	
	newBlackjackTable =  (blackjackTable *) malloc(sizeof(blackjackTable) +
			INITIALCAPACITY * sizeof(player *));

	newBlackjackTable->blackjackDealer = dealerPtr;
	newBlackjackTable->playerCapacity = INITIALCAPACITY;
	newBlackjackTable->playerAmount = 0;

	return newBlackjackTable;
}

void destroyBlackjackTable(blackjackTable *blackjackTablePtr) {
	killDealer(blackjackTablePtr->blackjackDealer);

	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		killPlayer(blackjackTablePtr->players[i]);
	}
	free(blackjackTablePtr);
}

blackjackTable *addPlayer(blackjackTable *blackjackTablePtr, char name[], int initialFunds) {
	player *newPlayer = createPlayer(name,  initialFunds);

	blackjackTablePtr->players[blackjackTablePtr->playerAmount] = newPlayer;
	blackjackTablePtr->playerAmount += 1;

	//TODO: Add resize down function
	if (blackjackTablePtr->playerAmount == blackjackTablePtr->playerCapacity) {
		printf("REALLOC\n");
		blackjackTablePtr->playerCapacity *= 2;
		int newSize = sizeof(blackjackTable) +
			 blackjackTablePtr->playerCapacity * sizeof(player *);
		blackjackTablePtr = realloc(blackjackTablePtr, newSize);
	}
	
	return blackjackTablePtr;
}

static player *getPlayerAtPosition(blackjackTable *blackjackTablePtr, int position) {
	return blackjackTablePtr->players[position];
}

static inline void dealInitialCards(blackjackTable *blackjackTablePtr, int initialCard, dealer *blackjackDealer) {
	for (int i = 0; i < initialCard; i++) {
		for (int j = 0; j < blackjackTablePtr->playerAmount; j++) {
			player *activePlayer = blackjackTablePtr->players[j];

			card *topCard = dealACard(blackjackDealer);

			//0 Because it's the initial hand
			receiveCard(activePlayer,  topCard, 0);
		}
		card *dealersCard = dealACard(blackjackDealer);

		dealDealersHand(blackjackDealer, dealersCard);
	}
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

static inline playerDecision askForDecision(playerHand *activePlayerHand, int howManyHands) {
	bool canSplit;
	if (howManyHands < MAXAMOUNTOFSPLITS) {
		canSplit = checkForSplit(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));
	}
	else {
		//If the player has exceeded the amount of valid splits, then
		//we don't even bother calculating it
		canSplit = false;
	}

	bool canDoubleDown;
	canDoubleDown = checkForDoubleDown(getAmountOfCardsInHand(activePlayerHand));

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

	int playerBet;

	bool canMakeBet;
	canMakeBet = false;

	while (canMakeBet == false) {
		printf("%s, what's your bet?\n", activePlayer->name); 
		scanf("%d", &playerBet);

		if (playerBet <= 0) {
			printf("Bet's can't be negative or 0\n");
			continue;
		}

		canMakeBet = canMakeABet(activePlayer, playerBet);
		
		if (canMakeBet == false) {
			printf("You don't have enough funds\n");
		}
		
	}


	makeABet(activePlayer, playerBet, whichHand);
}

static inline void asksPlayerForBet(blackjackTable *blackjackTablePtr) {
	for (int position = 0; position < blackjackTablePtr->playerAmount; position++) {
		player *activePlayer;
		activePlayer = getPlayerAtPosition(blackjackTablePtr, position);

		askPlayerForBet(activePlayer, 0);
	}
}

static dealer *getDealer(blackjackTable *blackjackTablePtr) {
	return blackjackTablePtr->blackjackDealer;
}

static void printDealersCards(dealer *blackjackDealer, bool showAllCards) {
	int howManyCards;

	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(blackjackDealer);

	bool lastCardBlank;
	if (showAllCards == false) {
		howManyCards = 1;
		lastCardBlank =  true;
	}
	else {
		howManyCards = getAmountOfCardsInHand(dealersHand);//blackjackDealer->cardsInHand;
		lastCardBlank =  false;
	}
	printf("DEALER: \n");
	asciiRepresentation(getCards(dealersHand), howManyCards, lastCardBlank);
}

static inline char *cardinalToOrdinal(int cardinal) {
	switch (cardinal) {
		case 1:
			return "first";
		case 2:
			return "second";
		case 3:
			return "third";
		case 4:
			return "fourth";
		default:
			return "nth";
	}
}

static void printPlayerAndHand(player *activePlayer, int whichHand) {
	printf("%s's ", activePlayer->name);
	if (getNumberOfHands(activePlayer) > 1) {
		//We add 1 because the first hand is stored in position 0,
		//the second hand in position 1 and so on
		printf("%s ", cardinalToOrdinal(whichHand + 1));
	}
	printf("hand");
}

static void printVisualRepresentation(player *activePlayer, int whichHand, dealer *blackjackDealer, bool showAllDealerCards) {
	playerHand *activePlayerHand;
	activePlayerHand = getSpecificHand(activePlayer, whichHand);

	printDealersCards(blackjackDealer, showAllDealerCards);

	printPlayerAndHand(activePlayer, whichHand);
	printf("\n");

	asciiRepresentation(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand), false);
}

static inline bool processPlayerMove(player *activePlayer, int currentHand, playerDecision playersDecision, dealer *blackjackDealer) {
	card *topCard;

	bool playersTurnContinues;
	switch (playersDecision) {
		case Hit:
			topCard = dealACard(blackjackDealer);

			receiveCard(activePlayer, topCard, currentHand);
			playersTurnContinues = true;
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
			topCard = dealACard(blackjackDealer);
			receiveCard(activePlayer, topCard, currentHand);
			playersTurnContinues = false;
			break;
		case Split:
			splitCards(activePlayer, currentHand);
			//We deal an addtional to the newly
			//created hand
			topCard = dealACard(blackjackDealer);
			receiveCard(activePlayer, topCard, currentHand);

			topCard = dealACard(blackjackDealer);
			receiveCard(activePlayer, topCard, getNumberOfHands(activePlayer) - 1);
			playersTurnContinues = true;
			break;
	}

	return playersTurnContinues;
}


static inline void activePlayerTurn(player *activePlayer, dealer *blackjackDealer) {
	playerDecision playersDecision;

	int playersSum; playerHand *activePlayerHand;

	bool firstHand;

	for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {
		bool playersTurnContinues;
		playersTurnContinues = true;

		activePlayerHand = getSpecificHand(activePlayer, currentHand);
		playersSum = sumCards(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));

		//You can only get a Blackjack in you first hand when you
		//are originally dealt cards
		if (firstHand == true && playersSum == BLACKJACK) {
			system("clear");
			printVisualRepresentation(activePlayer, 0, blackjackDealer, false);
			sleep(SLEEPAMOUNT);
			saveCardSum(activePlayerHand, playersSum);
			return;
		}

		//Main player turn loop
		while (playersTurnContinues == true && playersSum < CARDSUMBEFOREBUST) {
			system("clear");
			printVisualRepresentation(activePlayer, currentHand, blackjackDealer, false);

			//If the player busts, then he lost his turn
			if (playersSum > CARDSUMBEFOREBUST) {
				break;
			}

			playersDecision = askForDecision(activePlayerHand, getNumberOfHands(activePlayer));

			//This bit is horrible. I hate it so much. Will refactor
			//eventually
			/*                         ||
			 *                         ||
			 *                         \/
			 */
			playerDecision correctDecision;
			correctDecision = getCorrectChoice(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand), getCards(getSpecificHandDealer(blackjackDealer))[0]);
			bool isItCorrectChoice;
			isItCorrectChoice = (correctDecision == playersDecision);
			/*                         /\
			 *                         ||
			 *                         ||
			 */

			playersTurnContinues = processPlayerMove(activePlayer, currentHand, playersDecision, blackjackDealer);

			printCorrectOrNot(isItCorrectChoice, correctDecision);
			sleep(SLEEPAMOUNT);


			activePlayerHand = getSpecificHand(activePlayer, currentHand);
			playersSum = sumCards(getCards(activePlayerHand), getAmountOfCardsInHand(activePlayerHand));
		}
	//This will over write the value everytime. This is fine (I think)
	firstHand = false;

	system("clear");
	printVisualRepresentation(activePlayer, currentHand, blackjackDealer, false);
	saveCardSum(activePlayerHand, playersSum);
	sleep(SLEEPAMOUNT);
	}

	return;
}

static inline void playersTurns(blackjackTable *blackjackTablePtr, dealer *blackjackDealer) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];
		
		activePlayerTurn(activePlayer, blackjackDealer);
	}
}

static inline void dealersTurn(blackjackTable *blackjackTablePtr, dealer *blackjackDealer) {
	int dealersSum;
	
	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(blackjackDealer);

	int howManyCardsDealer;
	howManyCardsDealer= getAmountOfCardsInHand(dealersHand);

	dealersSum = sumCards(getCards(dealersHand), howManyCardsDealer);


	//TODO: Remove this bit, IDK why I wrote it twice
	system("clear");
	for (int position = 0; position < blackjackTablePtr->playerAmount; position++) {
		player *activePlayer;
		activePlayer = getPlayerAtPosition(blackjackTablePtr, position);

		for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {

			playerHand *activePlayerHand;
			activePlayerHand = getSpecificHand(activePlayer, currentHand);
			printPlayerAndHand(activePlayer, currentHand);
			printf(" sum: %d\n", getHandSum(activePlayerHand));
		}
	}
	printDealersCards(blackjackDealer, true);
	sleep(SLEEPAMOUNT);

	while (dealersSum < DEALERLIMIT) {
		system("clear");
		for (int position = 0; position < blackjackTablePtr->playerAmount; position++) {
			player *activePlayer;
			activePlayer = getPlayerAtPosition(blackjackTablePtr, position);

			for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {

				playerHand *activePlayerHand;
				activePlayerHand = getSpecificHand(activePlayer, currentHand);
				/* printf("%s's sum: %d\n", activePlayer->name, */
				/* 			getHandSum(activePlayerHand)); */
				printPlayerAndHand(activePlayer, currentHand);
				printf(" sum: %d\n", getHandSum(activePlayerHand));
			}
		}
		card *topCard = dealACard(blackjackDealer);

		dealDealersHand(blackjackDealer,  topCard);
		//dealersHand changed from its initical call to here, due to
		//the dealDealersHand function, that's why we have to
		//redefine it
		dealersHand = getSpecificHandDealer(blackjackDealer);
		howManyCardsDealer= getAmountOfCardsInHand(dealersHand);

		dealersSum = sumCards(getCards(dealersHand), howManyCardsDealer);
		printDealersCards(blackjackDealer, true);
		sleep(SLEEPAMOUNT);
	}

	saveCardSum(dealersHand, dealersSum);
	printf("\nDealers sum :%d\n", dealersSum);
}

typedef enum playerRoundResult {Win, Lost, Tie, Blackjack} playerRoundResult;
static inline playerRoundResult roundEndedIn(playerHand *activeHand, dealer *dealerPtr) {
	int playersSum;
	playersSum = getHandSum(activeHand);

	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(dealerPtr);

	int dealersSum;
	dealersSum = getHandSum(dealersHand);

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

static inline void losersAndWiners(blackjackTable *blackjackTablePtr, dealer *dealerPtr) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];

		for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {
			playerHand *playersHand;
			playersHand = getSpecificHand(activePlayer, currentHand);

			playerRoundResult playerResult;
			playerResult = roundEndedIn(playersHand, dealerPtr);

			/* printf("%s: ", activePlayer->name); */
			printPlayerAndHand(activePlayer, currentHand);
			printf(": ");

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
	sleep(SLEEPAMOUNT);
	printf("\n");
}

static inline void showMoney(blackjackTable *blackjackTablePtr) {
	system("clear");
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];

		int activePlayersMoney;
		activePlayersMoney = getFunds(activePlayer);

		printf("%s's money: %d\n", activePlayer->name, activePlayersMoney);
	}
	sleep(SLEEPAMOUNT);
	printf("\n");
}

static inline void resetPlayers(blackjackTable *blackjackTablePtr, dealer *dealerPtr) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];

		resetPlayer(activePlayer);
	}
	resetDealer(dealerPtr);

}

void blackjackRound(blackjackTable *blackjackTablePtr) {
	//This for loop is used for debugging
	/* for (int i = 0; i < 2; i++) { */
	while (true) {
		dealer *blackjackDealer = getDealer(blackjackTablePtr);

		showMoney(blackjackTablePtr);
		asksPlayerForBet(blackjackTablePtr);
		dealInitialCards(blackjackTablePtr, INITIALCARDCOUNT, blackjackDealer);
		playersTurns(blackjackTablePtr, blackjackDealer);
		dealersTurn(blackjackTablePtr, blackjackDealer);
		losersAndWiners(blackjackTablePtr, blackjackDealer);
		resetPlayers(blackjackTablePtr, blackjackDealer);
	}
}
