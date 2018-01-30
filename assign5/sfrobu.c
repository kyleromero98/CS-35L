/*
Name: Kyle Romero
UID: 204747283
Section: Laboratory 7
TA: Tan

Homework 5 - Encrypted Sort Revisited
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int comparisons = 0;

// Checks for a I/O error
void checkReadError (ssize_t bytes) {
  if (bytes < 0) {
    fprintf(stderr, "Error: read error");
    exit(1);
  }
}

// Checks for memory errors
void checkMemError (void* pointer) {
  if (pointer == NULL) {
    fprintf(stderr, "Error: Memory allocation error\n");
    exit(1);
  }
}

// Compares two strings and returns 0, -1, or 1
int frobcmp (char const* a, char const* b, bool ignoreCase) {
  char atemp, btemp, adecode, bdecode;
  int count = 0;
  for (;;) {
    atemp = a[count];
    btemp = b[count];

    // noting in spec how to decode the strings
    adecode = atemp ^ 42;
    bdecode = btemp ^ 42;

    // check for ignoring case or not
    if (ignoreCase) {
      if (islower(adecode))
	adecode = toupper(adecode);
      if (islower(bdecode))
	bdecode = toupper(bdecode);
    }

    // both spaces, or less or longer/shorter
    if (atemp == ' ' && btemp == ' ')
      return 0;
    if (adecode < bdecode || (atemp == ' ' && btemp != ' '))
      return -1;
    if (bdecode < adecode || (atemp != ' ' && btemp == ' '))
      return 1;
    count++;
  }
  return 0;
}

// wrapper for ignoring case
int frobcmpigcase_wrapper(const void* a, const void* b) {
  comparisons++;
  return frobcmp(*((const char**) a), *((const char**) b), true);
}

// wrapper for not ignoring case
int frobcmp_wrapper(const void* a, const void* b) {
  comparisons++;
  return frobcmp(*((const char**) a), *((const char**) b), false);
}

// takes from stdin and decodes/sorts
int main (int argc, char** argv) {
  // check for argument errors
  if (argc > 2) {
    fprintf(stderr, "Error: sfrobu accepts only one argument\n");
    exit(1);
  }

  bool ignoreCase = false;
  // Check for -f option
  if (argc == 2 && strcmp(argv[1], "-f") != 0) {
    fprintf(stderr, "Error: You entered an invalid option\n");
    exit(1);
  } else if (argc == 2 && strcmp(argv[1], "-f") == 0) {
    ignoreCase = true;
  }
  
  struct stat fileStats;

  // get the file stats
  if (fstat(0, &fileStats) != 0) {
    fprintf(stderr, "Error: Information gathering failed\n");
    exit(1);
  }

  ssize_t filelength = fileStats.st_size;
  ssize_t linesize = 25;

  char** filewords;
  char* fileinput;
  ssize_t inputCounter = 0;
  ssize_t buffersize = 0;
  int wordCount = 0;

  // check for regular file
  if (S_ISREG(fileStats.st_mode)) {
    fileinput = (char*)malloc((filelength + linesize) * sizeof(char));
    checkMemError(fileinput);

    // if we read more than one byte
    if (read(0, fileinput, filelength) > 0) {
      // allocate how many we think we have
      char* temp = (char*)malloc((filelength + 1) * sizeof(char));
      checkMemError(temp);
      buffersize = filelength;
      // remove all excess white space
      for (size_t i = 0; i < filelength; i++) {
        if (fileinput[i] != ' ' || (i > 0 && fileinput[i] == ' ' && fileinput[i - 1] != ' ')) {
	  if (fileinput[i] == ' ')
	    wordCount++;
	  temp[inputCounter] = fileinput[i];
	  inputCounter++;
        }
      }
      // remove old input with lots of white space
      char* del = fileinput;
      free(del);
      fileinput = temp;
    }
  } else {// else we have a weird file
    fileinput = (char*)malloc(sizeof(char) * linesize);
    buffersize = linesize;
  }

      // get more chars and check for expanding input
    char currentChar[1];
    for (int i = inputCounter;; i++) {
      // if no more chars to read
      if (read(0, currentChar, 1) <= 0 || currentChar[0] == EOF)
	break;

      // we need to expand
      if (i == buffersize) {
	buffersize *= 2;
	fileinput = (char*)realloc(fileinput, buffersize * sizeof(char));
	checkMemError(fileinput);
      }

      // check for space
      if (currentChar[0] == ' ') {
	// unnecessary space so don't add
	if (inputCounter > 0 && currentChar[i - 1] == ' ')
	  continue;
	else if (inputCounter > 0) { // space is necessary
	  fileinput[inputCounter++] = currentChar[0]; // add the space
	  wordCount++;
	}
      } else {
	// normal char, so append
	fileinput[inputCounter++] = currentChar[0];
      }
    }
    
    // checking last place for space
    if (fileinput[inputCounter - 1] != ' ') {
      fileinput[inputCounter++] = ' ';
      wordCount++;
    }

    // construction the wordArr to store the words
    char** wordArr = (char**)malloc(sizeof(char*) * (wordCount + 1));
    checkMemError(wordArr);

    int newWordCount = 0;

    // add the words to the wordArr according to the spaces in fileinput
    for (int start = 0, end = 0; end < inputCounter; end++) {
      if (fileinput[end] == ' ') {
	wordArr[newWordCount] = (char*)malloc(sizeof(char) * ((end - start) + 1));
	checkMemError(wordArr[newWordCount]);
	for (int j = start; j <= end; j++) {
	  wordArr[newWordCount][j - start] = fileinput[j]; 
	}
	newWordCount++;
	start = end + 1;
      }
    }

    // sort depending on ignorecase or not
    if (ignoreCase)
      qsort(wordArr, newWordCount, sizeof(char*), frobcmpigcase_wrapper);
    else
      qsort(wordArr, newWordCount, sizeof(char*), frobcmp_wrapper);

    // print out the words
    for (int i = 0; i < newWordCount; i++) {
      char* temp = wordArr[i];
      for (;;) {
	if (write(1, temp, 1) <= 0) {
	  fprintf(stderr, "Error writing one byte\n");
	  exit(1);
	}
        if (*temp++ == ' ')
	  break;
      }
      // free now so we don't have to iterate later
      free(wordArr[i]);
    }

    // make sure we know how many comparisons were done
    // fprintf(stderr, "Comparisons: %d\n", comparisons);

    free(fileinput);
    free(wordArr);
}
