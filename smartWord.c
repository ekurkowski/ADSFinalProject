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

int find_word(char word[100],int index,int index_2);
void remove_special_c(char word[100],int len);

// first table for the alphabet
struct hash{
	struct inter *range[26]; // another table within the table to hold the second letter of the alphabet
}*alpha_hash[26];

struct inter{ // list of alphabetized words that all start with the same first two letters
	struct node *head;
	struct node *tail;
};

// node to hold actual word and the score of the word
typedef struct node{
	char pos_word[MAX_WORDLEN]; // word used
	int score; // number of times used
	struct node *next;
} Word;

// allocates memory for the hash and subtable
void create_space(){
    int i;
	for(i=0;i<26;i++){
		alpha_hash[i] = (struct hash*) malloc(sizeof(struct hash));
        int j;
		for(j=0;j<26;j++){
			alpha_hash[i]->range[j] = (struct inter*) malloc(sizeof(struct inter));
            alpha_hash[i]->range[j]->head = NULL;
            alpha_hash[i]->range[j]->tail = NULL;
		}
	}
	return;
}

// inserts words from the words.txt file. Already alphabetized, so don't spend time checking
void insert_wordsfile(char word[100],int index,int index_2){
	struct inter *list = alpha_hash[index]->range[index_2];
    
	Word *new_node = (Word*) malloc(sizeof(Word));
	strcpy(new_node->pos_word,word);
	new_node->next = NULL;
	new_node->score = 0;

	if(list->head == NULL){ //if list is empty in index
		list->head = new_node;
		list->tail = new_node;
	} else { //if list is not empty
		list->tail->next = new_node;
		list->tail = new_node;
	}

	return;
}

// inserts words form the old messages in alphabetical order
void insert_oldMes(char word[100],int index,int index_2, int score){
	struct node *temp,*prev = NULL;
	struct node *list = (struct node*) alpha_hash[index]->range[index_2]->head;
	struct node *new_node = (struct node*) malloc(sizeof(struct node));
	strcpy(new_node->pos_word,word);
	new_node->next = NULL;
	new_node->score = score;
	

	if(list == NULL){ //if list is empty 
		//printf(" F");
		//printf("\n%s",word);
		alpha_hash[index]->range[index_2]->head = new_node;
		alpha_hash[index]->range[index_2]->tail = new_node;
		
	}
	else{ //if list HAS MORE THAN 1 item
		//printf("\n MORe");
		temp = alpha_hash[index]->range[index_2]->head;
		//printf("\n%s %s",temp->pos_word,word);
		while(temp->next != NULL && (strcmp(temp->pos_word,word)<0)){//try to find place to add in list
			if(strcmp(temp->pos_word,word) == 0){ //word already in 
				return;
			}
			prev = temp;
			temp = temp->next;
		}
		if(strcmp(temp->pos_word,word) == 0){ //word already in 
			return;
		}
		if((strcmp(temp->pos_word,word) > 0) && (strcmp(temp->pos_word,alpha_hash[index]->range[index_2]->head->pos_word) == 0)){ //if added to front
			//printf(" here1");
			//printf("\n%s",word);
			new_node->next = temp;
			alpha_hash[index]->range[index_2]->head = new_node;
		}
		else if(temp->next == NULL && (strcmp(temp->pos_word,word)<0)){ //if added to end
			//printf(" here2");
			//printf("\n%s",word);
			temp->next = new_node;
			alpha_hash[index]->range[index_2]->tail = new_node;
		}
		else{ //added to middle
			//printf(" here3");
			//printf("\n%s",word);
			prev->next = new_node;
			new_node->next = temp;
		}
	}
	
	return;
}

