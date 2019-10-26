/* hw2.c */
/* NAME: Xinhao Luo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Operations
#define OP_PLUS '+'
#define OP_MINUS '-'
#define OP_TIMES '*'
#define OP_DIVIDED '/'
#define OP_EQ '='
// Registers
#define REG_NUM 0
#define REG_T 1
#define REG_S 2
#define REG_ZERO 3
// Syntax
#define ENDLINE ';'
// Init value
#define UNREG -1
// Boundaries
#define MAX_T_REG 9

// Global Variables
// Just dont want to pass them around
int s_register = 0;
int t_register = 0;
int alphabet[26];

// Since each alphabet will only appear once, we can use an char[26] to track them
int alphabet2num(char a) {
  return a - 'a';
}

int isPowerOf2(int num) {
  if (num % 2 != 0){
    // not the power
    return -1;
  }

  int count = 0;
  while (num != 1) {
    num = num / 2;
    count++;
  }
  return count;
}

int findPowers(int x, int *powers) {
  // Allocate memory
  powers = malloc(100 * sizeof(int));

  int size = 0;
  while (x > 0) {
    powers[size++] = x % 2;
    x = x / 2;
  }

  if (x == 0) {
    // Return the size of array
    return size;
  } else {
    return 0;
  }
}

// Register var if does not appear in the s list
void register_var(int* alphabet, const char current, int *s_register) {
  int num = alphabet2num(current);
  if (alphabet[num] == UNREG) {
    alphabet[num] = *s_register;
    (*s_register)++;
  }
}

// Collect immediate value
unsigned long read_all_digits(const char* line, unsigned long *i, unsigned long strLen) {
  // Read All Digit
  unsigned int index = 0;
  char digits[100];
  for (unsigned long j = *i; j < strLen; j++) {
    if (isdigit(line[j])) {
      digits[index] = line[j];
      index++;
    } else {
      digits[index] = '\0';
      *i = j - 1;
      break;
    }
  }

  return atoi(digits);
}


int main( int argc, char * argv[] )
{
  char line[256];
  FILE *fp;
  fp = fopen(argv[1], "r");

  // check if the file can be opened
  if (!fp) {
    fprintf(stderr, "Error: Cannot open file!\n");
    return EXIT_FAILURE;
  }

  // Init s-register
  for (int i = 0; i < 26; i++) {
    alphabet[i] = UNREG;
  }

  while (fgets(line, 255, (FILE*)fp) != NULL) {
    // print the line
    printf("# %s",line);
    // Remove new line
    line[strcspn( line, "\n" )] = '\0';

    unsigned long strLen = strlen(line);
    // The last one should be an ENDLINE
    if (line[strLen - 1] != ENDLINE) {
      fprintf( stderr, "Parsing error...\n" );
      return EXIT_FAILURE;
    }

    int stage = 0; // 0
    // Line Loop Begin
    // Identify Types of command
    // you can assume that each assignment statement will consist
    // of a mix of addition and subtraction operations, a mix of multiplication or division operations, or
    // a simple assignment (e.g., x = 42).
    // Example: f = g + h - 42;
    // g = 100;
    for (unsigned long i = 0; i < strLen; i++) {
      char c = line[i];
      if (c == ENDLINE) {
        stage = 4;
      } else if(isspace(c)) {
        continue;
      } else if (islower(c)) {
        if (stage == 0 || stage == 2) {
          register_var(alphabet, c, &s_register);
          stage++;
        } else {
          fprintf( stderr, "Parsing error...\n" );
          return EXIT_FAILURE;
        }
      } else if (isdigit(c)) {
        if (stage == 0 || stage == 2) {
          unsigned long num = read_all_digits(line, &i, strLen);
          stageNMapper(stage, &current_var1, &current_var2, num);
          stage++;
        } else {
          fprintf( stderr, "Parsing error...\n" );
          return EXIT_FAILURE;
        }
      } else if(c == OP_EQ) {
        if (stage == 1) {
          stage = 0; // Reset stage: the true expression comes!
        } else {
          // Wait! LHS should have only one var, right?
          fprintf( stderr, "Parsing error...\n" );
          return EXIT_FAILURE;
        }
      } else if (c == OP_MINUS){
        if (stage == 1) {
          current_op = OP_MINUS;
          stage++;
        } else {
          fprintf( stderr, "Parsing error...\n" );
          return EXIT_FAILURE;
        }
      } else if (c == OP_PLUS) {
        if (stage == 1) {
          current_op = OP_PLUS;
          stage++;
        } else {
          fprintf( stderr, "Parsing error...\n" );
          return EXIT_FAILURE;
        }
      } else {
        fprintf( stderr, "Parsing error...\n" );
        return EXIT_FAILURE;
      } // End of registering vars
    }
  }

  fclose(fp);
  return EXIT_SUCCESS;
}
