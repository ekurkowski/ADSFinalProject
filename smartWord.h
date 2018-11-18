/*

    DO NOT MODIFY THIS FILE

    This is the interface of functions provided by the smartWord module
    for other modules.

    Internal smartWord functions not intended for other modules are
    not declared here.

 */

#ifndef SMARTWORD_H
#define SMARTWORD_H

#include <stdbool.h>

#define MAX_WORDLEN 100  // maximum word length
#define NUM_GUESSES   3  // number of word guesses

void initSmartWord(char *wordFile);

void procOldMsgSmartWord(char *oldMessageFile);

void guessSmartWord(char letter, int letterPosition, int wordPosition,
	   char guesses[NUM_GUESSES][MAX_WORDLEN+1]);

void feedbackSmartWord(bool isCorrectGuess, char *correctWord);        


#endif