// initializes smart word by creating the database and inserting the given words
void initSmartWord(char *wordFile){

  FILE *in_file;
  in_file = fopen(wordFile, "r");
	if (in_file == NULL) {
		printf("error opening file");
	}
  create_space();//functions mallocs all possible indexs in hashtables
	char word[100];
	while(fscanf(in_file,"%s",word)==1){ //reads entire words.txt file
        int length = (int)strlen(word);
        int f_c_value,s_c_value;  //first_character_value and second
        int i;
        for(i=0;i<length;i++){  //sets word to lowercase
            word[i] = tolower(word[i]);
        }

        f_c_value = word[0];  //gets ascii numerical value
        
        
        if (length == 1) {
            s_c_value = word[0];
        } else {
            s_c_value = word[1];
        }

        f_c_value = f_c_value%97; //mods number by 97 to get specific index to put word at in hash tables
        s_c_value = s_c_value%97;
        insert_wordsfile(word,f_c_value,s_c_value);
      }
	return;
}

// inserts the words from the old message file into the database
void procOldMsgSmartWord(char *wordFile){
  FILE *in_file;
  in_file = fopen(wordFile, "r");
  char word[100];
  while(fscanf(in_file,"%s",word)==1){  //reads entire old_tweets files
  	int length = (int)strlen(word);
  	int f_c_value,s_c_value; //first_character_value and second
  	int found_ctr;
      int i;
  	for(i=0;i<length;i++){  //sets word to lowercase
  		word[i] = tolower(word[i]);
  	}
	
	remove_special_c(word,length); //removes character
	
	if(word[0] == '0'){ //checks if its not even a word
		
		return;
	}

  	f_c_value = word[0];  //gets ascii numerical value
  	s_c_value = word[1];

  	f_c_value = f_c_value%97; //mods number by 97 to get specific index to put word at in hash tables
  	s_c_value = s_c_value%97;

  	found_ctr = find_word(word,f_c_value,s_c_value);

  	if(found_ctr == 0){ //if word was not found it will insert it into the structure
  		insert_oldMes(word,f_c_value,s_c_value, 2);
  	}
  }
	return ;
}

// sees if the word is already in database before adding. Avoids duplicate words and wasted space
int find_word(char word[100],int index,int index_2){
	struct node *temp;
	temp = alpha_hash[index]->range[index_2]->head;
	if(temp == NULL){
		return 0;
	}
	while(strcmp(temp->pos_word,word) < 0 && temp->next != NULL){ //loops through range to see if word is in it
		temp = temp->next;
	}
	if(strcmp(temp->pos_word,word) == 0){  //if word was found in structure
		if(temp->score == 0){
			temp->score = temp->score + 1;  //increment by 1 if already at 0
		}
		else if(temp->score >=1){
			temp->score = temp->score + 2; //update score by a incremneting score by 2 if already at 1
		}
		return 1;
	} 
	else if(temp->next == NULL){  //if word wasnt already found in structure
		return 0;
	}
    return 0;
}

// removes all characters from a string that are not letters
void remove_special_c(char word[100],int len){
	int zero_ctr=0;
    int i;
	for(i=0;i<len;i++){
		if(word[i] < 'a' || word[i] > 'z'){
			word[i] = '0';
		}
	}
	if(zero_ctr == len){ //word is not an accpetable word
		len = 1;
		word[0] = '0';
		word[1] = '\0';
		len = 1;
		return;
	}
	for(i=0;i<len;i++){
		if(word[i] == '0'){
            int j;
			for(j=i;j<len;j++){
				word[j] = word[j+1];
			}
			word[len-1] = '\0';
			len--;
		}
		if(word[i] == '0'){
            int j;
			for(j=i;j<len;j++){
				word[j] = word[j+1];
				word[j+1] = ' ';
			}
			word[len-1] = '\0';
			len--;
		}
	}
	if(word[0] == '0' && (word[1] > 'a' && word[1] < 'z')){ //if 1 character didnt get deleted
        int j;
		for(j=0;j<len;j++){
			word[j] = word[j+1];
		}
		word[len-1] = '\0';
		len--;
	}
	
	if(len==0){ //if all characters git deleted it wasnt a word
		len = 1;
		word[0] = '0';
		word[1] = '\0';
	}
	
	return;
}

