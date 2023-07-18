#include "player.h"

//The amount of cards a player has in their hand will vary. According to this
//very scientific website: https://www.blackjackincolor.com/cardsperround.htm
//"The books tell us that the average hand in Blackjack (including the dealer 
//hands) contains about 2.7 cards"
//Maybe it's not the most precise thing in the world, but the number makes 
//sense. We'll assume it to be a bit bigger, just to simplify things and avoid
//the amount of times we need to realloc memory
#define INITIALPLAYERCARDS 4

player *createPlayer(char *playerName, int initialFunds) {
	int initialSize;
	initialSize = sizeof(player) + INITIALPLAYERCARDS * sizeof(card *);

	player *pokerPlayer;
	pokerPlayer = (player *) malloc(initialSize); 

	/* pokerPlayer->name = playerName; */
	strcpy(pokerPlayer->name, playerName);
	pokerPlayer->funds = initialFunds;
	pokerPlayer->cardsInHand = 0;
	pokerPlayer->cardCapacity = INITIALPLAYERCARDS;
	pokerPlayer->bet = 0;

	return pokerPlayer;
}

//TODO: Change function name if I want to get PEGI 13
void killPlayer(player *playerPtr){
	free(playerPtr);
}

static player *resizePlayer(player *playerPtr) {
	//If there's still margin, we don't realloc memory
	if (playerPtr->cardCapacity > playerPtr->cardsInHand) {
		/* printf("DIDN'T RESIZE\n"); */
		return playerPtr;
	}

	//We will downsize if it's way to big
	//                   8                             2      * 2
	else if (playerPtr->cardCapacity > playerPtr->cardsInHand * 2) {
		printf("DOWNSIZE\n");

		//Set the new capacity
		playerPtr->cardCapacity /= 2; //Divided equal (fancy notation)
	}

	//We need more memory
	else {
		printf("UPSIZE\n");

		//Set the new capacity
		playerPtr->cardCapacity *= 2; //Divided equal (fancy notation)
	}
		
	player *resizedPlayer;
	int newSize = sizeof(player) + 
			playerPtr->cardCapacity * sizeof(card *);


	resizedPlayer = realloc(playerPtr, newSize);
	return resizedPlayer;
}

player *receiveCard(player *playerPtr, card *newCard) {
	player *resizedPlayer = resizePlayer(playerPtr);

	resizedPlayer->hand[playerPtr->cardsInHand] = newCard;
	resizedPlayer->cardsInHand += 1;

	return resizedPlayer;
}

void removeCards(player *playerPtr) {
	//We simply move the index. The pointer will simply get removed by
	//the receiveCard function (it will overwrite the value)
	playerPtr->cardsInHand = 0;
}

void printCards(player *playerPtr) {
	for (int i = 0; i < playerPtr->cardsInHand; i++) {
		printf("%s's card: ", playerPtr->name); 
		printf("%d, %d \n", playerPtr->hand[i]->rank, 
				playerPtr->hand[i]->suit);
	}
}

void makeABet(player *playerPtr, int bet) {
	//TODO: Create if statement to check if you have enough funds
	playerPtr->bet += bet;
	playerPtr->funds -= bet;
}

int loseBet(player *playerPtr) {
	int lostBet;
	lostBet = playerPtr->bet;

	playerPtr->bet = 0;
	return lostBet;
}
