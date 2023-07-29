#include "basicStrategy.h"

#include <stdbool.h>

#include "playerDecision.h"

/* typedef enum playerDecision {Hit, Stand, DoubleDown, Split} playerDecision; */

//This macro makes the table smaller, just aesthetics
#define DD DoubleDown
#define ST Stand
#define SP Split
#define HT Hit


/* efjknfen */

const playerDecision hardHands[16][10] = {
//      2   3   4   5   6   7   8   9  10   A
/* 5*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 6*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 7*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 8*/{HT, HT, HT, HT, HT, HT, HT, HT, HT, HT},
/* 9*/{HT, DD, DD, DD, DD, HT, HT, HT, HT, HT},
/*10*/{DD, DD, DD, DD, DD, DD, DD, DD, HT, HT},
/*11*/{DD, DD, DD, DD, DD, DD, DD, DD, DD, DD},
/*12*/{HT, HT, SP, SP, SP, HT, HT, HT, HT, HT},
/*13*/{SP, SP, SP, SP, SP, HT, HT, HT, HT, HT},
/*14*/{SP, SP, SP, SP, SP, HT, HT, HT, HT, HT},
/*15*/{SP, SP, SP, SP, SP, HT, HT, HT, HT, HT},
/*16*/{SP, SP, SP, SP, SP, HT, HT, HT, HT, HT},
/*17*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*18*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*19*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*20*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*21*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
}

const playerDecision softHands[8][10] = {
//       2   3   4   5   6   7   8   9  10   A
/*A,2*/{HT, HT, HT, DD, DD, HT, HT, HT, HT, HT},
/*A,3*/{HT, HT, HT, DD, DD, HT, HT, HT, HT, HT},
/*A,4*/{HT, HT, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,5*/{HT, HT, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,6*/{HT, DD, DD, DD, DD, HT, HT, HT, HT, HT},
/*A,7*/{SP, DD, DD, DD, DD, SP, SP, HT, HT, HT},
/*A,8*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
/*A,9*/{SP, SP, SP, SP, SP, SP, SP, SP, SP, SP},
}

#define TR true
#define FA false
const bool splitHands[8][10] = {
//       2   3   4   5   6   7   8   9  10   A
/*2,2*/{TR, TR, TR, TR, TR, TR, FA, FA, FA, FA},
/*3,3*/{TR, TR, TR, TR, TR, TR, FA, FA, FA, FA},
/*4,4*/{FA, FA, FA, TR, TR, FA, FA, FA, FA, FA},
/*5,5*/{FA, FA, FA, FA, FA, FA, FA, FA, FA, FA},
/*6,6*/{TR, TR, TR, TR, TR, FA, FA, FA, FA, FA},
/*7,7*/{TR, TR, TR, TR, TR, TR, FA, FA, FA, FA},
/*8,8*/{TR, TR, TR, TR, TR, TR, TR, TR, TR, TR},
/*9,9*/{TR, TR, TR, TR, TR, FA, TR, TR, FA, FA},
/*T,T*/{FA, FA, FA, FA, FA, FA, FA, FA, FA, FA},
/*A,A*/{TR, TR, TR, TR, TR, TR, TR, TR, TR, TR},
}
  
bool isCorrectChoice(playerDecision decisionToCheck, card *playersCard[], int amountOfCards, card *dealersCard) {
	int cardSum; = sumCards(playersCard, amountOfCards);
	if (amountOfCards == 2) {
	}
	int


}
