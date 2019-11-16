#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINES 5
#define MAX_CYCLE 8
#define MAX_LINE_LENGTH 128
#define MAX_INST_LENGTH 5
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
// Registers
#define REG_NUM 0
#define REG_T 1
#define REG_S 2
#define REG_A 3
#define REG_ZERO 3
// Syntax
#define SYB_SPACE ' '
#define SYB_DOLLAR '$'
#define SYB_COMMA ','
#define SYB_LEFT_OPEN '('
#define SYB_RIGHT_OPEN ')'
// Init value
#define UNREG -1
// Boundaries
#define MAX_T_REG 9

// A record is a number - immediate value(int), a $s register(for vars), or a $t register(for temp).
struct Record {
    int type;
    int data;
};

struct Instruction {
    struct Record dependency1;
    struct Record dependency2;
    struct Record output;
    int started_at;
    int nops_to;
    int nops_num;
    int stage;
    int out;
    int dep;
} Instruction_default = {
        .dependency1={.type=UNREG, .data=UNREG},
        .dependency2={.type=UNREG, .data=UNREG},
        .output={.type=UNREG, .data=UNREG},
        .nops_to=UNREG,
        .nops_num= UNREG,
        .out = UNREG,
        .dep = UNREG,
        .stage = 0,
        .started_at = UNREG, // How many dot before exec
};

char stages[5][4] = {"IF", "ID", "EX", "MEM", "WB"};
char nop_1[5][4] = {"IF", "ID", "*", "*", "*"};
char nop_2[6][4] = {"IF", "ID", "ID", "*", "*", "*"};
int num_of_stages = 5;
// Global Variables
// Just dont want to pass them around
char mips[MAX_LINES][MAX_LINE_LENGTH];
int mips_line = 0;
struct Instruction instructions[MAX_LINES]; // Including possible NOPs
int cycle = 0;


// Print Helper

// Print Same Given Stage
void print_stages(int num, char *stage) {
  for (int i = 0; i < num; i++) {
    printf("%s\t", stage);
  }
}

void print_star(int num) {
  for (int i = 0; i < num; i++) {
    printf("*\t");
  }
}

void print_newline() {
  printf("\n");
}

void print_dot(int num) {
  for (int i = 0; i < num; i++) {
    printf(".\t");
  }
}

// Print stage by given order
void print_stage(int from, int to) {
  for (int i = from; i <= to; i++) {
    printf("%s\t", stages[i]);
  }
}

void print_nop() {
  printf("nop\t");
}

void print_nop1_stage(int started_at) {
  print_nop();
  int resp = cycle - started_at;
  if (resp > 4) {
    for (int i = 0; i <= 4; i++) {
      printf("%s\t", nop_2[i]);
    }
    print_dot(resp - 4);
  } else {
    for (int i = 0; i <= resp; i++) {
      printf("%s\t", nop_2[i]);
    }
  }
}

void print_nop2_stage(int started_at) {
  print_nop();
  int resp = cycle - started_at;
  if (resp > 5) {
    for (int i = 0; i <= 4; i++) {
      printf("%s\t", nop_2[i]);
    }
    print_dot(resp - 5);
  } else {
    for (int i = 0; i <= resp; i++) {
      printf("%s\t", nop_2[i]);
    }
  }
}

void print_rest() {
  // Where we have not yet reached
  print_dot(MAX_CYCLE - cycle);
  print_newline();
}

void print_normal_instruction(struct Instruction *i) {
  // How many cycle we need to cover?
  int resp = cycle - i->started_at;
  // What stage we have
  // Normal(0-1) - (if)Dup - Normal(2-4) - Rest
  if (resp > 2) {
    print_stage(0, 1);
    resp -= 2;
    int n = i->nops_num;
    // Dup
    if (n > 0) {
      if (resp > 2) {
        print_stages(resp - 2, "ID");
        resp -= 2;
      } else {
        print_stages(resp, "ID");
        resp = 0;
      }
    }

    // Normal
    if (resp > 3) {
      print_stage(2, 4);
      print_dot(resp - 3);
    } else {
      print_stage(2, resp);
    }
  } else {
    print_stage(0, resp - 1);
  }
}

void increase_stage(struct Instruction *i) {
  if (i->stage < num_of_stages - 1) {
    i->stage++; // No more than 5
  }
}

void init_array() {
  for (int i = 0; i < MAX_LINES; i++) {
    instructions[i] = Instruction_default;
  }
}

bool same_reg(struct Record *a, struct Record *b) {
  if (a->data == b->data &&
      a->type == b->type)
    return true;
  else
    return false;
}

bool compare_instruction_record(struct Instruction *i, struct Record *d1, struct Record *d2) {
  return same_reg(&(i->output), d1) || same_reg(&(i->output), d2);
}

int string_to_type(char s) {
  if (s == 's') {
    return REG_S;
  } else if (s == 't') {
    return REG_T;
  } else if (s == 'a') {
    return REG_A;
  }
  {
    fprintf(stderr, "Unknown String to Type: %c\n", s);
    return UNREG;
  }
}

int string_to_int(const char *s) {
  return atoi(s);
}

