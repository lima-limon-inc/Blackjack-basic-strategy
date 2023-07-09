#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cards.h"

int sumCards(card *cards[], int amountOfCards)
{
	int sum = 0;
	bool foundAnAce = false;

	for (int card = 0; card < amountOfCards; card++) {
		int cardValue = cards[card]->rank;

		//Only one ace can be used with value 11
		if (cardValue == 1 && foundAnAce == false) {
			foundAnAce = true;
			cardValue = 11;
		}

		//King, Queen, Jack all equal 10
		if (cardValue > 10) {
			cardValue = 10;
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