struct inter *StartGuess = NULL; // keeps track of where word being guessed begins
Word *beginGuess = NULL;   // keeps track of second letter of word being guessed
Word *PrevGuessed = NULL; // list of the previously guessed for each word. Resets after word has been guessed
int hashval = 0; // which letter the word starts with

// return the highest three scores out of the four
void GetHighestScores(Word *one, Word *two, Word *three, Word *four) {
  Word *temp;
  if (three->score < four->score) {
	  if (two->score < four->score) {
		  if (one->score < four->score) {
			  temp = one;
			  one = four;
			  three = two;
			  two = temp;
		  } else {
			  temp = two;
			  two = four;
			  three = temp;
		  }
	  } else {
		  three = four;
	  }
  } else {
	  return;
  } 
}

// checks to see if given word has been guessed already.
bool guessed(char word[MAX_WORDLEN]) {
  Word *ptr = PrevGuessed;
  while (ptr != NULL) {
    if (strcmp(ptr->pos_word, word)==0) {
      return true; // returns true if it has been guessed
    }
    ptr = ptr->next;
  }
  return false; // false if it has not been guessed
}

// adds the word onto the list of guessed words
void append(char word[MAX_WORDLEN]) {
  Word *newGuess = (Word *)malloc(sizeof(Word));
  strcpy(newGuess->pos_word, word);

  newGuess->next = PrevGuessed;
  PrevGuessed = newGuess;
}


// Given:
//   letter: letter typed in by the user (a-z, A-Z)
//   letterPosition:  position of the letter in the word, starts from 0
//   wordPosition: position of the word in a message, starts from 0
// Return via a parameter:
//   gueses: NUM_GUESSES (3) word guesses (case-insensive comparison is used)

