#include <unistd.h>
#include <stdio.h>

#include "blackjackTable.h"

#include "globalMacros.h"
#include "player.h"
#include "graphics.h"
#include "playerDecision.h"
#include "blackjackRules.h"
#include "basicStrategy.h"

#define MAXAMOUNTOFSPLITS 4
#define INITIALCAPACITY 8
#define INITIALCARDCOUNT 2
#define CARDSUMBEFOREBUST 21
/* #define BLACKJACK 21 */
#define DEALERLIMIT 17
/* #define AMOUNTOFCARDSFORBLACKJACK 2 */
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

void removePlayer(blackjackTable *blackjackTablePtr, int playerPosition) {
	killPlayer(blackjackTablePtr->players[playerPosition]);

	//Move player pointers to adjust position
	for (int i = playerPosition; i < blackjackTablePtr->playerAmount; i++) {
		blackjackTablePtr->players[i] = blackjackTablePtr->players[i + 1];
	}

	blackjackTablePtr->playerAmount -= 1;
}

blackjackTable *addPlayer(blackjackTable *blackjackTablePtr, char name[], int initialFunds) {
	player *newPlayer = createPlayer(name,  initialFunds);

	blackjackTablePtr->players[blackjackTablePtr->playerAmount] = newPlayer;
	blackjackTablePtr->playerAmount += 1;

	//TODO: Add resize down function
	if (blackjackTablePtr->playerAmount == blackjackTablePtr->playerCapacity) {
		/* printf("REALLOC\n"); */
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
			giveCardTo(activePlayer,  topCard, 0);
		}
		card *dealersCard = dealACard(blackjackDealer);

		dealDealersHand(blackjackDealer, dealersCard);
	}
}



#define FULLINPUTMESSAGE "What do you do? (H)it, (S)tand or (D)ouble Down or S(P)lit: "
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

static inline playerDecision askForDecision(player *activePlayer, int currentHand, playerHand *activePlayerHand, int howManyHands) {
	int currentBet;
	currentBet = getBet(activePlayer, currentHand);

	bool canAffordMove;
	canAffordMove = canMakeABet(activePlayer, currentBet);

	bool canSplit;
	if (howManyHands < MAXAMOUNTOFSPLITS) {
		canSplit = checkForSplit(getCards(activePlayerHand), getAmountOfCards(activePlayerHand));

		// The split is only valid IF you can make it AND you can 
		// afford it
		canSplit = (canSplit && canAffordMove);

	}
	else {
		//If the player has exceeded the amount of valid splits, then
		//we don't even bother calculating it
		canSplit = false;
	}

	bool canDoubleDown;
	canDoubleDown = checkForDoubleDown(getAmountOfCards(activePlayerHand));
	canDoubleDown = (canDoubleDown && canAffordMove);

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
		FLUSHSTDIN;

		if (strcmp(userInput, "H") == 0 || strcmp(userInput, "h") == 0) {
			playersDecision = Hit;
			validAnswer = true;
		}
		else if (strcmp(userInput, "S") == 0 || strcmp(userInput, "s") == 0) {
			playersDecision = Stand;
			validAnswer = true;
		}
		else if ((canDoubleDown == true && strcmp(userInput, "D") == 0) || (canDoubleDown == true && strcmp(userInput, "d") == 0)) {
			playersDecision = DoubleDown;
			validAnswer = true;
		}
		else if ((canSplit == true && strcmp(userInput, "P") == 0) || (canSplit == true && strcmp(userInput, "p") == 0)) {
			playersDecision = Split;
			validAnswer = true;
		}
	}

	return playersDecision;
}

static inline bool askPlayerForBet(player *activePlayer, int whichHand) {
	bool wantsToKeepPlaying;
	wantsToKeepPlaying = true;

	int playerBet;

	bool canMakeBet;
	canMakeBet = false;

	while (canMakeBet == false) {
		printf("%s, what's your bet?\n(Type '%s' to leave the table)\n", activePlayer->name, ENOUGHSYMBOLSTRING); 
		scanf("%d", &playerBet);
		FLUSHSTDIN;

		if (playerBet == ENOUGHSYMBOL) {
			wantsToKeepPlaying = false;
			return wantsToKeepPlaying;
		} else if (playerBet < 0) {
			printf("Bet's can't be negative\n");
			continue;
		}

		canMakeBet = canMakeABet(activePlayer, playerBet);
		
		if (canMakeBet == false) {
			printf("You don't have enough funds\n");
		}
		
	}

	makeABet(activePlayer, playerBet, whichHand);
	return wantsToKeepPlaying;
}

