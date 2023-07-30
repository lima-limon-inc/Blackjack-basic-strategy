#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include <stdbool.h>

#include "cards.h"
#include "playerDecision.h"

void asciiRepresentation(card *cards[], int amountOfCards);

void printCorrectOrNot(bool isCorrect, playerDecision correctDecision);

#endif
