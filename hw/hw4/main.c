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
#define OP_MOD '%'
#define OP_EQ '='
// Symbols
#define SYB_MFLO 'l'
#define SYB_DIV 'd'
#define SYB_MULT 'm'
#define SYB_MOVE 'v'
#define SYB_SLL 's'
#define SYB_LI 'i'
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
int mips_line = 0;
int alphabet[26];

// A record is a number - immediate value(int), a $s register(for vars), or a $t register(for temp).
struct Record {
    int type;
    int data;
};

// a line of MIPS code
struct Line {
    char operation;
    struct Record first;
    struct Record second;
    struct Record third;
};

// Since each alphabet will only appear once, we can use an char[26] to track them
int alphabet2num(char a) {
  return a - 'a';
}

int isPowerOf2(int num) {
  if (num % 2 != 0) {
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
      fprintf(stderr, "Unrecognized Record type: %d does something goes wrong?\n", r.type);
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
  } else if (l.operation == SYB_MULT) {
    printf("mult ");
  } else if (l.operation == SYB_DIV) {
    printf("div ");
  } else if (l.operation == SYB_MOVE) {
    printf("move ");
  } else if (l.operation == SYB_MFLO) {
    printf("mflo ");
  } else if (l.operation == SYB_SLL) {
    printf("sll ");
  } else if (l.operation == SYB_LI) {
    printf("li ");
  } else {
      fprintf(stderr, "Unrecognized Operation type: does something goes wrong?\n");
  }

  // Vars
  printRecord(l.first);
  if (l.operation == OP_PLUS ||
      l.operation == OP_MINUS ||
      l.operation == SYB_MOVE ||
      l.operation == SYB_SLL
      ) {
    printf(",");
    printRecord(l.second);
    printf(",");
    if (l.third.type == REG_NUM && l.operation == OP_MINUS) {
      // Add minus when minus operation and it is an immediate value
      printf("-");
    }
    printRecord(l.third);
  } else if (l.operation == SYB_MULT ||
             l.operation == SYB_DIV ||
             l.operation == SYB_LI) {
    printf(",");
    printRecord(l.second);
  } else if (l.operation == SYB_MFLO) {
    // Do nothing
  }
  printf("\n");
}

// Register S var to based on current stage
void stageSMapper(int stage, struct Record *current_var1, struct Record *current_var2, int s_register) {
  switch (stage) {
    case 0:
      current_var1->type = REG_S;
      current_var1->data = s_register;
      break;
    case 2:
      current_var2->type = REG_S;
      current_var2->data = s_register;
      break;
    default:
      fprintf(stderr, "Unknown Stage: does something goes wrong?\n");
      break;
  }
}

// Register immediate var to based on current stage
void stageNMapper(int stage, struct Record *current_var1, struct Record *current_var2, int intdata) {
  switch (stage) {
    case 0:
      current_var1->type = REG_NUM;
      current_var1->data = intdata;
      break;
    case 2:
      current_var2->type = REG_NUM;
      current_var2->data = intdata;
      break;
    default:
      fprintf(stderr, "Unknown stage: does something goes wrong?\n");
      break;
  }
}

// Register var if does not appear in the s list
void register_var(int *alphabet, const char current, int *s_register) {
  int num = alphabet2num(current);
  if (alphabet[num] == UNREG) {
    alphabet[num] = *s_register;
    (*s_register)++;
  }
}

