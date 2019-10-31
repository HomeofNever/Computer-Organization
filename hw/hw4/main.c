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
#define SYB_MFHI 'h'
#define SYB_DIV 'd'
#define SYB_MULT 'm'
#define SYB_MOVE 'v'
#define SYB_SLL 's'
#define SYB_LI 'i'
#define SYB_SRL 'r'
#define SYB_BLTZ 't'
#define SYB_J 'j'
#define SYB_BLOCK 'b'
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
int label = 0;

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
  // Always positive
  if (num < 0) {
    num = 0 - num;
  }
  while (num != 1) {
    num = num / 2;
    count++;
  }
  return count;
}

int findPowers(int x, int *powers) {
  int size = 0;
  // Always positive
  if (x < 0) {
    x = 0 - x;
  }
  while (x > 0) {
    powers[size] = x % 2;
    x = x / 2;
    size++;
  }

  return size;
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
    case SYB_BLOCK:
      printf("L%d", r.data);
      break;
    default:
      fprintf(stderr, "Unrecognized Record type: %d does something goes wrong?\n", r.type);
  }
}

void printLine(struct Line l) {
  // Operation
  switch(l.operation) {
    case OP_PLUS:
      if (l.third.type == REG_NUM) {
        printf("addi ");
      } else {
        printf("add ");
      }
      break;
    case OP_MINUS:
      if (l.third.type == REG_NUM) {
        printf("addi ");
      } else {
        printf("sub ");
      }
      break;
    case SYB_MULT:
      printf("mult ");
      break;
    case SYB_DIV:
      printf("div ");
      break;
    case SYB_MOVE:
      printf("move ");
      break;
    case SYB_MFLO:
      printf("mflo ");
      break;
    case SYB_SLL:
      printf("sll ");
      break;
    case SYB_LI:
      printf("li ");
      break;
    case SYB_J:
      printf("j ");
      break;
    case SYB_SRL:
      printf("srl ");
      break;
    case SYB_BLTZ:
      printf("bltz ");
      break;
    case SYB_MFHI:
      printf("mfhi ");
    case SYB_BLOCK:
      // Do Nothing
      break;
    default:
      fprintf(stderr, "Unrecognized Operation type: does something goes wrong?\n");
  }

  // Vars
  printRecord(l.first);
  switch (l.operation) {
    case OP_PLUS:
    case OP_MINUS:
    case SYB_SLL:
    case SYB_SRL:
      printf(",");
      printRecord(l.second);
      printf(",");
      if (l.third.type == REG_NUM && l.operation == OP_MINUS) {
        // Add minus when minus operation and it is an immediate value
        printf("-");
      }
      printRecord(l.third);
      break;
    case SYB_MULT:
    case SYB_DIV:
    case SYB_LI:
    case SYB_MOVE:
    case SYB_BLTZ:
      printf(",");
      printRecord(l.second);
      break;
    case SYB_BLOCK:
      printf(":");
      break;
    case SYB_MFLO:
    case SYB_MFHI:
    case SYB_J:
      // Do Nothing
      break;
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
void register_var(const char current) {
  int num = alphabet2num(current);
  if (alphabet[num] == UNREG) {
    alphabet[num] = s_register;
    s_register++;
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

int get_t_register() {
  int current = t_register;
  t_register++;
  return current % 10;
}

struct Record build_t_register() {
  struct Record r = {.data=get_t_register(), .type=REG_T};
  return r;
}

void decrease_t_register() {
  t_register--;
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
    struct Record r = build_t_register();
    mips[mips_line].first = r;
    current_var1->type = REG_T;
    current_var1->data = r.data;
    mips_line++;
  }

  mips[mips_line].operation = OP_PLUS;
  mips[mips_line].second = *current_var1;
  mips[mips_line].third = *current_var2;
  struct Record r = build_t_register();
  mips[mips_line].first = r;
  mips_line++;
  // Since we have assigned T, we will always switch var 1 to our t register
  return r;
}

struct Record minus(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  // Special condition: if the first part is immediate var while the second one is a s register
  // We need to build two line at once!
  if (current_var1->type == REG_NUM) {
    mips[mips_line].operation = OP_PLUS;
    mips[mips_line].second.type = REG_ZERO;
    mips[mips_line].second.data = REG_ZERO;
    mips[mips_line].third = *current_var1;
    struct Record r = build_t_register();
    mips[mips_line].first = r;
    current_var1->type = REG_T;
    current_var1->data = r.data;
    mips_line++;
  }

  mips[mips_line].operation = OP_MINUS;
  mips[mips_line].second = *current_var1;
  mips[mips_line].third = *current_var2;
  struct Record r = build_t_register();
  mips[mips_line].first = r;
  mips_line++;
  // Since we have assigned T, we will always switch var 1 to our t register
  return r;
}

struct Record multiple(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  // There are two types of condition: (s * s) or (s * num)
  if (current_var2->type != REG_NUM) {
    // mult $s0,$s0
    // mflo $t0
    mips[mips_line].operation = SYB_MULT;
    mips[mips_line].first = *current_var1;
    mips[mips_line].second = *current_var2;
    mips_line++;
    mips[mips_line].operation = SYB_MFLO;
    struct Record temp =build_t_register();
    mips[mips_line].first = temp;
    mips_line++;
    return temp;
  } else {
    int num = current_var2->data;
    // Special Cases
    // If 0
    if (num == 0) {
      // Allocate a temp
      struct Record result = build_t_register();
      load_val(current_var2, &result, mips);
      return result;
    }

    // If 1
    if (num == 1) {
      // Allocate a temp
      struct Record result = build_t_register();
      mips[mips_line].operation = SYB_MOVE;
      mips[mips_line].first = result;
      mips[mips_line].second = *current_var1;
      mips_line++;
      // Allocate another temp
      struct Record result1 = build_t_register();
      mips[mips_line].operation = SYB_MOVE;
      mips[mips_line].first = result1;
      mips[mips_line].second = result;
      mips_line++;
      return result1;
    }

    // If -1
    if (num == -1) {
      // Allocate a temp
      struct Record result = build_t_register();
      mips[mips_line].operation = SYB_MOVE;
      mips[mips_line].first = result;
      mips[mips_line].second = *current_var1;
      mips_line++;
      // Allocate another temp
      struct Record result1 = build_t_register();
      mips[mips_line].operation = OP_MINUS;
      mips[mips_line].first = result1;
      mips[mips_line].second.data = -1;
      mips[mips_line].second.type = REG_ZERO;
      mips[mips_line].third = result;
      mips_line++;
      return result1;
    }

    // Allocate memory
    int power[100];
    int length = findPowers(num, power);
    // Init
    int flag = 0;
    struct Record current_reg_rec = build_t_register();
    struct Record current_reg_next_rec = build_t_register();

    for (; length > 0; length--) {
      if (power[length] == 1) {
        // SLL
        mips[mips_line].operation = SYB_SLL;
        mips[mips_line].first = current_reg_rec;
        mips[mips_line].second = *current_var1;
        mips[mips_line].third.data = length;
        mips[mips_line].third.type = REG_NUM;
        mips_line++;
        if (flag == 0) {
          // This is the first args
          // MOVE
          mips[mips_line].operation = SYB_MOVE;
          mips[mips_line].first = current_reg_next_rec;
          mips[mips_line].second = current_reg_rec;
          mips_line++;
          // Set flag
          flag = 1;
        } else {
          // Add
          mips[mips_line].operation = OP_PLUS;
          mips[mips_line].first = current_reg_next_rec;
          mips[mips_line].second = current_reg_next_rec;
          mips[mips_line].third = current_reg_rec;
          mips_line++;
        }
      }
    } // End of Loop

    // Here we have multiple of 1 (if exist)
    // add $t1,$t1,$s0
    if (power[0] == 1) {
      mips[mips_line].operation = OP_PLUS;
      mips[mips_line].first = current_reg_next_rec;
      mips[mips_line].second = current_reg_next_rec;
      mips[mips_line].third = *current_var1;
      mips_line++;
    }

    // Dealing with Result
    struct Record result = build_t_register();
    if (current_var2->data < 0) {
      // Negative value
      // Replace with sub
      // sub $s1,$zero,$t1
      mips[mips_line].operation = OP_MINUS;
      mips[mips_line].first = result;
      mips[mips_line].second.data = -1;
      mips[mips_line].second.type = REG_ZERO;
      mips[mips_line].third = current_reg_next_rec;
      mips_line++;
    } else {
      // Move Result
      // move $s1,$t1
      mips[mips_line].operation = SYB_MOVE;
      mips[mips_line].first = result;
      mips[mips_line].second = current_reg_next_rec;
      mips_line++;
    }
    return result;
  }
}

struct Record divided(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  if (current_var2->type != REG_NUM) {
    // div $t0,$s1
    // mflo $s2
    mips[mips_line].operation = SYB_DIV;
    mips[mips_line].first = *current_var1;
    mips[mips_line].second = *current_var2;
    mips_line++;
    mips[mips_line].operation = SYB_MFLO;
    struct Record temp = build_t_register();
    mips[mips_line].first = temp;
    mips_line++;
    return temp;
  } else {
    int num = current_var2->data;

    if (num == 1) {
      // move $s1,$s0
      // Allocate a temp
      struct Record result = build_t_register();
      mips[mips_line].operation = SYB_MOVE;
      mips[mips_line].first = result;
      mips[mips_line].second = *current_var1;
      mips_line++;
      return result;
    }

    if (num == -1) {
      // sub $s1,$zero,$s0
      struct Record result = build_t_register();
      mips[mips_line].operation = OP_MINUS;
      mips[mips_line].first = result;
      mips[mips_line].second.data = -1;
      mips[mips_line].second.type = REG_ZERO;
      mips[mips_line].third = *current_var1;
      mips_line++;
      return result;
    }

    int times = isPowerOf2(num);

    if (times == -1) {
      // No the power of 2
      // li $t0,-31
      struct Record tmp = build_t_register();
      load_val(current_var2, &tmp, mips);
      // div $s0,$t0
      mips[mips_line].operation = SYB_DIV;
      mips[mips_line].first = *current_var1;
      mips[mips_line].second = tmp;
      mips_line++;
      // mflo $s1
      mips[mips_line].operation = SYB_MFLO;
      struct Record tmp1 = build_t_register();
      mips[mips_line].first = tmp1;
      mips_line++;
      return tmp1;
    } else {
      // Special case: use srl
      // Init
      struct Record tmp2 = build_t_register();
      struct Record tmp = build_t_register();
      int l0 = label;
      label++;
      int l1 = label;
      label++;
      // bltz $s0,L0
      mips[mips_line].operation = SYB_BLTZ;
      mips[mips_line].first = *current_var1;
      mips[mips_line].second.data = l0;
      mips[mips_line].second.type = SYB_BLOCK;
      mips_line++;
      // srl $s1,$s0,5
      mips[mips_line].operation = SYB_SRL;
      mips[mips_line].first = tmp2;
      mips[mips_line].second = *current_var1;
      mips[mips_line].third.data = times;
      mips[mips_line].third.type = REG_NUM;
      mips_line++;
      // Negative: sub by zero
      if (num < 0) {
        mips[mips_line].operation = OP_MINUS;
        mips[mips_line].first = tmp;
        mips[mips_line].second.data = -1;
        mips[mips_line].second.type = REG_ZERO;
        mips[mips_line].third = tmp2;
        mips_line++;
      }
      // j L1
      mips[mips_line].operation = SYB_J;
      mips[mips_line].first.type = SYB_BLOCK;
      mips[mips_line].first.data = l1;
      mips_line++;
      // L0:
      mips[mips_line].operation = SYB_BLOCK;
      mips[mips_line].first.type = SYB_BLOCK;
      mips[mips_line].first.data = l0;
      mips_line++;
      // li $t0,32
      load_val(current_var2, &tmp2, mips);
      // div $s0,$t0
      mips[mips_line].operation = SYB_DIV;
      mips[mips_line].first = *current_var1;
      mips[mips_line].second = tmp2; // We use same t register here
      // Since there we have a branch
      // we can reuse tmp2;
      mips_line++;
      // mflo $s1
      mips[mips_line].operation = SYB_MFLO;
      mips[mips_line].first = tmp;
      mips_line++;
      // L1:
      mips[mips_line].operation = SYB_BLOCK;
      mips[mips_line].first.type = SYB_BLOCK;
      mips[mips_line].first.data = l1;
      mips_line++;

      return tmp;
    }
  }
}

struct Record mod(struct Record * current_var1, struct Record * current_var2, struct Line *mips) {
  // Mod will always between two register
  // div $t0,$s1
  // mflo $s2
  mips[mips_line].operation = SYB_DIV;
  mips[mips_line].first = *current_var1;
  mips[mips_line].second = *current_var2;
  mips_line++;
  mips[mips_line].operation = SYB_MFHI;
  struct Record temp = build_t_register();
  mips[mips_line].first = temp;
  mips_line++;
  return temp;
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
      fprintf(stderr, "Parsing error: expecting endline but find '%c'", line[strLen - 1]);
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
      } else if (c == OP_MINUS) {
        // Special: negative number
        if (isdigit(line[i + 1])) {
          if (stage == 0 || stage == 2) {
            i++; // Increase one
            unsigned long num = read_all_digits(line, &i, strLen);
            stageNMapper(stage, &current_var1, &current_var2, 0 - num);
            stage++;
          } else {
            fprintf(stderr, "Parsing error...1\n");
            return EXIT_FAILURE;
          }
        } else {
          // Normal minus symbol
          if (stage == 1) {
            current_op = c;
            stage++;
          } else {
            fprintf(stderr, "Parsing error...?\n");
            return EXIT_FAILURE;
          }
        }
      } else if (islower(c)) {
        if (stage == 0 || stage == 2) {
          register_var(c);
          stageSMapper(stage, &current_var1, &current_var2, alphabet[alphabet2num(c)]);
          stage++;
        } else {
          fprintf(stderr, "Parsing error...2\n");
          return EXIT_FAILURE;
        }
      } else if (isdigit(c)) {
        if (stage == 0 || stage == 2) {
          unsigned long num = read_all_digits(line, &i, strLen);
          stageNMapper(stage, &current_var1, &current_var2, num);
          stage++;
        } else {
          fprintf(stderr, "Parsing error...3\n");
          return EXIT_FAILURE;
        }
      } else if (c == OP_EQ) {
        if (stage == 1) {
          stage = 0; // Reset stage: the true expression comes!
          assignee = current_var1;
        } else {
          // Wait! LHS should have only one var, right?
          fprintf(stderr, "Parsing error...4\n");
          return EXIT_FAILURE;
        }
      } else if (c == OP_PLUS ||
                 c == OP_TIMES ||
                 c == OP_DIVIDED ||
                 c == OP_MOD ) {
        if (stage == 1) {
          current_op = c;
          stage++;
        } else {
          fprintf(stderr, "Parsing error...5\n");
          return EXIT_FAILURE;
        }
      } else {
        fprintf(stderr, "Parsing error...6\n");
        return EXIT_FAILURE;
      }  // End of registering vars

      // Begin parsing line
      if (stage == 3) {
        if (current_op == OP_PLUS) {
          // Reassign next var
          current_var1 = add(&current_var1, &current_var2, mips);
        } else if (current_op == OP_MINUS) {
          current_var1 = minus(&current_var1, &current_var2, mips);
        } else if (current_op == OP_TIMES) {
          current_var1 = multiple(&current_var1, &current_var2, mips);
        } else if (current_op == OP_DIVIDED) {
          current_var1 = divided(&current_var1, &current_var2, mips);
        } else if (current_op == OP_MOD) {
          current_var1 = mod(&current_var1, &current_var2, mips);
        } else {
          fprintf(stderr, "Unrecognized OP...\n");
        }
        stage = 1;
      } else if (stage == 4) {
        // We need to know if this is an assign?
        if (current_var1.type == REG_NUM) {
          load_val(&current_var1, &assignee, mips);
        } else if (current_var1.type == REG_S) {
          // x = a ?
          mips[mips_line].operation = SYB_MOVE;
          mips[mips_line].first = assignee;
          mips[mips_line].second = current_var1;
          mips_line++;
        } else {
          // Nice! Here should be the end of expression, and let's wrap things up!
          // Assign var to assignee, change the last line of the Line Seq
          // For special case Division, we need to change some places
          if (mips[mips_line - 1].operation == SYB_BLOCK) {
            mips[mips_line - 2].first = assignee; // mflo
            mips[mips_line - 7].first = assignee; // srl/sub
            // Negative: we need to change different line
            if (mips[mips_line - 7].operation == OP_MINUS) {
              mips[mips_line - 7].third = assignee; // sub
              mips[mips_line - 8].first = assignee; // srl
            }
          } else {
            mips[mips_line - 1].first = assignee;
          }
          // We also need to return unused t_register
          decrease_t_register();
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