void guessSmartWord(char letter, int letterPosition, int wordPosition,char guesses[NUM_GUESSES][MAX_WORDLEN+1]) {
  Word *ptr = NULL;
  Word *guess_one; // pointers to each of the nodes containing the possible guesses
  Word *guess_two;
  Word *guess_three;
  letter = tolower(letter);
  // pay attention to upper and lowercase input, this standardizes the letters to lowercase

  if (letterPosition == 0) { // first letter relates to first hash table of any word that begins with the given letter
    hashval = letter % 97;
    StartGuess = alpha_hash[hashval]->range[0];
      
      // these catches for unknown are in case there are not at least three guessed that even fit the given letters
    guess_one = StartGuess->head;
      if (guess_one == NULL) {
          strcpy(guesses[0], "unknown");
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          return;
      }
    guess_two = StartGuess->head->next;
      if (guess_two == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          return;
      }
    guess_three = StartGuess->head->next->next;
      if (guess_three == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], guess_two->pos_word);
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          append(guesses[1]);
          return;
      }

      // iterates through all of the words that begin with this letter in the whole database
    int i = 0;
    for (i = 0; i < 26; i++) {
      ptr = alpha_hash[hashval]->range[i]->head;

      while (ptr != NULL) {
        if (!guessed(ptr->pos_word)) {
            // keeps the highest three scored words out of four
          GetHighestScores(guess_one, guess_two, guess_three, ptr);
        }
        ptr = ptr->next;
      // iterate through all of the possible words and return highest three
      }
    }
  } else if (letterPosition == 1) {
    // move StartGuess to the beginning of that linked list in the sub array. all possible words start with same two letters
    StartGuess = alpha_hash[hashval]->range[letter % 97];
    beginGuess = StartGuess->head;
    ptr = beginGuess;

      // catch in case not three words in database that start with given first and second letter
    guess_one = ptr;
      if (guess_one == NULL) {
          strcpy(guesses[0], "unknown");
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          return;
      }
    guess_two = ptr->next;
      if (guess_two == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          return;
      }
      
    guess_three = ptr->next->next;
      // if we do not have the word in our database, returns unknown
      if (guess_three == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], guess_two->pos_word);
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          append(guesses[1]);
          return;
      }

      // iterates through every word that begins with the two letters given
    while (ptr != StartGuess->tail) {
      if (!guessed(ptr->pos_word)) {
        GetHighestScores(guess_one, guess_two, guess_three, ptr); // keeps highest scoring words that haven't been guessed yet
      }
      ptr = ptr->next;
    }
  } else {

      // moves down the list to the next letter, slowly fewer and fewer guesses as we get more letters
      ptr = beginGuess;
      if (beginGuess == NULL) {
          strcpy(guesses[0], "unknown");
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          return;
      }
      // move beginGuess to where it matches the letter of the word in alphabetical order
      while (ptr->pos_word[letterPosition] != letter && ptr->pos_word[letterPosition-1] == beginGuess->pos_word[letterPosition-1]) {
          ptr = ptr->next;
          if (ptr == NULL) {
              strcpy(guesses[0], "unknown");
              strcpy(guesses[1], "unknown");
              strcpy(guesses[2], "unknown");
              return;
          }
      }

      // catches in case there are not enough words that fit the criteria
      beginGuess = ptr;
      guess_one = ptr;
      if (guess_one == NULL) {
          strcpy(guesses[0], "unknown");
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          return;
      }
      guess_two = ptr->next;
      if (guess_two == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], "unknown");
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          return;
      }
      
      guess_three = ptr->next->next;
      // if we do not have the word in our database, returns unknown
      if (guess_three == NULL) {
          strcpy(guesses[0], guess_one->pos_word);
          strcpy(guesses[1], guess_two->pos_word);
          strcpy(guesses[2], "unknown");
          append(guesses[0]);
          append(guesses[1]);
          return;
      }

      // iterates through possible words
      while (ptr->pos_word[letterPosition] == letter) {
        if (!guessed(ptr->pos_word)) {
          GetHighestScores(guess_one, guess_two, guess_three, ptr); // returns highest scoring three of the four for each word
        }
        ptr = ptr->next;
          if (ptr == NULL) {
              break;
          }
      }
    }

  // return the three guesses
  strcpy(guesses[0], guess_one->pos_word);
  strcpy(guesses[1], guess_two->pos_word);
  strcpy(guesses[2], guess_three->pos_word);

  // add the three guesses onto the guessed list
  append(guess_three->pos_word);
  append(guess_two->pos_word);
  append(guess_one->pos_word);
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

// used to help improve accuracy and help our algorithm learn
void feedbackSmartWord(bool isCorrectGuess, char *correctWord) {
    if (isCorrectGuess == 1) { // if we guess correctly, increments the score of the word and frees the memory from the previously guessed list
        free(PrevGuessed);
        PrevGuessed = NULL;
        int index_1, index_2;
        int i;
        for(i=0;i<strlen(correctWord);i++){  //sets word to lowercase
            correctWord[i] = tolower(correctWord[i]);
        }
        index_1 = correctWord[0] % 97;
        index_2 = correctWord[1] % 97;
        find_word(correctWord, index_1, index_2);
    }
    if (isCorrectGuess == 0 && correctWord != NULL) { // if we guess incorrectly, increments to word so it has been used more, or add the word to the list if it is not in our database
        free(PrevGuessed);
        PrevGuessed = NULL;
        int index_1, index_2;
        int i;
        for(i=0;i<strlen(correctWord);i++){  //sets word to lowercase
            correctWord[i] = tolower(correctWord[i]);
        }
        index_1 = correctWord[0] % 97;
        index_2 = correctWord[1] % 97;
        if (!find_word(correctWord, index_1, index_2)) {
            insert_oldMes(correctWord, index_1, index_2, 3);
        }
    }
}
