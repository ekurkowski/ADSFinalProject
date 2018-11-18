/***********************************************************
 *
 *   DO NOT MODIFY  
 *
 ************************************************************/

/*

  Author: Philip Chan
  Email: pkc@cs.fit.edu

  Usage: evalSmartWord wordFile oldMessageFile newMessageFile 

  Description:

  The goal is to evaluate SmartWord.  The program simulates keystokes
  from a user by reading in newMessageFile, asks for guesses from
  SmartWord, provides feedback on correctness, and measures
  performance.  SmartWord is provided with oldMessageFile for
  initialization.

     a.  Pseudocode for simulating user keystokes.

     processWordFile(wordFile) // a list of English words
     processOldMessages(oldMessageFile)  // old messages that the system has seen

     while not end of newMessageFile // to simulate new messages being typed in
       while not end of line
         if a word has more than 1 letter
           while not end of word and incorrect guesses
   	     guesses = guess(letter, letterPosition, wordPosition) 
	     measure performance for the first 3 guesses (ignore the rest if any)
	     feedback(correctGuess, word)

     report performance

     b.  Measuring Performance

         * average percentage of skipped letters for a word

         * average time per guess
     
         * memory usage--before EvalSmartWord exits.

	 * overall score--accuracy^2/sqrt(time * memory)  



 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "evalSmartWord.h"
#include "smartWord.h"

#define  MAX_LINELEN   1000

void  preprocessingInSmartWord(char* wordFile, char *oldMessageFile);
void  guessingInSmartWord(char *newMessageFile);
float getAccuTimeOfSmartWord(char *word, int wordPosition,
			     clock_t *totalCpuTimeUsed,
			     int *totalNumGuesses);
bool  isCorrectGuess(char guesses[NUM_GUESSES][MAX_WORDLEN+1], char *word);
void  getLettersAndSpaces(char* src, char* dest);
long  getPeakMemory();



int main(int argc, char* argv[])
{
  if (argc != 4)
    {
      fprintf(stderr,
	      "Usage: evalSmartWord wordFile oldMessageFile newMessageFile");
      exit(-1);
    }
  preprocessingInSmartWord(argv[1], argv[2]);
  guessingInSmartWord(argv[3]);
}


// call initSmartWord() and procOldMsgSmartWord() in smartWord
// report time and memory for preprocessing
// wordFile has the dictionary words
// oldMessageFile has old messages

void preprocessingInSmartWord(char* wordFile, char *oldMessageFile)
{
  printf("Preprocessing in SmartWord...\n");

  clock_t startTime, endTime;
  startTime = clock();
  initSmartWord(wordFile);
  procOldMsgSmartWord(oldMessageFile);
  endTime = clock();

  printf("cpu time in seconds (not part of score): %.4e\n",
	 ((double) (endTime - startTime)) / CLOCKS_PER_SEC);
  printf("memory in bytes (not part of score): %ld\n",
	 getPeakMemory());
}


// given a (new) message file, feed smartWord a letter at a time 
// report accuracy, time, memory, and score
void guessingInSmartWord(char *newMessageFile)
{
  printf("Guessing in SmartWord...\n");

  FILE *msgFile = fopen(newMessageFile, "r");
  if (msgFile == NULL)
    {
      fprintf(stderr, "error opening %s\n", newMessageFile);
      perror("guessingInSwartWord()");
      exit(-1);
    }

  char    line[MAX_LINELEN+2],  // a line from the message file
          words[MAX_LINELEN+2], // words on a line
          *word;                // a word 
  float   accuracySum = 0.0;    // sum of accuracy of all words
  int     wordPosition = 0,     // word position on a line 
          totalNumWords = 0,    // # of words in the file 
          totalNumGuesses = 0;  // total # of guesses from smartWord
  clock_t totalCpuTimeUsed = 0; // total cpu time used by smartWord

  /// for each line in the file
  while (fgets(line, MAX_LINELEN, msgFile) != NULL)
    {
      getLettersAndSpaces(line, words); //only letters and spaces for words
      wordPosition=0;
      word = strtok(words, " ");

      // for each word on the line
      while (word != NULL) 
	{
	  //printf("%s\n", word);
	  if (strlen(word) <= MAX_WORDLEN)
	    {
	      accuracySum += getAccuTimeOfSmartWord(word, wordPosition,
						    &totalCpuTimeUsed,
						    &totalNumGuesses);
	    }
	  else
	    {
	      fprintf(stderr, "word is longer than MAX_WORDLEN=%d\n ",
		      MAX_WORDLEN);
	      fprintf(stderr, "skipping: %s\n", word);
	    }
	  wordPosition++;
	  word = strtok(NULL, " ");
	}
      totalNumWords += wordPosition;
    }
  fclose(msgFile);


  //printf("totalNumWords=%d totalNumGuesses=%d\n", totalNumWords, totalNumGuesses);
  if ((totalNumWords > 0) || (totalNumGuesses > 0))
    {
      // calculate average accuracy and cpu time
      // print accuracy, cpu time, memory and score
      float  accuracy = accuracySum * 100.0 / totalNumWords;
      double cpuTime = totalCpuTimeUsed / (double)totalNumGuesses / CLOCKS_PER_SEC;
      long   memory = getPeakMemory();

      if (cpuTime <= 0) // too small to measure, unlikely
	cpuTime = 1.0 / CLOCKS_PER_SEC;
      if (memory <= 0) // too small to measure, highly unlikely
	memory = 1;

      printf("accuracy: %.4f\n", accuracy);
      printf("cpu time in seconds: %.4e\n", cpuTime),
      printf("memory in bytes: %ld\n", memory);
      printf("score: %.5f\n", (accuracy * accuracy) / sqrt(cpuTime * memory));
    }
  else
    printf("%s does not have words with letters\n", newMessageFile);
}



// given a word and position of the word on the line
//   ask smartWord to make gussses
//   add to total cpu time used and total number of guesses
//   return accuracy (% of skipped letters)
float getAccuTimeOfSmartWord(char *word, int wordPosition,
			     clock_t *totalCpuTimeUsed,
			     int *totalNumGuesses)
{
  int     letterPosition = 0,                // letter position in a word
          wordLength = strlen(word);         // length of word 
  bool    isCorrect = false;      // true if one of the guesses is correct
  float   accuracy = 0.0;                       // % of skipped letters
  clock_t startTime, endTime;

  char    guesses[NUM_GUESSES][MAX_WORDLEN+1];  // guesses from smartWord
  // prevent feedbackSmartWord() from changing the correct "word" 
  char    wordCopy[MAX_WORDLEN+1];
  strcpy(wordCopy, word);
  
  // for each letter of the word and while guesses are not correct
  while (letterPosition < wordLength && isCorrect == false) 
    {
      startTime = clock();
      // ask smartWord to guess
      guessSmartWord(word[letterPosition], letterPosition, wordPosition, guesses);
      endTime = clock();

      (*totalCpuTimeUsed) += (endTime - startTime);
      (*totalNumGuesses)++;

      isCorrect = isCorrectGuess(guesses, word);
      if (isCorrect) // correct, calculate the % of letters skipped
	{
           feedbackSmartWord(true, wordCopy);
           accuracy = (wordLength - (letterPosition + 1)) / (float)wordLength;
        } 
      else // incorrect
	{
	  if (letterPosition < (wordLength - 1))
            feedbackSmartWord(false, NULL);
	  else  // all the letters have been provided 
	    feedbackSmartWord(false, wordCopy);
          letterPosition++;
	}
    }
  //printf("  accuracy: %.5f\n", accuracy);
  return accuracy;
}


// return true if word is one of the guesses
bool isCorrectGuess(char guesses[NUM_GUESSES][MAX_WORDLEN+1], char *word)
{
  bool isCorrect = false;
  int  guessIndex = 0;

  while (guessIndex < NUM_GUESSES && !isCorrect)
    {
      if (strncasecmp(word, guesses[guessIndex], strlen(word)) == 0)
	{
           isCorrect = true;
           //printf("  correct: %s\n", word);
        }
      else
	guessIndex++;
    }
  return isCorrect;
}
  

// given the source string (src),
// the destination string (dest) has only letters or spaces
void getLettersAndSpaces(char* src, char* dest)
{
  int destIndex = 0,
      srcLen = strlen(src);
  
  for (int srcIndex = 0; srcIndex < srcLen; srcIndex++)
    {
      char c = src[srcIndex];
      if ( isalpha(c) || (c == ' ') )
	{
	  dest[destIndex++] = c;
	}
    }
  dest[destIndex] = '\0';  // terminate the destination string
}









/**********************************************************************
Different versions of getPeakMemory() for different operating systems,
selected by #define at the top of this file.

LINUX
MAC_OS
WINDOWS

OTHERS: getPeakMemory() is not supported
***********************************************************************/

