#include "blackjackRules.h"
#include "cards.h"

//TODO: Move to playerHand

#define AMOUNTOFCARDSNEEDEDFORSPLIT 2
bool checkForSplit(card *cards[], int amountOfCards) {
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
bool checkForDoubleDown(int amountOfCards) {
	bool canDoubleDown;
	canDoubleDown = (amountOfCards == AMOUNTOFCARDSNEEDEDFORDOUBLEDOWN);

	return canDoubleDown;
}

bool isSoftHand(card *cards[], int amountOfCards) {
	bool hasAce;
	hasAce = false;

	int sum;
	sum = 0;
	for (int currentCard = 0; currentCard < amountOfCards; currentCard++) {
		if (cards[currentCard]->rank == 1) {
			hasAce = true;
			continue;
		}
		int cardRank;
		cardRank = getRank(cards[currentCard]);
		sum += cardRank;
	}

	bool lessThanTen;
	lessThanTen = (sum < 10);

	bool isSoft;
	isSoft = (lessThanTen && hasAce);

	return isSoft;
}

int getValue(card *cardPtr) {
	int cardValue;
	cardValue = cardPtr->rank;

	//King, Queen, Jack all equal 10
	if (cardValue > 10) {
		cardValue = 10;
	}

	return cardValue;
}

//amountOfCards represents how many of the cards are valid cards.
//The array may also contain old, values from previous rounds. Those are not 
//meant to be counted. Normally, amountOfCards will be equal to the amount of
//cards in a players hand
int sumCards(card *cards[], int amountOfCards)
{
	int sum = 0;
	bool foundAnAce = false;

	for (int card = 0; card < amountOfCards; card++) {
		int cardValue; //= cards[card]->rank;
		
		cardValue = getValue(cards[card]);

		//Only one ace can be used with value 11
		if (cardValue == 1 && foundAnAce == false) {
			foundAnAce = true;
			cardValue = 11;
		}

		sum += cardValue;
	}

	//If the sum of all the cards is greater than 21 BECAUSE of an ace,
	//then we substract 10. Because the other value of the ace is 1
	//11 - 10 = 1
	if (foundAnAce == true && sum > 21) {
		sum -= 10;
	}

   return sum;
}
