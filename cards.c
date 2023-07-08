#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define CARDLENGTH 2
typedef enum suits {Clubs, Diamonds, Hearts, Spades} suits;
typedef struct card{
//Nomenclature taken from: https://en.wikipedia.org/wiki/Standard_52-card_deck

	/* short uint rank; */
	int rank;

	//Suit: (C)lubs ♣, (D)iamonds ♦, (H)earts ♥, (S)pades ♠
	/* char suit[CARDLENGTH]; */
	suits suit;
	
}card;


#define SUITAMOUNT 4
#define DECKLENGTH 13
#define DECKAMOUNT 8
typedef struct deck {
	card cards[DECKAMOUNT * DECKLENGTH * SUITAMOUNT];

}deck;

void main()
{
	card cards;

	cards.rank = 4;
	/* strcpy(cards.suit, "f"); */
	cards.suit = Clubs;

	/* printf("Hello world \n"); */
	/* printf("Nombre: %s \n", cards.suit); */
	/* printf("Nombre: %d \n", cards.rank); */

	deck* deckOfCards = malloc(sizeof(deck));
	/* deck* deckOfCards = calloc(1, sizeof(deck)); */

	/* deckOfCards->cards[4].rank = 9; */
	/* printf("Nombre: %d \n", deckOfCards->cards[0].rank); */

	/* printf("Nombre: %ld \n", sizeof(deckOfCards->cards)); */
	/* printf("Nombre: %ld \n", sizeof(*deckOfCards->cards)); */

	/* uint amountOfCards = sizeof(*deckOfCards)/sizeof(deckOfCards->cards[0]); */
	/* printf("Nombre: %d \n", amountOfCards); */
	for ( int suitType = Clubs; suitType <= Spades; suitType++) {

		/* char suitType[CARDLENGTH]; */
		/* switch (j) { */
		/* 	case 0: */
		/* 		strcpy(suitType, "C"); */
		/* 		break; */
		/* 	case 1: */
		/* 		strcpy(suitType, "D"); */
		/* 		break; */
		/* 	case 2: */
		/* 		strcpy(suitType, "H"); */
		/* 		break; */
		/* 	case 3: */
		/* 		strcpy(suitType, "S"); */
		/* 		break; */
		/* } */
		/* printf("%d\n", j); */
		/* printf("%s\n", suitType); */

		for (int rankNumer = 0 + 13 * suitType; rankNumer <= DECKLENGTH + 13 * suitType; rankNumer++) {
			printf("%d\n", rankNumer);
			/* printf("Hello world \n"); */
			deckOfCards->cards[rankNumer].rank = rankNumer;
			deckOfCards->cards[rankNumer].suit = suitType;
			/* deckOfCards->cards[i].suit = suitType; */
			/* strcpy(deckOfCards->cards[i].suit, suitType); */
			/* strcpy(deckOfCards->cards[i].suit, "a"); */

		}
	}

	printf("Nombre: %d \n", deckOfCards->cards[5].rank);
	printf("Nombre: %d \n", deckOfCards->cards[0].suit);
	printf("Nombre: %d \n", deckOfCards->cards[1].suit);
	printf("Nombre: %d \n", deckOfCards->cards[2].suit);
	printf("Nombre: %d \n", deckOfCards->cards[3].suit);
	printf("Nombre: %d \n", deckOfCards->cards[4].suit);
	printf("Nombre: %d \n", deckOfCards->cards[5].suit);
	printf("Nombre: %d \n", deckOfCards->cards[13].suit);
	printf("Nombre: %d \n", deckOfCards->cards[14].suit);
	printf("Nombre: %d \n", deckOfCards->cards[35].suit);
	printf("Nombre: %d \n", deckOfCards->cards[52].suit);



}
