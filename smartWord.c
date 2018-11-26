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
#include <ctype.h>

#include "smartWord.h"



struct hash{
	struct inter *range[26];
}*alpha_hash[26];

struct inter{
	struct node *head;
	struct node *tail;
};

typedef struct node{
	char pos_word[MAX_WORDLEN];
	int score; // number of times used
	struct node *next;
} Word;

void create_space();
int find_word(char word[100],int index,int index_2);

void create_space(){
	for(int i=0;i<26;i++){
		alpha_hash[i] = (struct hash*) malloc(sizeof(struct hash));
		for(int j=0;j<26;j++){
			alpha_hash[i]->range[j] = (struct inter*) malloc(sizeof(struct inter));
		}
	}
	return;
}


void insert(char word[100],int index,int index_2){
	struct node *temp;
	struct node *list = (struct node*) alpha_hash[index]->range[index_2]->head;
	struct node *new_node = (struct node*) malloc(sizeof(struct node));
	strcpy(new_node->pos_word,word);
	new_node->next = NULL;
	new_node->score = 1;

	if(list == NULL){ //if list is empty in index
		alpha_hash[index]->range[index_2]->head = new_node;
		alpha_hash[index]->range[index_2]->tail = new_node;
	}
	else{ //if list is not empty
		temp = alpha_hash[index]->range[index_2]->tail;
		temp->next = new_node;
		alpha_hash[index]->range[index_2]->tail = new_node;
	}
	//printf("\nSuccess added %s at inde x %d and %d",word,index,index_2);

	return;
}

void initSmartWord(char *wordFile){
  create_space();//functions mallocs all possible indexs in hashtables
	char word[100];
	while(fscanf(wordFile,"%s",word)==1){ //reads entire words.txt file
  	int length = strlen(word);
  	int f_c_value,s_c_value;  //first_character_value and second
  	for(int i=0;i<length;i++){  //sets word to lowercase
  		word[i] = tolower(word[i]);
  	}

  	f_c_value = word[0];  //gets ascii numerical value
  	s_c_value = word[1];

  	f_c_value = f_c_value%97; //mods number by 97 to get specific index to put word at in hash tables
  	s_c_value = s_c_value%97;
  	insert(word,f_c_value,s_c_value);
  }

	return;
}

void procOldMsgSmartWord(char *wordFile){
  char word[100];
  while(fscanf(wordFile,"%s",word)==1){  //reads entire old_tweets files
  	int length = strlen(word);
  	int f_c_value,s_c_value; //first_character_value and second
  	int found_ctr;
  	for(int i=0;i<length;i++){  //sets word to lowercase
  		word[i] = tolower(word[i]);
  	}

  	f_c_value = word[0];  //gets ascii numerical value
  	s_c_value = word[1];

  	f_c_value = f_c_value%97; //mods number by 97 to get specific index to put word at in hash tables
  	s_c_value = s_c_value%97;

  	found_ctr = find_word(word,f_c_value,s_c_value);

  	if(found_ctr == 0){ //if word was not found it will insert it into the structure
  		insert(word,f_c_value,s_c_value);
  	}
  }
// https://www.programiz.com/c-programming/examples/remove-characters-string
	return ;
}

int find_word(char word[100],int index,int index_2){
	struct node *temp;
	temp = alpha_hash[index]->range[index_2]->head;

	while(strcmp(temp->pos_word,word) != 0){ //loops through range to see if word is in it
		temp = temp->next;
	}
	if(strcmp(temp->pos_word,word) == 0){  //if word was found in structure
		temp->score = temp->score + 1; //update score
		return 1;
	}
	else{  //if word wasnt already found in structure
		return 0;
	}
}

void print_list(){
	struct node *temp;

	temp = alpha_hash[0]->range[1]->head;
	while(temp != NULL){
		printf("\n%s",temp->pos_word);
		temp = temp->next;
	}
	return;
}


Word *StartGuess = NULL;
Word *EndGuess = NULL;
Word *PrevGuessed = NULL;

/*
  Description for each function, parameter, and return value (if any)
 */


void GetHighestScores(Word *one, Word *two, Word *three, Word *four) {
  // return the highest three scores out of the four
  // this might not be super efficient and could use some help I think
  // 3 words already in list then fourth is checking the lowest ranked of the three guesses if it is higher check if it is also higher then the second if it is check if it is higher than the first, whenever it finds one that it's lower than stop and replace except if it is lower than the lowest one then return null
}

bool guessed(char word[MAX_WORDLEN]) {
  Word *ptr = PrevGuessed;
  while (ptr != NULL) {
    if (strcmp(ptr->pos_word, word)==0) {
      return true;
    }
    ptr = ptr->next;
  }
  return false;
}

void append(char word[MAX_WORDLEN]) {
  Word *newGuess = (Word *)malloc(sizeof(Word));
  strcpy(newGuess->pos_word, word);
}


// Given:
//   letter: letter typed in by the user (a-z, A-Z)
//   letterPosition:  position of the letter in the word, starts from 0
//   wordPosition: position of the word in a message, starts from 0
// Return via a parameter:
//   gueses: NUM_GUESSES (3) word guesses (case-insensive comparison is used)

void guessSmartWord(char letter, int letterPosition, int wordPosition,char guesses[NUM_GUESSES][MAX_WORDLEN+1]) {
  Word *ptr = NULL;
  Word *guess_one;
  Word *guess_two;
  Word *guess_three;
  // pay attention to upper and lowercase input

  if (letterPosition == 0) {
    int hashval = letter % 97;
    //StartGuess = alpha_hash[hashval];
    // StartGuess = the same pointer as the letter in the array
    ptr = StartGuess;
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
  while (ptr->pos_word[letterPosition] != tolower(letter)) {
      ptr = ptr->next;
  }

  StartGuess = ptr;
  guess_one = ptr;
  guess_two = ptr->next;
  guess_three = ptr->next->next; // pay attention to make sure this isn't the end of the list

  while (ptr->pos_word[letterPosition] != tolower(letter)) {
    if (!guessed(ptr->pos_word)) {

    }
    GetHighestScores(guess_one, guess_two, guess_three, ptr);
    ptr = ptr->next;
  }

  // return the three guesses
  strcpy(guesses[0], guess_one->pos_word);
  strcpy(guesses[1], guess_two->pos_word);
  strcpy(guesses[2], guess_three->pos_word);

  // add the three guesses onto the guessed list
  append(guess_one->pos_word);
  append(guess_two->pos_word);
  append(guess_three->pos_word);
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

void feedbackSmartWord(bool isCorrectGuess, char *correctWord) {

}
