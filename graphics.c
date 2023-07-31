#include "graphics.h"

//COLORS (taken from https://stackoverflow.com/a/23657072/13683575)
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define TOPCARD    "┌───────────┐"
#define RANKCARD   "│%s%s        │"
#define MIDDLECARD "│           │"
#define SUITCARD   "│     %s     │"
#define BRANKCARD  "│        %s%s│"
#define BOTTOMCARD "└───────────┘"

#define TOPCARDBACKSIDE    "┌───────────┐"
#define RANKCARDBACKSIDE   "│░░░░░░░░░░░│"
#define MIDDLECARDBACKSIDE "│░░░░░░░░░░░│"
#define SUITCARDBACKSIDE   "│░░░░░░░░░░░│"
#define BRANKCARDBACKSIDE  "│░░░░░░░░░░░│"
#define BOTTOMCARDBACKSIDE "└───────────┘"


//Got from printf("%ld\n", sizeof("┌─────────┐"));
#define COLORSIZE sizeof(RED)
//Represents the length of the rank character (including color)
#define RANKLENGTH (COLORSIZE + sizeof(int) + COLORSIZE)
//Represents the length of the suit character (including color)
#define SUITLENGTH (COLORSIZE + sizeof(int) + COLORSIZE)

#define CARDWIDTH sizeof(BOTTOMCARD)
#define SPACESBETWEENCARDS 4

#define SPACESBETWEENUMBERANDSUIT 2

static inline char *intToRank(int rank, suits cardSuits) {

	char color[COLORSIZE];
	switch (cardSuits) {
		case Hearts:
			strcpy(color, RED);
			break;
		case Diamonds:
			strcpy(color, RED);
			break;
		default:
			strcpy(color, WHT);
	}
	
	char *representation = malloc(RANKLENGTH);
	switch (rank) {
		case 1:
			sprintf(representation, "%sA%s", color, RESET);
			break;
		case 11:
			sprintf(representation, "%sJ%s", color, RESET);
			break;
		case 12:
			sprintf(representation, "%sQ%s", color, RESET);
			break;
		case 13:
			sprintf(representation, "%sK%s", color, RESET);
			break;
		//The rest of the cards have their number as the representation
		default:
			sprintf(representation, "%s%d%s", color, rank, RESET);
	}
	return representation;
}

static inline char *intToSuit(suits cardSuits) {
	char *representation = malloc(SUITLENGTH);
 
	switch (cardSuits) {
		case Clubs:
			sprintf(representation, "♣");
			break;
		case Diamonds:
			//                      This changes the symbol's color
			sprintf(representation, RED "♦" RESET);
			break;
		case Hearts:
			sprintf(representation, RED "♥" RESET);
			break;
		case Spades:
			sprintf(representation, "♠");
			break;
	}

	return representation;
}

//This function isn't pretty. In fact, it's pretty ugly. It simply prints a card
//TODO: Change argument to playerHand
void asciiRepresentation(card *cards[], int amountOfCards, bool lastCardBlank) {
	int amountOfCardsToPrint;
	amountOfCardsToPrint = amountOfCards;
	if (lastCardBlank == true){
		amountOfCardsToPrint += 1;
	}

	int lengthOfAllTheCards = CARDWIDTH * amountOfCardsToPrint + SPACESBETWEENCARDS * amountOfCards;
	char cardRepresentation[lengthOfAllTheCards];
	/* char cardRepresentation[1]; */
	char bufferCard[CARDWIDTH];

	strcpy(cardRepresentation, " ");
	for (int i = 0; i < amountOfCards; i++) {
		strcat(cardRepresentation, TOPCARD);
		strcat(cardRepresentation, "  ");
	}
	if (lastCardBlank == true) {
		strcat(cardRepresentation, TOPCARDBACKSIDE);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	
	strcpy(cardRepresentation, " ");
	char *rank;
	for (int i = 0; i < amountOfCards; i++) {
		rank = intToRank(cards[i]->rank, cards[i]->suit);

		//Red or white 10
		if ((strcmp(rank,"\x1B[31m10\x1B[0m") == 0) || 
		    (strcmp(rank,"\x1B[37m10\x1B[0m") == 0)) {
			sprintf(bufferCard, RANKCARD, rank," ");
		}
		else {
			sprintf(bufferCard, RANKCARD, rank, "  ");
		}

		strcat(cardRepresentation, bufferCard);
		strcat(cardRepresentation, "  ");
		free(rank);
	}
	if (lastCardBlank == true) {
		strcat(cardRepresentation, RANKCARDBACKSIDE);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	for (int j = 0; j < SPACESBETWEENUMBERANDSUIT; j++) {
		strcpy(cardRepresentation, " ");
		for (int i = 0; i < amountOfCards; i++) {
			strcat(cardRepresentation, MIDDLECARD);
			strcat(cardRepresentation, "  ");
		}
		if (lastCardBlank == true) {
			strcat(cardRepresentation, MIDDLECARDBACKSIDE);
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

		strcat(cardRepresentation, "  ");
		free(suit);
	}
	if (lastCardBlank == true) {
		strcat(cardRepresentation, SUITCARDBACKSIDE);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	for (int j = 0; j < SPACESBETWEENUMBERANDSUIT; j++) {
		strcpy(cardRepresentation, " ");
		for (int i = 0; i < amountOfCards; i++) {
			strcat(cardRepresentation, MIDDLECARD);
			strcat(cardRepresentation, "  ");
		}
		if (lastCardBlank == true) {
			strcat(cardRepresentation, MIDDLECARDBACKSIDE);
			strcat(cardRepresentation, "  ");
		}
	printf(cardRepresentation);
	printf("\n");
	}

	strcpy(cardRepresentation, " ");
	for (int i = 0; i < amountOfCards; i++) {
		rank = intToRank(cards[i]->rank, cards[i]->suit);

		//Red or white 10
		if ((strcmp(rank,"\x1B[31m10\x1B[0m") == 0) || 
		    (strcmp(rank,"\x1B[37m10\x1B[0m") == 0)) {
			sprintf(bufferCard, BRANKCARD, " ", rank);
		}
		else {
			sprintf(bufferCard, BRANKCARD, "  ", rank);
		}

		strcat(cardRepresentation, bufferCard);
		strcat(cardRepresentation, "  ");
		free(rank);
	}
	if (lastCardBlank == true) {
		strcat(cardRepresentation, BRANKCARDBACKSIDE);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");

	strcpy(cardRepresentation, " ");
	for (int i = 0; i < amountOfCards; i++) {
		strcat(cardRepresentation, BOTTOMCARD);
		strcat(cardRepresentation, "  ");
	}
	if (lastCardBlank == true) {
		strcat(cardRepresentation, BOTTOMCARDBACKSIDE);
		strcat(cardRepresentation, "  ");
	}
	printf(cardRepresentation);
	printf("\n");
}

void printCorrectOrNot(bool isCorrect, playerDecision correctDecision) {
	char *message = malloc(sizeof("Double Down"));
	/* strcpy(message, ""); */
	switch (correctDecision) {
		case Hit:
			strcpy(message, "Hit");
			break;
		case Stand:
			strcpy(message, "Stand");
			break;
		case DoubleDown:
			strcpy(message, "Double Down");
			break;
		case Split:
			strcpy(message, "Split");
			break;
	}

	if (isCorrect == true) {
		/* printf(); */
		printf("%s\n", GRN "✓" RESET);
	}
	else {
		/* printf; */
		printf("%s: %s\n", RED "X" RESET, message);
	}

	free(message);
	/* printf(message); */
}
