/* hw2.c */
/* NAME: Xinhao Luo*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OPPLUS '+'
#define OPMINUS '-'
#define REGNUM 0
#define REGT 1
#define REGS 2
#define ENDLINE ';'
#define EQ '='
#define UNREG -1

// A record is a number - 0 or a $s register (for vars) or a $t register (for temp).
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

int alphabet2num(char a) {
  return a - 'a';
}

void printRecord(struct Record r) {
  if (r.type == REGNUM) {
    printf("%d", r.data);
  } else if (r.type == REGS) {
    printf("$%c%d", 's', r.data);
  } else if (r.type == REGT) {
    printf("$%c%d", 't', r.data);
  } else {
    fprintf(stderr, "Unrecognized Record type: does something goes wrong?\n");
  }
}

void printLine(struct Line l) {
  // Operation
  if (l.operation == OPPLUS) {
    printf("add ");
  } else if (l.operation == OPMINUS) {
    printf("sub ");
  } else {
    fprintf(stderr, "Unrecognized Operation type: does something goes wrong?\n");
  }

  // Vars
  printRecord(l.first);
  printf(",");
  printRecord(l.second);
  printf(",");
  printRecord(l.third);
  printf("\n");
}

void register_var(int* alphabet, const char current, int *s_register) {
  int num = alphabet2num(current);
  if (alphabet[num] == UNREG) {
    alphabet[num] = *s_register;
    (*s_register)++;
  }
}

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

void stageSMapper(int stage, struct Record *current_var1, struct Record *current_var2, int s_register) {
  switch (stage){
    case 0:
      current_var1->type = REGS;
      current_var1->data = s_register;
      break;
    case 2:
      current_var2->type = REGS;
      current_var2->data = s_register;
      break;
    default:
      fprintf( stderr, "WTF?\n" );
      break;
  }
}

void stageNMapper(int stage, struct Record *current_var1, struct Record *current_var2, int intdata) {
  switch (stage){
    case 0:
      current_var1->type = REGNUM;
      current_var1->data = intdata;
      break;
    case 2:
      current_var2->type = REGNUM;
      current_var2->data = intdata;
      break;
    default:
      fprintf( stderr, "WTF?\n" );
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
  int stage = 0; // 0 none, 1 var1 should be assigned, 2 op should be assigned, 3 var2 should be assigned, 4 wrap things up!
  struct Record current_var1 = {.type= UNREG, .data = -1};
  char current_op = '?';
  struct Record current_var2 = {.type= UNREG, .data = -1};
  // Register relationship, the number EQ s0 - s9
  int alphabet[26];
  for (int i = 0; i < 26; i++) {
    alphabet[i] = UNREG;
  }

  // Ignore Semicolon
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
        fprintf( stderr, "Parsing error...1\n" );
        return EXIT_FAILURE;
      }
    } else if (isdigit(c)) {
      if (stage == 0 || stage == 2) {
        unsigned long num = read_all_digits(line, &i, strLen);
        stageNMapper(stage, &current_var1, &current_var2, num);
        stage++;
      } else {
        fprintf( stderr, "Parsing error...2\n" );
        return EXIT_FAILURE;
      }
    } else if(c == EQ) {
      if (stage == 1) {
        stage = 0; // Reset progress
      } else {
        fprintf( stderr, "Parsing error...3\n" );
        return EXIT_FAILURE;
      }
    } else if (c == OPMINUS){
      if (stage == 1) {
        current_op = OPMINUS;
        stage++;
      } else {
        fprintf( stderr, "Parsing error...4\n" );
        return EXIT_FAILURE;
      }
    } else if (c == OPPLUS) {
      if (stage == 1) {
        current_op = OPPLUS;
        stage++;
      } else {
        fprintf( stderr, "Parsing error...5\n" );
        return EXIT_FAILURE;
      }
    } else {
        fprintf( stderr, "Parsing error...6\n" );
        return EXIT_FAILURE;
    } // End of registering vars

    // Begin of building Line
    if (stage == 3) {
      // It is time to build a new line!
      mips[mips_line].operation = current_op;
      mips[mips_line].second = current_var1;
      mips[mips_line].third = current_var2;
      mips[mips_line].first.type = REGT;
      mips[mips_line].first.data = t_register;
      // Since we have assigned T, we need to switch the 1 to our vars
      current_var1.type = REGT;
      current_var1.data = t_register;
      t_register++;
      mips_line++;
      stage = 1;
    } else if (stage == 4) {
      // Assign var to s0, change the last line of the Line Seq
      mips[mips_line - 1].first.type = REGS;
      mips[mips_line - 1].first.data = 0;
    }
  }

  for (int i = 0; i < mips_line; i++) {
    printLine(mips[i]);
  }

  return EXIT_SUCCESS;
}
