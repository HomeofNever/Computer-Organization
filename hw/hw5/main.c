#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// MAX
#define MAX_LINES 5
#define MAX_CYCLE 9
#define MAX_LINE_LENGTH 128
#define MAX_INST_LENGTH 5
#define MAX_STAGE 5
#define MAX 128
// Types
#define IN_ADD "add"
#define IN_ADDI "addi"
#define IN_SUB "sub"
#define IN_AND "and"
#define IN_ANDI "andi"
#define IN_OR "or"
#define IN_ORI "ori"
#define IN_LW "lw"
#define IN_SW "sw"
// Syntax
#define ENDL "\0"
#define SYB_SPACE ' '
#define SYB_DOLLAR '$'
#define SYB_COMMA ','
#define SYB_LEFT_OPEN '('
#define SYB_RIGHT_OPEN ')'
// Init value
#define UNREG -1
// Boundaries
#define NOP1 "nop1"
#define NOP2 "nop2"

// Struct
struct Instruction {
    char mips[MAX_LINE_LENGTH];
    char operator[MAX_INST_LENGTH];
    char d1[MAX_INST_LENGTH];
    char d2[MAX_INST_LENGTH];
    char o[MAX_INST_LENGTH];
} default_instruction = {
        .operator = ENDL,
        .mips = ENDL,
        .d1 = ENDL,
        .d2 = ENDL,
        .o = ENDL
};

struct Line {
    struct Instruction instruction;
    char cols[MAX_CYCLE][MAX_INST_LENGTH];
    int stage;
    int started_at;
    int nop_at;
};

// Init Global Vars
struct Line board[MAX];
char stages[5][4] = {"IF", "ID", "EX", "MEM", "WB"};
char nop_1_stage[5][4] = {"IF", "ID", "*", "*", "*"};
char nop_2_stage[6][4] = {"IF", "ID", "ID", "*", "*", "*"};
int line_num = 0;
int real_exec_line = 0;
int current_cycle = 0;

// Helper
bool not_empty(const char *c) {
  return c[0] != '\0';
}

bool is_nop_1(struct Line *l) {
  return strcmp(l->instruction.operator, NOP1) == 0;
}

bool is_nop_2(struct Line *l) {
  return strcmp(l->instruction.operator, NOP2) == 0;
}

bool compare_dep(struct Instruction *i, char *d1, char *d2) {
  return (not_empty(d1) && strcmp(i->o, d1) == 0) ||
         (not_empty(d2) && strcmp(i->o, d2) == 0);
}

bool has_dep(struct Instruction *i, char *d) {
  return not_empty(d) &&
         ((not_empty(i->d1) && strcmp(i->d1, d) == 0) ||
          (not_empty(i->d2) && strcmp(i->d2, d) == 0));
}

// Init
void initialize_line(struct Line *l) {
  l->instruction = default_instruction;
  l->stage = 0;
  l->started_at = 0;
  l->nop_at = UNREG;
  for (int i = 0; i < MAX_CYCLE; ++i) {
    strcpy(l->cols[i], ".");
  }
}

void init() {
  for (int i = 0; i < MAX; i++)
    initialize_line(&board[i]);
}

// Read Helper
void read_instruction(const char *line, unsigned long *i, char *inst, unsigned long strLen) {
  unsigned int index = 0;
  for (unsigned long j = *i; j < strLen; j++) {
    if (line[j] != SYB_SPACE) {
      inst[index] = line[j];
      index++;
    } else {
      inst[index] = '\0';
      *i = j + 1;
      break;
    }
  }
}

void read_parts(const char *line, unsigned long *i, char *reg, unsigned long strLen) {
  unsigned long j;
  for (j = *i; j < strLen; j++) {
    if (line[j] == SYB_DOLLAR) {
      // Here we have a Register
      // Read next two
      reg[0] = line[j + 1];
      reg[1] = line[j + 2];
      reg[2] = '\0';
      j += 3;
      break;
    }
  }

  *i = j; // Assign Offset
}


