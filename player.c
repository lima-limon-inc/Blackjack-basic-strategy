#include "player.h"

//The amount of cards a player has in their hand will vary. According to this
//very scientific website: https://www.blackjackincolor.com/cardsperround.htm
//"The books tell us that the average hand in Blackjack (including the dealer 
//hands) contains about 2.7 cards"
//Maybe it's not the most precise thing in the world, but the number makes 
//sense. We'll assume it to be a bit bigger, just to simplify things and avoid
//the amount of times we need to realloc memory


player *createPlayer(char *playerName, int initialFunds) {
	int initialSize;
	initialSize = sizeof(player) + INITIALCARDS * sizeof(card);

	player *pokerPlayer;
	pokerPlayer = (player *) malloc(initialSize); 

	pokerPlayer->name = playerName;
	pokerPlayer->funds = initialFunds;
	pokerPlayer->cardsInHand = 0;
	pokerPlayer->cardCapacity = INITIALCARDS;

	return pokerPlayer;
}

//TODO: Change function name if I want to get PEGI 13
void killPlayer(player *playerPtr){
	free(playerPtr);
}

static player *resizePlayer(player *playerPtr) {
	//If there's still margin, we don't realloc memory
	if (playerPtr->cardCapacity > playerPtr->cardsInHand) {
		printf("DIDN'T RESIZE\n");
		return playerPtr;
	}

	//We will downsize if it's way to big
	//                   8                             2      * 2
	else if (playerPtr->cardCapacity > playerPtr->cardsInHand * 2) {
		printf("DOWNSIZE\n");

		//Set the new capacity
		playerPtr->cardCapacity /= 2; //Divided equal (fancy notation)


		player *smallerPlayer;
		int smallerSize = sizeof(player) + 
				playerPtr->cardCapacity * sizeof(card *);


		smallerPlayer = realloc(playerPtr, smallerSize);
		return smallerPlayer;
	}

	//We need more memory
	else {
		printf("UPSIZE\n");

		//Set the new capacity
		playerPtr->cardCapacity *= 2; //Divided equal (fancy notation)
		
		player *biggerPlayer;
		int biggerSize = sizeof(player) + 
				playerPtr->cardCapacity * sizeof(card *);


		biggerPlayer = realloc(playerPtr, biggerSize);
		return biggerPlayer;
	}
}

void receiveCard(player *playerPtr, card *newCard) {
	playerPtr = resizePlayer(playerPtr);

	playerPtr->hand[playerPtr->cardsInHand] = newCard;
	playerPtr->cardsInHand += 1;

	printf("%d \n", playerPtr->cardsInHand);
}

void removeCards(player *playerPtr) {
	//We simply move the index. The pointer will simply get removed by
	//the receiveCard function (it will overwrite the value)
	playerPtr->cardsInHand = 0;
}
