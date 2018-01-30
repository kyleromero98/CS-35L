#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
  // error check for incorrect num of arguments
  if (argc != 3) {
    fprintf(stderr, "Error: Incorrect number of arguments: %d, requires 2\n", argc);
    exit(1);
  }

  // getting arguments
  char* from = argv[1];
  char* to = argv[2];

  // save both lengths for later
  size_t fromlength = strlen(from);
  size_t tolength = strlen(to);

  // Checking for missing operand
  if (fromlength == 0) {
    fprintf(stderr, "Error: Nothing was passed in stdin\n");
    exit(1);
  }
  
  // Checking for differing length
  if (fromlength != tolength) {
    fprintf(stderr, "Error: from and to have differing lengths\n");
    exit(1);
  }

  // error check for repeated bytes in from
  for (size_t i = 0; i < fromlength; i++) {
    for (size_t j = i + 1; j < fromlength; j++) {
      if (from[i] == from[j]) {
	fprintf(stderr, "Error: from contains duplicate bytes\n");
	exit(1);
      }
    }
  }

  // get initial char
  char currentChar[1];
  ssize_t numBytesRead = read(0, currentChar, 1);

  while (numBytesRead > 0) {
    // look for input match in from and map to to
    for (size_t i = 0; i < fromlength; i++) {
      if (currentChar[0] == from[i]) {
	// found, so map to to
	currentChar[0] = to[i];
       	break;
      }
    }
    // not found, so just print original char
    write(1, currentChar, 1);
    numBytesRead = read(0, currentChar, 1);
  }
  return 0;
}