void read_mips(FILE *file) {
  char current_line[MAX_LINE_LENGTH];
  while (fgets(current_line, MAX_LINE_LENGTH, file)) {
    // Remove the last newline
    current_line[strcspn(current_line, "\n")] = '\0';
    unsigned long strLen = strlen(current_line);

    // Parsing line
    unsigned long i = 0;
    char o[MAX_INST_LENGTH];
    char d1[MAX_INST_LENGTH];
    char d2[MAX_INST_LENGTH];
    // Read Instruction type
    char inst[MAX_INST_LENGTH];
    read_instruction(current_line, &i, inst, strLen);

    // Identify type
    if (strcmp(inst, IN_ADD) == 0 ||
        strcmp(inst, IN_AND) == 0 ||
        strcmp(inst, IN_OR) == 0 ||
        strcmp(inst, IN_SUB) == 0) {
      read_parts(current_line, &i, o, strLen);
      read_parts(current_line, &i, d1, strLen);
      read_parts(current_line, &i, d2, strLen);
    } else if (strcmp(inst, IN_ADDI) == 0 ||
               strcmp(inst, IN_LW) == 0 ||
               strcmp(inst, IN_ANDI) == 0 ||
               strcmp(inst, IN_ORI) == 0) {
      read_parts(current_line, &i, o, strLen);
      read_parts(current_line, &i, d1, strLen);
    } else if (strcmp(inst, IN_SW) == 0) {
      read_parts(current_line, &i, d1, strLen);
      read_parts(current_line, &i, d2, strLen);
    } else {
      fprintf(stderr, "Unknown Reg type: '%s'\n", inst);
    } // End of Var detect

    // Start of dependency detect
    // add $t0,$s2,$s3
    // addi $t1,$t3,73
    // or $s0,$s0,$t3
    // lw $a0,12($sp)
    // sw $t6,32($a1)
    int nops_at = UNREG;
    for (int j = line_num - 1; j >= 0; j--) {
      if (!(is_nop_1(&board[j]) && is_nop_2(&board[j]))) {
        // Find if there is a dependency
        if (compare_dep(&board[j].instruction, d1, d2)) {
          // Compute distance
          int dist = line_num - board[j].started_at;
          if (dist > 0 && dist < 3) {
            // One bubble
            // Wait! Check if it is waited
            // If the dep is not the nearest line, the last line may have waited for the dep for you
            if (j == line_num - 1 ||
                !has_dep(&board[line_num - 1].instruction, board[j].instruction.o)
                    ) {
              nops_at = j;
              strcpy(board[line_num].instruction.operator, "nop1");
              board[line_num].started_at = real_exec_line;
              line_num++;
              if (dist == 1) {
                strcpy(board[line_num].instruction.operator, "nop2");
                board[line_num].started_at = real_exec_line;
                line_num++;
              }
            }
          }
          break; // Dep Found, if it is not qualify, then we don't need to worried about it anymore.
        }
      }
    }
    // Assign current line
    strcpy(board[line_num].instruction.mips, current_line);
    strcpy(board[line_num].instruction.operator, inst);
    strcpy(board[line_num].instruction.d1, d1);
    strcpy(board[line_num].instruction.d2, d2);
    strcpy(board[line_num].instruction.o, o);
    board[line_num].started_at = real_exec_line;
    board[line_num].nop_at = nops_at;
    line_num++;
    real_exec_line++;
  } // Finish Parsing
}

void print_line(struct Line *l) {
  // Current Cycle, Each of them have their own stages
  if (is_nop_1(l)) {
    if (current_cycle > l->started_at + 1) {
      printf("nop\t\t");
      for (int i = 0; i < MAX_CYCLE - 1; i++) {
        printf("%s\t", l->cols[i]);
      }
      printf("%s", l->cols[MAX_CYCLE - 1]);
      printf("\n");
    }
  } else if (is_nop_2(l)) {
    if (current_cycle > l->started_at + 2) {
      printf("nop\t\t");
      for (int i = 0; i < MAX_CYCLE - 1; i++) {
        printf("%s\t", l->cols[i]);
      }
      printf("%s", l->cols[MAX_CYCLE - 1]);
      printf("\n");
    }
  } else {
    printf("%s\t", l->instruction.mips);
    for (int i = 0; i < MAX_CYCLE - 1; i++) {
      printf("%s\t", l->cols[i]);
    }
    printf("%s", l->cols[MAX_CYCLE - 1]);
    printf("\n");
  }
}

void print_all() {
  printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
  for (int i = 0; i < line_num; i++) {
    print_line(&board[i]);
  }
  printf("\n");
}

bool run_one_stimulation() {
  // First line
  // #5 will be the final
  if (board[0].stage < MAX_STAGE) {
    strcpy(board[0].cols[current_cycle], stages[board[0].stage]);
    board[0].stage++;
  }

  // Rest
  for (int i = 1; i < line_num; i++) {
    if (current_cycle >= board[i].started_at) {
      if (is_nop_2(&board[i])) {
        if (board[i].stage < MAX_STAGE + 1) {
          strcpy(board[i].cols[current_cycle], nop_2_stage[board[i].stage]);
          board[i].stage++;
        }
      } else if (is_nop_1(&board[i])) {
        if (board[i].stage < MAX_STAGE) {
          strcpy(board[i].cols[current_cycle], nop_1_stage[board[i].stage]);
          board[i].stage++;
        }
      } else {
        if (board[i].stage < MAX_STAGE) {
          // Check dep
          if (board[i].nop_at != UNREG) {
            if (board[board[i].nop_at].stage != 5 && board[i].stage == 1) {
              strcpy(board[i].cols[current_cycle], stages[board[i].stage]);
            } else {
              strcpy(board[i].cols[current_cycle], stages[board[i].stage]);
              board[i].stage++;
            }
          } else {
            strcpy(board[i].cols[current_cycle], stages[board[i].stage]);
            if (board[i - 1].stage == 5 || board[i - 1].stage - board[i].stage > 1) {
              // If the previous block, wait for it
              board[i].stage++;
            }
          }
        }
      }
    }
  }

  return board[line_num - 1].stage == 5;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Missing Parameters\n");
    return EXIT_FAILURE;
  }

  // Open input file
  char const *const fileName = argv[1];
  FILE *file;
  file = fopen(fileName, "r");

  // check if the file can be opened
  if (!file) {
    fprintf(stderr, "Error: Cannot open file!\n");
    return EXIT_FAILURE;
  }

  // Init
  init();

  // Processing & Dependency Check
  read_mips(file);

  printf("START OF SIMULATION\n");
  printf("\n");
  for (; current_cycle < MAX_CYCLE; current_cycle++) {
    if (run_one_stimulation()) {
      print_all();
      break;
    }
    print_all();
  }
  printf("END OF SIMULATION");

  fclose(file);

  return EXIT_SUCCESS;
}