#ifndef BLACKJACKRULES_HEADER
#define BLACKJACKRULES_HEADER

#include "cards.h"

bool checkForSplit(card *cards[], int amountOfCards);

bool checkForDoubleDown(int amountOfCards);

bool isSoftHand(card *cards[], int amountOfCards);

int sumCards(card *cards[], int amountOfCards);

int getBlackjackValue(card *cardPtr);

#endif