static inline void asksPlayerForBet(blackjackTable *blackjackTablePtr) {
	for (int position = 0; position < blackjackTablePtr->playerAmount; position++) {
		player *activePlayer;
		activePlayer = getPlayerAtPosition(blackjackTablePtr, position);

		bool playerIsBroke;
		playerIsBroke = isBroke(activePlayer);
		if (playerIsBroke == true) {
			printf("%s, you have no money left \n", activePlayer->name);
			printf("You are removed from the casino\n");
			removePlayer(blackjackTablePtr, position);
			position--;
			continue;
		}

		//This could and possibly should be its own function. However,
		//it would be annoying to ask ALL players each time if they
		//want to exit the table
		bool wantsToKeepPlaying;
		wantsToKeepPlaying = askPlayerForBet(activePlayer, 0);
		if (wantsToKeepPlaying == false) {
			removePlayer(blackjackTablePtr, position);
			position--;
		}
	}
}

static dealer *getDealer(blackjackTable *blackjackTablePtr) {
	return blackjackTablePtr->blackjackDealer;
}

static void printDealersCards(dealer *blackjackDealer, bool showAllCards) {
	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(blackjackDealer);

	bool lastCardBlank;
	if (showAllCards == false) {
		lastCardBlank =  true;
	}
	else {
		/* howManyCards = getAmountOfCards(dealersHand);//blackjackDealer->cardsInHand; */
		lastCardBlank =  false;
	}
	printf("DEALER: \n");
	asciiRepresentation(dealersHand, lastCardBlank);
}

static inline bool offerPlayerForInsurance(player *activePlayer) {
	bool wantsInsurance;

	bool validAnswer;
	validAnswer = false;

	char userInput[10];

	while (validAnswer == false) {
		printf("Would %s like Insurance? (Y/N): ", activePlayer->name);
		scanf("%s", userInput);
		FLUSHSTDIN;

		if (strcmp(userInput, "Y") == 0) {
			wantsInsurance = true;
			validAnswer = true;
		}
		else if (strcmp(userInput, "N") == 0) {
			wantsInsurance = false;
			validAnswer = true;
		}
	}

	return wantsInsurance;
}

static inline void offerEveryoneInsurance(blackjackTable *blackjackTablePtr) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];

		int originalBet;
		originalBet = getBet(activePlayer, 0);
		originalBet /= 2;

		bool canAffordMove;
		canAffordMove = canMakeABet(activePlayer, originalBet);

		if (canAffordMove == false) {
			printf("%s can't afford  insurance\n", activePlayer->name);
			continue;
		}

		bool wantsInsurance;
		wantsInsurance = offerPlayerForInsurance(activePlayer);
		if (wantsInsurance == true) {
			askForInsurance(activePlayer);
		}
	}
}

static inline void awardPlayerInsurance(blackjackTable *blackjackTablePtr, bool dealerHasBlackjack) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];

		int insurancePosition;
		insurancePosition = getInsurancePosition();

		int insuranceBetAmount;
		insuranceBetAmount = getBet(activePlayer, insurancePosition);

		//Did not ask for insurance, we skip it
		if (insuranceBetAmount == 0) {
			continue;
		}
		
		if (dealerHasBlackjack == true) {
			insuranceBetAmount *= 2;
			winBet(activePlayer, insuranceBetAmount, insurancePosition);
			printf("%s won the insurance bet\n", activePlayer->name);
		} else {
			int lostMoney;
			lostMoney = loseBet(activePlayer, insurancePosition);
			takeMoney(blackjackTablePtr->blackjackDealer, lostMoney);
			printf("%s lost the insurance bet\n", activePlayer->name);
		}
	}
}

static inline bool insuranceLoop(blackjackTable *blackjackTablePtr, dealer *blackjackDealer) {
	bool dealerHasBlackjack;

	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(blackjackDealer);

	card *dealerTopCard;
	dealerTopCard = getCards(dealersHand)[0];

	int topCardRank;
	topCardRank = getBlackjackValue(dealerTopCard);

	//Insurance is only offered if the dealer has an Ace
	if (topCardRank != 1 && topCardRank != 10) {
		dealerHasBlackjack = false;
		return dealerHasBlackjack;
	}

	system("clear");
	printDealersCards(blackjackDealer, false);

	if (topCardRank == 1) {
		offerEveryoneInsurance(blackjackTablePtr);
	}

	int amountOfCards;
	amountOfCards = getAmountOfCards(dealersHand);

	card **dealersCards;
	dealersCards = getCards(dealersHand);

	int dealersSum;
	dealersSum = sumCards(dealersCards, amountOfCards);

	dealerHasBlackjack = isBlackjack(dealersSum, amountOfCards);

	printf("Checking if dealer has Blackjkack\n");
	printf(".");
	/* sleep(SLEEPAMOUNT); */
	printf(".");
	printf(".");
	sleep(SLEEPAMOUNT);
	printf("\n");
	sleep(SLEEPAMOUNT);

	if (dealerHasBlackjack == true) {
		printf("Dealer HAS Blackjkack\n");
		awardPlayerInsurance(blackjackTablePtr, dealerHasBlackjack);
	} else {
		printf("Dealer does not have Blackjkack\n");
	}
	/* sleep(SLEEPAMOUNT); */

	/* if (dealerHasBlackjack == false) { */
	/* 	return; */
	/* } */


	printf("\n");
	printf("Press any key to continue");
	getchar();

	return dealerHasBlackjack;
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

	asciiRepresentation(activePlayerHand, false);
}

