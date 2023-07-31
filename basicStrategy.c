#include "basicStrategy.h"

#include "playerDecision.h"
#include "blackjackRules.h"

//This macro makes the table smaller, just aesthetics
#define DD DoubleDown
#define ST Stand
#define SP Split
#define HT Hit

const playerDecision hardHands[17][10] = {
//      2   3   4   5   6   7   8   9  10   A
/* 5*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 6*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 7*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 8*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 9*/{HT, DD, DD, DD, DD, HT, HT, HT, HT, HT},
/*10*/{DD, DD, DD, DD, DD, DD, DD, DD, HT, HT},
/*11*/{DD, DD, DD, DD, DD, DD, DD, DD, DD, DD},
/*12*/{HT, HT, ST, ST, ST, HT, HT, HT, HT, HT},
/*13*/{ST, ST, ST, ST, ST, HT, HT, HT, HT, HT},
/*14*/{ST, ST, ST, ST, ST, HT, HT, HT, HT, HT},
/*15*/{ST, ST, ST, ST, ST, HT, HT, HT, HT, HT},
/*16*/{ST, ST, ST, ST, ST, HT, HT, HT, HT, HT},
/*17*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*18*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*19*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*20*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*21*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
};

const playerDecision softHands[8][10] = {
//       2   3   4   5   6   7   8   9  10   A
/*A,2*/{HT, HT, HT, DD, DD, HT, HT, HT, HT, HT},
/*A,3*/{HT, HT, HT, DD, DD, HT, HT, HT, HT, HT},
/*A,4*/{HT, HT, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,5*/{HT, HT, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,6*/{HT, DD, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,7*/{ST, DD, DD, DD, DD, ST, ST, HT, HT, HT},
/*A,8*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*A,9*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
};

const playerDecision splitHands[10][10] = {
//       2   3   4   5   6   7   8   9  10   A
/*2,2*/{SP, SP, SP, SP, SP, SP, HT, HT, HT, HT},
/*3,3*/{SP, SP, SP, SP, SP, SP, HT, HT, HT, HT},
/*4,4*/{HT, HT, HT, SP, SP, HT, HT, HT, HT, HT},
/*5,5*/{DD, DD, DD, DD, DD, DD, DD, DD, HT, HT},
/*6,6*/{SP, SP, SP, SP, SP, HT, HT, HT, HT, HT},
/*7,7*/{SP, SP, SP, SP, SP, SP, HT, HT, HT, HT},
/*8,8*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*9,9*/{SP, SP, SP, SP, SP, ST, SP, SP, SP, SP},
/*T,T*/{ST, ST, ST, ST, ST, ST, ST, ST, ST, ST},
/*A,A*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
};

//The array starts at 2
#define HORIZONTALOFFSET 2
#define VERTICALOFFSET 2
#define VERTICALOFFSETHARD 5

static inline playerDecision isCorrectSplit(card *playersCard[], card *dealersCard) {
	int cardRrank = getBlackjackValue(playersCard[0]);
	if (cardRrank == 1) {
		//A is 11 in the chart
		cardRrank = 11;
	}

	int dealersRank = getBlackjackValue(dealersCard);
	if (dealersRank == 1) {
		//A is 11 in the chart
		dealersRank = 11;
	}

	int horizontalPosition = dealersRank - HORIZONTALOFFSET;
	int verticalPosition = cardRrank - VERTICALOFFSET;

	playerDecision correctDecision = splitHands[verticalPosition][horizontalPosition];


	return correctDecision;
}

static inline playerDecision isCorrectHard(card *playersCard[], int amountOfCards, card *dealersCard) {
	/* int cardRrank = playersCard[0]->rank; */
	int cardSum = sumCards(playersCard, amountOfCards);

	int dealersRank = getBlackjackValue(dealersCard);
	if (dealersRank == 1) {
		//A is 11 in the chart
		dealersRank = 11;
	}

	int horizontalPosition = dealersRank - HORIZONTALOFFSET;
	int verticalPosition = cardSum - VERTICALOFFSETHARD;

	playerDecision correctDecision = hardHands[verticalPosition][horizontalPosition];
	
	//If you can't DoubleDown, then the best thing is to Hit
	bool canDoubleDown;
	if (correctDecision == DD) {
		canDoubleDown = checkForDoubleDown(amountOfCards);
		correctDecision = (canDoubleDown == true) ? DD : HT;

	}

	return correctDecision;

}

//If this functions is called it means that THERE IS 1 ace. If not, the 
//function won't work (Why would you use it otherwise? :P)
static inline playerDecision isCorrectSoft(card *playersCard[], int amountOfCards, card *dealersCard) {
	int cardSum = sumCards(playersCard, amountOfCards);

	//We remove 11 because the table is represented in the form of A,1; A,2
	//and so on.
	cardSum -= 11;

	int dealersRank = getBlackjackValue(dealersCard);
	if (dealersRank == 1) {
		//A is 11 in the chart
		dealersRank = 11;
	}

	int horizontalPosition = dealersRank - HORIZONTALOFFSET;
	int verticalPosition = cardSum - VERTICALOFFSET;

	playerDecision correctDecision = softHands[verticalPosition][horizontalPosition];

	return correctDecision;

}
  
playerDecision getCorrectChoice(card *playersCard[], int amountOfCards, card *dealersCard) {
	playerDecision correctDecision;

	bool canSplit;
	canSplit = checkForSplit(playersCard, amountOfCards);

	if (canSplit == true) {
		correctDecision = isCorrectSplit(playersCard, dealersCard);
		return correctDecision;
	}

	//AKA has an ace
	bool isSoft;
	isSoft  = isSoftHand(playersCard, amountOfCards);
	if (isSoft == true) {
		correctDecision = isCorrectSoft(playersCard, amountOfCards, dealersCard);
		return correctDecision;
	}

	correctDecision = isCorrectHard(playersCard, amountOfCards, dealersCard);

	return correctDecision;
}