bool print_one_stimulation() {
  printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
  // The first line will always the same
  printf("%s\t", mips[0]);
  print_stage(0, instructions[0].stage);
  // Where we have not yet reached
  print_dot(MAX_CYCLE - cycle);
  print_newline();

  // Rest
  for (int i = 1; i < mips_line; i++) {
    printf("%s\t", mips[i]);
    // For Each Cycles, calculate:
    int start = instructions[i].started_at;
    // We need to start running
    if (cycle >= start) {
      int st = instructions[i].stage;
      if (st == 1) {
        // We need to detect if the nop
        if (instructions[i].nops_num != UNREG) {
          // First Nop
          // Delays
          print_dot(start);
          print_nop1_stage(start);
          print_rest();
          if (instructions[i].nops_num == 2) {
            // Delays
            print_nop();
            print_nop2_stage(start);
            print_rest();
          } // EOF NOPS
        }
      } // END of NOPS identify

      // delay
      print_dot(start);
      int real_int = print_normal_instruction(&instructions[i]); // Let the function handle
      print_newline();

      // Increase stage
      if (instructions[i - 1].stage - instructions[i].stage > 1) {
        increase_stage(&instructions[i]);
      }
    }
  }

  increase_stage(&instructions[0]);
  cycle++;

  return instructions[mips_line - 1].stage == 4;
}

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

void read_parts(const char *line, unsigned long *i, struct Record *reg, unsigned long strLen) {
  // or $s0,$s0,$t3
  // lw $a0,12($sp)
  unsigned long j;
  for (j = *i; j < strLen; j++) {
    if (line[j] == SYB_DOLLAR) {
      // Here we have a Register
      // Read next two
      reg->type = string_to_type(line[j + 1]);
      reg->data = string_to_int(&line[j + 2]);
      j += 3;
      break;
    }
  }

  *i = j; // Assign Offset
}


void read_mips(FILE *file) {
  while (fgets(mips[mips_line], MAX_LINE_LENGTH, file)) {
    struct Record dependency_1 = {.type = UNREG, .data = UNREG};
    struct Record dependency_2 = {.type = UNREG, .data = UNREG};
    struct Record output = {.type = UNREG, .data = UNREG};
    // Remove the last newline
    mips[mips_line][strcspn(mips[mips_line], "\n")] = '\0';
    unsigned long strLen = strlen(mips[mips_line]);

    // Parsing line
    unsigned long i = 0;
    int dep = UNREG;
    int out = UNREG;
    // Read Instruction type
    char inst[MAX_INST_LENGTH];
    read_instruction(mips[mips_line], &i, inst, strLen);

    // Identify type
    if (strcmp(inst, IN_ADD) == 0 ||
        strcmp(inst, IN_AND) == 0 ||
        strcmp(inst, IN_OR) == 0 ||
        strcmp(inst, IN_SUB) == 0) {
      read_parts(mips[mips_line], &i, &output, strLen);
      read_parts(mips[mips_line], &i, &dependency_1, strLen);
      read_parts(mips[mips_line], &i, &dependency_2, strLen);
      dep = 2;
      out = 1;
    } else if (strcmp(inst, IN_ADDI) == 0 ||
               strcmp(inst, IN_LW) == 0 ||
               strcmp(inst, IN_ANDI) == 0 ||
               strcmp(inst, IN_ORI) == 0) {
      read_parts(mips[mips_line], &i, &output, strLen);
      read_parts(mips[mips_line], &i, &dependency_1, strLen);
      dep = 1;
      out = 1;
    } else if (strcmp(inst, IN_SW) == 0) {
      read_parts(mips[mips_line], &i, &dependency_1, strLen);
      read_parts(mips[mips_line], &i, &dependency_2, strLen);
      out = 0;
      dep = 2;
    } else {
      fprintf(stderr, "Unknown Reg type: '%s'\n", inst);
    } // End of Var detect

    // Start of dependency detect
    // add $t0,$s2,$s3
    // addi $t1,$t3,73
    // or $s0,$s0,$t3
    // lw $a0,12($sp)
    // sw $t6,32($a1)
    int nops = UNREG;
    int nops_to = UNREG;
    for (int j = mips_line - 1; j >= 0; j--) {
      if (instructions[j].out > 0) {
        // Find if there is a dependency
        if (compare_instruction_record(&instructions[j], &dependency_1, &dependency_2)) {
          // Compute distance
          int dist = mips_line - instructions[j].started_at;
          nops = dist - 1;
          nops_to = mips_line;
          break;
        }
      }
    }
    // Assign current line
    instructions[mips_line].started_at = mips_line;
    instructions[mips_line].dependency1 = dependency_1;
    instructions[mips_line].dependency2 = dependency_2;
    instructions[mips_line].output = output;
    instructions[mips_line].nops_to = nops_to;
    instructions[mips_line].nops_num = nops;
    instructions[mips_line].out = out;
    instructions[mips_line].dep = dep;
    mips_line++;
  } // Finish Parsing
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
  init_array();

  // Processing & Dependency Check
  read_mips(file);

  printf("START OF SIMULATION\n");
  printf("\n");
  for (int i = 0; i < MAX_CYCLE; i++) {
    if (print_one_stimulation()) {
      break;
    }
  }
  printf("\n");
  printf("END OF SIMULATION");


  fclose(file);

  return EXIT_SUCCESS;
}