static inline bool processPlayerMove(player *activePlayer, int currentHand, playerDecision playersDecision, dealer *blackjackDealer) {
	card *topCard;

	bool playersTurnContinues;
	switch (playersDecision) {
		case Hit:
			topCard = dealACard(blackjackDealer);

			giveCardTo(activePlayer, topCard, currentHand);
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
			giveCardTo(activePlayer, topCard, currentHand);
			playersTurnContinues = false;
			break;
		case Split:
			splitCards(activePlayer, currentHand);
			//We deal an addtional to the newly
			//created hand
			topCard = dealACard(blackjackDealer);
			giveCardTo(activePlayer, topCard, currentHand);

			topCard = dealACard(blackjackDealer);
			giveCardTo(activePlayer, topCard, getNumberOfHands(activePlayer) - 1);
			playersTurnContinues = true;
			break;
	}

	return playersTurnContinues;
}

static void endPlayerTurn(player *activePlayer, int currentHand, dealer *blackjackDealer) {
	/* playerHand *activePlayerHand; */
	/* activePlayerHand = getSpecificHand(activePlayer, currentHand); */

	system("clear");
	printVisualRepresentation(activePlayer, currentHand, blackjackDealer, false);
	sleep(SLEEPAMOUNT);
	/* saveCardSum(activePlayerHand, playersSum); */
}

static inline void isItCorrectChoice(playerHand *playerHandPtr, card *dealersCard, playerDecision playersDecision) {
	playerDecision correctDecision;
	correctDecision = getCorrectChoice(playerHandPtr, dealersCard);

	bool isItCorrectChoice;
	isItCorrectChoice = (correctDecision == playersDecision);

	printCorrectOrNot(isItCorrectChoice, correctDecision);
}

static inline int mainPlayerActionLoop(player *activePlayer, dealer *blackjackDealer, int currentHand, int initialSum) {
	int playersSum;
	playersSum = initialSum;

	bool playersTurnContinues;
	playersTurnContinues = true;

	playerDecision playersDecision;

	playerHand *activePlayerHand;
	activePlayerHand = getSpecificHand(activePlayer, currentHand);

	//Main player turn loop
	while (playersTurnContinues == true && playersSum < CARDSUMBEFOREBUST) {
		system("clear");
		printVisualRepresentation(activePlayer, currentHand, blackjackDealer, false);

		//If the player busts, then he lost his turn
		if (playersSum > CARDSUMBEFOREBUST) {
			break;
		}

		playersDecision = askForDecision(activePlayer, currentHand, activePlayerHand, getNumberOfHands(activePlayer));

		card *dealersCard;
		dealersCard = getCards(getSpecificHandDealer(blackjackDealer))[0];

		/* playerDecision correctDecision; */
		/* correctDecision = getCorrectChoice(activePlayerHand, dealersCard); */

		/* bool isItCorrectChoice; */
		/* isItCorrectChoice = (correctDecision == playersDecision); */

		isItCorrectChoice(activePlayerHand, dealersCard, playersDecision);

		playersTurnContinues = processPlayerMove(activePlayer, currentHand, playersDecision, blackjackDealer);

		/* printCorrectOrNot(isItCorrectChoice, correctDecision); */
		sleep(SLEEPAMOUNT);


		activePlayerHand = getSpecificHand(activePlayer, currentHand);
		playersSum = sumCards(getCards(activePlayerHand), getAmountOfCards(activePlayerHand));
	}

	return playersSum;
}

