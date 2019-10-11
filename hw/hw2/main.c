/* hw2.c */
/* NAME: Xinhao Luo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Operations
#define OP_PLUS '+'
#define OP_MINUS '-'
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

// A record is a number - immediate value(int), a $s register(for vars), or a $t register(for temp).
struct Record
{
    int type;
    int data;
};

// a line of MIPS code
struct Line
{
    char operation;
    struct Record first;
    struct Record second;
    struct Record third;
};

// Since each alphabet will only appear once, we can use an char[26] to track them
int alphabet2num(char a) {
  return a - 'a';
}

void printRecord(struct Record r) {
  switch(r.type) {
    case REG_NUM:
      printf("%d", r.data);
      break;
    case REG_S:
      printf("$%c%d", 's', r.data);
      break;
    case REG_T:
      printf("$%c%d", 't', r.data);
      break;
    case REG_ZERO:
      printf("$zero");
      break;
    default:
      fprintf(stderr, "Unrecognized Record type: does something goes wrong?\n");
  }
}

void printLine(struct Line l) {
  // Operation
  if (l.operation == OP_PLUS) {
    if (l.third.type == REG_NUM) {
      printf("addi ");
    } else {
      printf("add  ");
    }
  } else if (l.operation == OP_MINUS) {
    if (l.third.type == REG_NUM) {
      printf("addi ");
    } else {
      printf("sub  ");
    }
  } else {
    fprintf(stderr, "Unrecognized Operation type: does something goes wrong?\n");
  }

  // Vars
  printRecord(l.first);
  printf(",");
  printRecord(l.second);
  printf(",");
  if (l.third.type == REG_NUM && l.operation == OP_MINUS) {
    // Add minus when minus operation and it is an immediate value
    printf("-");
  }
  printRecord(l.third);
  printf("\n");
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

// Register S var to based on current stage
void stageSMapper(int stage, struct Record *current_var1, struct Record *current_var2, int s_register) {
  switch (stage){
    case 0:
      current_var1->type = REG_S;
      current_var1->data = s_register;
      break;
    case 2:
      current_var2->type = REG_S;
      current_var2->data = s_register;
      break;
    default:
      fprintf( stderr, "Unknown Stage: does something goes wrong?\n" );
      break;
  }
}

// Register immediate var to based on current stage
void stageNMapper(int stage, struct Record *current_var1, struct Record *current_var2, int intdata) {
  switch (stage){
    case 0:
      current_var1->type = REG_NUM;
      current_var1->data = intdata;
      break;
    case 2:
      current_var2->type = REG_NUM;
      current_var2->data = intdata;
      break;
    default:
      fprintf( stderr, "Unknown stage: does something goes wrong?\n" );
      break;
  }
}

int main( int argc, char * argv[] )
{
  char line[256];

  printf("Please enter a valid C assignment statement:\n");
  if (fgets(line, 256, stdin) == NULL)
  {
    fprintf(stderr, "fgets() failed");
    return EXIT_FAILURE;
  }
  printf("The MIPS pseudocode is:\n");

  // Remove new line
  line[strcspn( line, "\n" )] = '\0';

  // Do magic
  unsigned long strLen = strlen(line);
  // The last one should be an ENDLINE
  if (line[strLen - 1] != ENDLINE) {
    fprintf( stderr, "Parsing error...\n" );
    return EXIT_FAILURE;
  }

  // Example: f = g + h - 42;
  // Init variables
  int s_register = 0;
  int t_register = 0;
  struct Line mips[100];
  int mips_line = 0;
  // Current Stage
  int stage = 0; // 0 none, 1: var1 should be assigned, 2: op should be assigned, 3: var2 should be assigned, 4: wrap things up!
  struct Record current_var1 = {.type= UNREG, .data = -1};
  char current_op = '?';
  struct Record current_var2 = {.type= UNREG, .data = -1};
  // Init var relationship: the number EQ s0 - s9
  int alphabet[26];
  for (int i = 0; i < 26; i++) {
    alphabet[i] = UNREG;
  }

  // Ignore semicolon
  for (unsigned long i = 0; i < strLen; i++) {
    char c = line[i];
    if (c == ENDLINE) {
      stage = 4;
    } else if(isspace(c)) {
      continue;
    } else if (islower(c)) {
      if (stage == 0 || stage == 2) {
        register_var(alphabet, c, &s_register);
        stageSMapper(stage, &current_var1, &current_var2, alphabet[alphabet2num(c)]);
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

    // Begin building Line
    if (stage == 3) {
      // We need to check if the t register has used up
      if (t_register > MAX_T_REG) {
        t_register = 0;
      }

      // It is time to build a new line!
      // Special condition: if the first part is immediate var while the second one is a s register
      // We need to build two line at once!
      if (current_var1.type == REG_NUM) {
        mips[mips_line].operation = OP_PLUS;
        mips[mips_line].second.type = REG_ZERO;
        mips[mips_line].second.data = REG_ZERO;
        mips[mips_line].third = current_var1;
        mips[mips_line].first.type = REG_T;
        mips[mips_line].first.data = t_register;
        current_var1.type = REG_T;
        current_var1.data = t_register;
        t_register++;
        mips_line++;
      }

      mips[mips_line].operation = current_op;
      mips[mips_line].second = current_var1;
      mips[mips_line].third = current_var2;
      mips[mips_line].first.type = REG_T;
      mips[mips_line].first.data = t_register;
      // Since we have assigned T, we will always switch var 1 to our t register
      current_var1.type = REG_T;
      current_var1.data = t_register;
      t_register++;
      mips_line++;
      stage = 1;
    } else if (stage == 4) {
      // Nice! Here should be the end of expression, and let's wrap things up!
      // Assign var to s0, change the last line of the Line Seq
      mips[mips_line - 1].first.type = REG_S;
      mips[mips_line - 1].first.data = 0;
    }
  }

  // Print Lines
  for (int i = 0; i < mips_line; i++) {
    printLine(mips[i]);
  }

  return EXIT_SUCCESS;
}