// Collect immediate value
unsigned long read_all_digits(const char *line, unsigned long *i, unsigned long strLen) {
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

void load_val(struct Record * current_var1, struct Record * assignee, struct Line *mips) {
  mips[mips_line].operation = SYB_LI;
  mips[mips_line].first = *assignee;
  mips[mips_line].second = * current_var1;
  mips_line++;
}

struct Record add(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  // Special condition: if the first part is immediate var while the second one is a s register
  // We need to build two line at once!
  if (current_var1->type == REG_NUM) {
    mips[mips_line].operation = OP_PLUS;
    mips[mips_line].second.type = REG_ZERO;
    mips[mips_line].second.data = REG_ZERO;
    mips[mips_line].third = *current_var1;
    mips[mips_line].first.type = REG_T;
    mips[mips_line].first.data = t_register;
    current_var1->type = REG_T;
    current_var1->data = t_register;
    t_register++;
    mips_line++;
  }

  mips[mips_line].operation = '+';
  mips[mips_line].second = *current_var1;
  mips[mips_line].third = *current_var2;
  mips[mips_line].first.type = REG_T;
  mips[mips_line].first.data = t_register;
  // Since we have assigned T, we will always switch var 1 to our t register
  struct Record ct = {.type = REG_T, .data=t_register};
  t_register++;
  mips_line++;
  return ct;
}

struct Record minus(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  // Special condition: if the first part is immediate var while the second one is a s register
  // We need to build two line at once!
  if (current_var1->type == REG_NUM) {
    mips[mips_line].operation = OP_PLUS;
    mips[mips_line].second.type = REG_ZERO;
    mips[mips_line].second.data = REG_ZERO;
    mips[mips_line].third = *current_var1;
    mips[mips_line].first.type = REG_T;
    mips[mips_line].first.data = t_register;
    current_var1->type = REG_T;
    current_var1->data = t_register;
    t_register++;
    mips_line++;
  }

  mips[mips_line].operation = '-';
  mips[mips_line].second = *current_var1;
  mips[mips_line].third = *current_var2;
  mips[mips_line].first.type = REG_T;
  mips[mips_line].first.data = t_register;
  // Since we have assigned T, we will always switch var 1 to our t register
  struct Record ct = {.type = REG_T, .data=t_register};
  t_register++;
  mips_line++;
  return ct;
}

struct Record multiple(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  mips[mips_line].operation = '+';
  mips[mips_line].second = *current_var1;
  mips[mips_line].third = *current_var2;
  mips[mips_line].first.type = REG_T;
  mips[mips_line].first.data = t_register;
  // Since we have assigned T, we will always switch var 1 to our t register
  struct Record ct = {.type = REG_T, .data=t_register};
  t_register++;
  mips_line++;
  return ct;
}

int main(int argc, char *argv[]) {
  char line[256];
  FILE *fp;
  fp = fopen(argv[1], "r");

  // check if the file can be opened
  if (!fp) {
    fprintf(stderr, "Error: Cannot open file!\n");
    return EXIT_FAILURE;
  }

  // Init var relationship: the number EQ s0 - s9
  for (int i = 0; i < 26; i++) {
    alphabet[i] = UNREG;
  }
  // Init registers
  t_register = 0;
  s_register = 0;

  // Begin Line Iteration
  while (fgets(line, 255, (FILE *) fp) != NULL) {
    // Remove new line
    line[strcspn(line, "\n")] = '\0';
    // print the line
    printf("# %s\n", line);

    unsigned long strLen = strlen(line);
    // The last one should be an ENDLINE
    if (line[strLen - 1] != ENDLINE) {
      fprintf(stderr, "Parsing error...\n");
      return EXIT_FAILURE;
    }

    // Example: f = g + h - 42;
    // Init variables
    mips_line = 0;
    struct Line mips[100];
    struct Record assignee = {.type= REG_S, .data = -1};
    // Current Stage
    int stage = 0; // 0 none, 1: var1 should be assigned, 2: op should be assigned, 3: var2 should be assigned, 4: wrap things up!
    struct Record current_var1 = {.type= UNREG, .data = -1};
    char current_op = '?';
    struct Record current_var2 = {.type= UNREG, .data = -1};
    // Line Loop Begin
    // Example: f = g + h - 42;
    for (unsigned long i = 0; i < strLen; i++) {
      char c = line[i];
      if (c == ENDLINE) {
        stage = 4;
      } else if (isspace(c)) {
        continue;
      } else if (islower(c)) {
        if (stage == 0 || stage == 2) {
          register_var(alphabet, c, &s_register);
          stageSMapper(stage, &current_var1, &current_var2, alphabet[alphabet2num(c)]);
          stage++;
        } else {
          fprintf(stderr, "Parsing error...\n");
          return EXIT_FAILURE;
        }
      } else if (isdigit(c)) {
        if (stage == 0 || stage == 2) {
          unsigned long num = read_all_digits(line, &i, strLen);
          stageNMapper(stage, &current_var1, &current_var2, num);
          stage++;
        } else {
          fprintf(stderr, "Parsing error...\n");
          return EXIT_FAILURE;
        }
      } else if (c == OP_EQ) {
        if (stage == 1) {
          stage = 0; // Reset stage: the true expression comes!
          assignee = current_var1;
        } else {
          // Wait! LHS should have only one var, right?
          fprintf(stderr, "Parsing error...\n");
          return EXIT_FAILURE;
        }
      } else if (c == OP_MINUS ||
                 c == OP_PLUS ||
                 c == OP_TIMES ||
                 c == OP_DIVIDED ||
                 c == OP_MOD ) {
        if (stage == 1) {
          current_op = c;
          stage++;
        } else {
          fprintf(stderr, "Parsing error...\n");
          return EXIT_FAILURE;
        }
      } else {
        fprintf(stderr, "Parsing error...\n");
        return EXIT_FAILURE;
      }  // End of registering vars

      // Begin parsing line
      if (stage == 3) {
        // We need to check if the t register has used up
        if (t_register > MAX_T_REG) {
          t_register = 0;
        }
        if (current_op == OP_PLUS) {
          // Reassign next var
          current_var1 = add(&current_var1, &current_var2, mips);
          stage = 1;
        } else if (current_op == OP_MINUS) {
          current_var1 = minus(&current_var1, &current_var2, mips);
          stage = 1;
        } else if (current_op == OP_TIMES) {

        } else if (current_op == OP_DIVIDED) {

        } else if (current_op == OP_MOD) {

        } else {
          fprintf(stderr, "Unrecognized OP...\n");
        }
      } else if (stage == 4) {
        // We need to know if this is an assign?
        if (current_var1.type == REG_NUM) {
          load_val(&current_var1, &assignee, mips);
        } else {
          // Nice! Here should be the end of expression, and let's wrap things up!
          // Assign var to assignee, change the last line of the Line Seq
          mips[mips_line - 1].first = assignee;
        }
      }
    }
    // Print Lines
    for (int i = 0; i < mips_line; i++) {
      printLine(mips[i]);
    }
  }

  fclose(fp);
  return EXIT_SUCCESS;
}