static inline void activePlayerTurn(player *activePlayer, dealer *blackjackDealer) {

	bool firstHand;
	firstHand = true;

	for (int currentHand = 0; currentHand < getNumberOfHands(activePlayer); currentHand++) {
		playerHand *activePlayerHand;
		activePlayerHand = getSpecificHand(activePlayer, currentHand);

		int amountOfCards;
		amountOfCards = getAmountOfCards(activePlayerHand);

		int playersSum; 
		playersSum = sumCards(getCards(activePlayerHand), amountOfCards);

		/* if (firstHand == true && playersSum == BLACKJACK) { */

		//You can only get a Blackjack in you first hand when you
		//are originally dealt cards. If you get it, your turn finishes
		if (firstHand == true) {
			bool hasBlackjack;
			hasBlackjack = isBlackjack(playersSum, amountOfCards);
			if (hasBlackjack == true) {
				endPlayerTurn(activePlayer, currentHand, blackjackDealer);
				break;
			}
		}

		playersSum =  mainPlayerActionLoop(activePlayer, blackjackDealer, currentHand, playersSum);
		//This will over write the value everytime. This is fine (I think)
		firstHand = false;

		endPlayerTurn(activePlayer, currentHand, blackjackDealer);
	}
}

static inline void playersTurns(blackjackTable *blackjackTablePtr, dealer *blackjackDealer) {
	for (int i = 0; i < blackjackTablePtr->playerAmount; i++) {
		player *activePlayer;
		activePlayer = blackjackTablePtr->players[i];
		
		activePlayerTurn(activePlayer, blackjackDealer);
	}
}

static inline void showPlayersSums(blackjackTable *blackjackTablePtr) {
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
}

static inline void dealersTurn(blackjackTable *blackjackTablePtr, dealer *blackjackDealer) {
	int dealersSum;
	
	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(blackjackDealer);

	dealersSum = getHandSum(dealersHand);

	system("clear");
	showPlayersSums(blackjackTablePtr);
	printDealersCards(blackjackDealer, true);
	sleep(SLEEPAMOUNT);

	while (dealersSum < DEALERLIMIT) {
		system("clear");
		showPlayersSums(blackjackTablePtr);
		card *topCard = dealACard(blackjackDealer);

		dealDealersHand(blackjackDealer,  topCard);
		//dealersHand changed from its initical call to here, due to
		//the dealDealersHand function, that's why we have to
		//redefine it
		dealersHand = getSpecificHandDealer(blackjackDealer);

		dealersSum = getHandSum(dealersHand);
		/* dealersSum = sumCards(getCards(dealersHand), howManyCardsDealer); */
		printDealersCards(blackjackDealer, true);
		sleep(SLEEPAMOUNT);
	}

	/* saveCardSum(dealersHand, dealersSum); */
	printf("\nDealers sum :%d\n", dealersSum);
	sleep(SLEEPAMOUNT);
}

typedef enum playerRoundResult {Win, Lost, Tie, Blackjack} playerRoundResult;
static inline playerRoundResult roundEndedIn(playerHand *activeHand, dealer *dealerPtr) {
	int playersSum;
	playersSum = getHandSum(activeHand);

	int playersAmountOfCards;
	playersAmountOfCards = getAmountOfCards(activeHand);

	playerHand *dealersHand;
	dealersHand = getSpecificHandDealer(dealerPtr);

	int dealersSum;
	dealersSum = getHandSum(dealersHand);

	int dealerAmountOfCards;
	dealerAmountOfCards = getAmountOfCards(dealersHand);

	//Player lost, he busted
	if (playersSum > CARDSUMBEFOREBUST) {
		return Lost;
	}

	bool playersHasBlackjack;
	playersHasBlackjack = isBlackjack(playersSum, playersAmountOfCards);

	bool dealerHasBlackjack;
	dealerHasBlackjack = isBlackjack(dealersSum, dealerAmountOfCards);
	if (playersHasBlackjack == true) {
		if (dealerHasBlackjack == false) {
			return Blackjack;
		} else {
			return Tie;
		}
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
			system("clear");
			printVisualRepresentation(activePlayer, currentHand, dealerPtr, true);

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

		printf("Press any key to continue");
		getchar();
		}
	}
	sleep(SLEEPAMOUNT);
	/* printf("Press any key to continue"); */
	/* getchar(); */


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

static inline void resetPlayersAndDealer(blackjackTable *blackjackTablePtr, dealer *dealerPtr) {
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
	dealer *blackjackDealer = getDealer(blackjackTablePtr);
	while (blackjackTablePtr->playerAmount > 0) {
		bool dealerHasBlackjack;

		showMoney(blackjackTablePtr);
		asksPlayerForBet(blackjackTablePtr);
		dealInitialCards(blackjackTablePtr, INITIALCARDCOUNT, blackjackDealer);
		dealerHasBlackjack = insuranceLoop(blackjackTablePtr, blackjackDealer);
		if (dealerHasBlackjack == false) {
			playersTurns(blackjackTablePtr, blackjackDealer);
			dealersTurn(blackjackTablePtr, blackjackDealer);
		} 
		losersAndWiners(blackjackTablePtr, blackjackDealer);
		resetPlayersAndDealer(blackjackTablePtr, blackjackDealer);
	}
}
