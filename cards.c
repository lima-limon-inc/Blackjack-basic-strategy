#include <stdio.h>
#include <string.h>
#define CARDLENGTH 1

/* typedef struct deck { */



/* } */

typedef struct card{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	/* short uint rank; */
	int rank;

	//Suit: (C)lubs ♣, (D)iamonds ♦, (H)earts ♥, (S)pades ♠
	char suit[CARDLENGTH];
	
}card;

void main()
{
	card cards;

	cards.rank = 4;
	strcpy(cards.suit, "f");

	printf("Hello world \n");
	printf("Nombre: %s \n", cards.suit);
	printf("Nombre: %d \n", cards.rank);



}