#ifdef LINUX
// return peak usage of virtual memmory in bytes
long getPeakMemory()
{
  FILE *procFile = fopen("/proc/self/status", "r");
  if (!procFile)
    {
       perror("/proc/self/status: ");
       return -1;
    }

  long peakVirtualMemory=0;    // VmPeak
  char line[128];
  bool done = false;

  while ((fgets(line, 128, procFile) != NULL) && (done == false))
    {
      // "Peak virtual memory size"
      if (strncmp(line, "VmPeak:", 7) == 0)  // check the first 7 chars
	{
	  sscanf(line+7, "%ld", &peakVirtualMemory);  // in kB
	  done = true;
	}
    }
  fclose(procFile);
  return peakVirtualMemory * 1024;
}
#endif


#ifdef MAC_OS
#include <sys/time.h>
#include <sys/resource.h>
// return max resident set size
long getPeakMemory()
{
  struct rusage usage;
  
  if(getrusage(RUSAGE_SELF, &usage) == 0)
    //"the maximum resident set size utilized (in kilobytes)"
    return (long)usage.ru_maxrss * 1024;
  else 
    return -1;
}
#endif


#ifdef WINDOWS
#include <windows.h>
#include <psapi.h>

// return total amount of memory committed in bytes
long getPeakMemory()
{
  PROCESS_MEMORY_COUNTERS info;

  if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)) != 0)
    {
      //"The peak value in bytes of the Commit Charge during the lifetime of this process."
ge);
      return (long)info.PeakPagefileUsage;
    }
  else
    { 
      fprintf(stderr, "GetProcessMemoryInfo() error\n");
      return -1;
    }
}
#endif


#ifdef OTHERS

// return total amount of memory committed in bytes
long getPeakMemory()
{  
  fprintf(stderr, "getPeakMemory() not supported\n");
  return -1;

}
#endif
