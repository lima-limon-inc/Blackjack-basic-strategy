#include "player.h"

//The amount of cards a player has in their hand will vary. According to this
//very scientific website: https://www.blackjackincolor.com/cardsperround.htm
//"The books tell us that the average hand in Blackjack (including the dealer 
//hands) contains about 2.7 cards"
//Maybe it's not the most precise thing in the world, but the number makes 
//sense. We'll assume it to be a bit bigger, just to simplify things and avoid
//the amount of times we need to realloc memory
#define INITIALCARDS 4


player *createPlayer(char *playerName, int initialFunds) {
	int initialSize;
	initialSize = sizeof(deck) + INITIALCARDS * sizeof(card);

	player *pokerPlayer;
	pokerPlayer = (player *) malloc(initialSize); 

	pokerPlayer->name = playerName;
	pokerPlayer->funds = initialFunds;

	return pokerPlayer;
}
