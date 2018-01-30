/*
Name: Kyle Romero
UID: 204747283
Section: Laboratory 7
TA: Tan

Homework 4 - Sorting Encrypted Text
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define DEFAULT_SIZE 20

// Checks for a I/O error
void checkIOError (FILE* p) {
  if (ferror(p))
    exit(1);
}

// Checks for memory errors
void checkMemError (void* pointer) {
  if (pointer == NULL)
    exit(1);
}

// Compares two strings and returns 0, -1, or 1
int frobcmp (char const* a, char const* b) {
  while (*a == *b) {
    if (*a == ' ')
      return 0;
    a++;
    b++;
  }
  // derived from spec, noting that '*' turns into null byte
  if ((*a ^ 42) < (*b ^ 42))
    return -1;
  return 1;
}

int cmpWrapper(const void* a, const void* b) {
  return frobcmp(*((const char**) a), *((const char**) b));
}

void formatLineBuff (char** lineinput, char* buf, size_t size) {
  size_t i = 0, lineCount = 0;
  char* currentLine = buf;
  for (; i < size; i++) {
    if (buf[i] == ' ') {
      lineinput[lineCount] = currentLine;
      currentLine = buf + i + 1;
      lineCount++;
    }
  }  
}

void putString (const char* str) {
  while (1) {
    putchar(*str);
    checkIOError(stdout);
    if (*str++ == ' ')
      return;
  }
}

// takes from stdin and decodes/sorts
int main (void) {
  size_t filesize = DEFAULT_SIZE;
  
  char* input = (char*)malloc(sizeof(char) * filesize);
  checkMemError(input);
  char* temp;

  size_t buffersize = 0, lineNum = 0, linesize = 0;

  // Reading from stdin
  while (!feof(stdin)) {
    // get next char/error checking
    char currentChar = getchar();
    checkIOError(stdin);

    // did we just hit a space?
    if (!linesize && (currentChar == ' '))
      continue;
    
    // Check if we need to reallocate b/c of expanding buffer
    if (buffersize == filesize) {
      filesize *= 2;
      temp = (char*) realloc(input, sizeof(char) * filesize);
      checkMemError(temp);
      input = temp;
    }

    // Add chars to stored buffer
    if (!feof(stdin)) {
      input[buffersize++] = currentChar;
      linesize++;
      if (currentChar != ' ') {
	continue;
      }
    } else {
      if (!buffersize) {
	free(input);
	return 0; // empty file/only spaces
      }
      if (input[buffersize - 1] != ' ')
	input[buffersize++] = ' ';
      if (!linesize)
	break;
    }
    lineNum++;
    linesize = 0;
  }
   
  char** lineinput = (char**)malloc(sizeof(char*) * lineNum);
  checkMemError(lineinput);

  // Formats for output
  formatLineBuff(lineinput, input, buffersize);
  
  // sort w/ wrapper function
  int (*compare) (const void*, const void*) = &cmpWrapper;
  qsort(lineinput, lineNum, sizeof(char*), compare);
  
  // output results
  for (int i = 0; i < lineNum; i++) {
    putString(lineinput[i]);
  }

  // Free everything
  free(lineinput);
  free(input);
  return 0;
}
