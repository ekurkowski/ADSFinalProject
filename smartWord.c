/*

  Authors (group members):
  Email addresses of group members:
  Group name:

  Course:
  Section:

  Description of the overall algorithm:


*/


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "smartWord.h"


/*
  Description for each function, parameter, and return value (if any)
 */



// initialize smartWord by process a file of English dictionary words
void initSmartWord(char *wordFile)
{

  /*
    description of declared variables
   */


  
  /*
    description for each "block" (about 5 lines of instructions)
   */
  
}


// process old messages from oldMessageFile
void procOldMsgSmartWord(char *oldMessageFile)
{

  
}

// Given:
//   letter: letter typed in by the user (a-z, A-Z)
//   letterPosition:  position of the letter in the word, starts from 0
//   wordPosition: position of the word in a message, starts from 0
// Return via a parameter:
//   gueses: NUM_GUESSES (3) word guesses (case-insensive comparison is used) 

void guessSmartWord(char letter, int letterPosition, int wordPosition,
	   char guesses[NUM_GUESSES][MAX_WORDLEN+1])
{

  // just to show how to return 3 (poor) guesses
  strcpy(guesses[0], "abc");
  strcpy(guesses[1], "defgh");
  strcpy(guesses[2], "ijk");
}





// feedback on the 3 guesses from the user
// isCorrectGuess: true if one of the guesses is correct
// correctWord: 3 cases:
// a.  correct word if one of the guesses is correct
// b.  null if none of the guesses is correct, before the user has typed in 
//            the last letter
// c.  correct word if none of the guesses is correct, and the user has 
//            typed in the last letter
// That is:
// Case       isCorrectGuess      correctWord   
// a.         true                correct word
// b.         false               NULL
// c.         false               correct word

// values for bool: true (1), false (0)  

void feedbackSmartWord(bool isCorrectGuess, char *correctWord)
{

}

