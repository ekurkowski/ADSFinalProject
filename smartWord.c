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

typedef struct node {
    int count; // number of times used
    char word[MAX_WORDLEN]; // possible word to be used
    struct node *next;
} Word;

Word *StartGuess = NULL;
Word *EndGuess = NULL;
Word *PrevGuessed = NULL;

/*
  Description for each function, parameter, and return value (if any)
 */



// initialize smartWord by process a file of English dictionary words
void initSmartWord(char *wordFile)
{
 // can we normalize everything to be lowercase before we put it in the database?
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

   // can we normalize everything to be lowercase before we put it in the database?
   // useful link to make a new function that removes everything but letters
   // https://www.programiz.com/c-programming/examples/remove-characters-string
}


void GetHighestScores(Word *one, Word *two, Word *three, Word *four) {
  // return the highest three scores out of the four
  // this might not be super efficient and could use some help I think
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
  Word *ptr = NULL;
  Word *guess_one;
  Word *guess_two;
  Word *guess_three;
  // pay attention to upper and lowercase input

  if (letterPosition == 0) {
    // StartGuess = the same pointer as the letter in the array
    ptr = StartGuess
    // iterate through all of the possible words and return highest three
    // return
  }

  if (letterPosition == 1) {
    // move StartGuess to the beginning of that linked list in the sub array
    ptr = StartGuess;
    // iterate through and find three highest scores in the list and copy to the guesses
    // set EndGuess once reach the end
    // return
  }

  ptr = StartGuess;
  // move StartGuess to where it matches the letter of the word
  while (ptr->word[letterPosition] != tolower(letter)) {
      ptr = ptr->next;
  }

  StartGuess = ptr;
  guess_one = ptr;
  guess_two = ptr->next;
  guess_three = ptr->next->next; // pay attention to make sure this isn't the end of the list

  while (ptr->word[letterPosition] != tolower(letter)) {
    GetHighestScores(guess_one, guess_two, guess_three, ptr);
    ptr = ptr->next;
  }



  // just to show how to return 3 (poor) guesses
  strcpy(guesses[0], guess_one->word);
  strcpy(guesses[1], guess_two->word);
  strcpy(guesses[2], guess_three->word);
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
