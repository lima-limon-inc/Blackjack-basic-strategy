#include "cards.h"

//Got from printf("%ld\n", sizeof("┌─────────┐"));
#define CARDWIDTH 34
#define CARDLENGTH 11
#define SPACESBETWEENCARDS 4
#define SPACESBETWEENUMBERANDSUIT 2
#define RANKLENGTH 2
//‘♥’ directive writing 3 bytes
#define SUITLENGTH 4

#define TOPCARD    "┌──────────┐"
#define RANKCARD   "│%s%s        │"
#define MIDDLECARD "│          │"
#define SUITCARD   "│    %s     │"
#define BRANKCARD  "│        %s%s│"
#define BOTTOMCARD "└──────────┘"

//COLORS (taken from https://stackoverflow.com/a/23657072/13683575)
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


static inline char *intToRank(int rank) {
	char *representation = malloc(RANKLENGTH);
	
	switch (rank) {
		case 1:
			sprintf(representation, "A");
			break;
		case 11:
			sprintf(representation, "J");
			break;
		case 12:
			sprintf(representation, "Q");
			break;
		case 13:
			sprintf(representation, "K");
			break;
		//The rest of the cards have their number as the representation
		default:
			sprintf(representation, "%d", rank);
	}
	/* if (rank >= 2 || rank <= 9) { */
	/* } */

	return representation;
}

static inline char *intToSuit(suits cardSuits) {
	char *representation = malloc(SUITLENGTH);
	
	switch (cardSuits) {
		case Clubs:
			sprintf(representation, "♣");
			break;
		case Diamonds:
			sprintf(representation, "♦");
			break;
		case Hearts:
			sprintf(representation, "♥");
			break;
		case Spades:
			sprintf(representation, "♠");
			break;
	}

	return representation;
}

void asciiRepresentation(card *cards[], int amountOfCards) {
	char cardRepresentation[CARDWIDTH * amountOfCards + SPACESBETWEENCARDS * amountOfCards];
	char bufferCard[CARDWIDTH];

	strcpy(cardRepresentation, " ");
	for (int i = 0; i < amountOfCards; i++) {
		strcat(cardRepresentation, TOPCARD);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	
	strcpy(cardRepresentation, " ");
	char *rank;
	for (int i = 0; i < amountOfCards; i++) {
		rank = intToRank(cards[i]->rank);
		if (strcmp(rank,"10") == 0) {
			sprintf(bufferCard, RANKCARD, rank, "");
		}
		else {
			sprintf(bufferCard, RANKCARD, rank, " ");
		}

		strcat(cardRepresentation, bufferCard);
		strcat(cardRepresentation, "  ");
		free(rank);
	}
	printf(cardRepresentation);
	printf("\n");

	for (int j = 0; j < SPACESBETWEENUMBERANDSUIT; j++) {
		strcpy(cardRepresentation, " ");
		for (int i = 0; i < amountOfCards; i++) {
			strcat(cardRepresentation, MIDDLECARD);
			strcat(cardRepresentation, "  ");
		}
	printf(cardRepresentation);
	printf("\n");
	}

	strcpy(cardRepresentation, " ");
	char *suit;
	for (int i = 0; i < amountOfCards; i++) {
		suit = intToSuit(cards[i]->suit);
		sprintf(bufferCard, SUITCARD, suit);
		strcat(cardRepresentation, bufferCard);

		/* strcat(cardRepresentation, TOPCARD); */
		strcat(cardRepresentation, "  ");
		free(suit);
	}
	printf(cardRepresentation);
	printf("\n");

	for (int j = 0; j < SPACESBETWEENUMBERANDSUIT; j++) {
		strcpy(cardRepresentation, " ");
		for (int i = 0; i < amountOfCards; i++) {
			strcat(cardRepresentation, MIDDLECARD);
			strcat(cardRepresentation, "  ");
		}
	printf(cardRepresentation);
	printf("\n");
	}

	strcpy(cardRepresentation, " ");
	/* char *rank; */
	for (int i = 0; i < amountOfCards; i++) {
		rank = intToRank(cards[i]->rank);
		if (strcmp(rank,"10") == 0) {
			sprintf(bufferCard, BRANKCARD, "", rank);
		}
		else {
			sprintf(bufferCard, BRANKCARD, " ", rank);
		}

		strcat(cardRepresentation, bufferCard);
		strcat(cardRepresentation, "  ");
		free(rank);
	}
	printf(cardRepresentation);
	printf("\n");

	strcpy(cardRepresentation, " ");
	for (int i = 0; i < amountOfCards; i++) {
		strcat(cardRepresentation, BOTTOMCARD);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	/* free(suit); */
	/* free(rank); */
}
