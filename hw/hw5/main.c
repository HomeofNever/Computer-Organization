#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 5
#define MAX_CYCLE 9
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

// Global Variables
// Just dont want to pass them around
char mips[MAX_LINES][MAX_LINE_LENGTH];
int mips_line = 0;
struct Record dependency[MAX_LINES][2];
struct Record output_reg[MAX_LINES];
int stage[MAX_LINES];
int cycle = 1;

void init_array() {
    for (int i = 0; i < MAX_LINES; i++) {
        dependency[i][0].type = UNREG;
        dependency[i][0].data = UNREG;
        dependency[i][1].type = UNREG;
        dependency[i][1].data = UNREG;
        output_reg[i].data = UNREG;
        output_reg[i].type = UNREG;
        stage[i] = 0;
    }
}

int string_to_type(char s) {
    if (s == 's') {
        return REG_S;
    } else if (s == 't') {
        return REG_T;
    } else {
        fprintf(stderr, "Unknown String to Type");
        return UNREG;
    }
}

int string_to_int(const char *s) {
    return atoi(s);
}

void print_one_stimulation() {
    printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");
}

void print_style() {
    printf("START OF SIMULATION\n");
    printf("\n");

    printf("\n");
    printf("END OF SIMULATION");
}

void read_instruction(const char *line, unsigned long *i, char* inst, unsigned long strLen) {
    unsigned int index = 0;
    for (unsigned long j = *i; j < strLen; j++) {
        if (line[j] != SYB_SPACE) {
            inst[index] = line[j];
            index++;
        } else {
            inst[index] = '\0';
            *i = j - 1;
            break;
        }
    }
}

// add, addi, sub, and, andi, or, ori, lw, sw
// Either one dst,
int instruction_regs(const char *inst) {
    if (inst == IN_ADD ||
        inst == IN_AND ||
        inst == IN_OR ||
        inst == IN_SUB) {
        return 2;
    } else if (inst == IN_ADDI ||
               inst == IN_LW ||
               inst == IN_SW ||
               inst == IN_ANDI ||
               inst == IN_ORI) {
        return 1; // Ignore const
    } else {
        fprintf(stderr, "Unknown Reg type");
        return UNREG;
    }
}

void read_parts (const char *line, unsigned long *i, struct Record * reg, unsigned long strLen) {
    // or $s0,$s0,$t3
    // lw $a0,12($sp)
    for (unsigned long j = *i; j < strLen; j++) {
        if (line[j] == SYB_DOLLAR) {
            // Here we have a Register
            // Read next two
            reg->type = string_to_type(line[j + 1]);
            reg->data = string_to_int(&line[j + 2]);
            j += 3;
            *i = j;
            break;
        }
    }
}


void read_mips(FILE *file) {
    int last_reg = UNREG;

    while (fgets(mips[mips_line], MAX_LINE_LENGTH, file)) {
        unsigned long strLen = strlen(mips[mips_line]);
        // Parsing line
        // add $t0,$s2,$s3
        // addi $t1,$t3,73
        // or $s0,$s0,$t3
        // lw $a0,12($sp)
        // sw $t6,32($a1)
        for (unsigned long i = 0; i < strLen; i++) {
            // Read Instruction type
            char inst[MAX_LINE_LENGTH];
            read_instruction(mips[mips_line], &i, inst, strLen);
            last_reg = instruction_regs(inst);
            read_parts(mips[mips_line], &i, &output_reg[mips_line], strLen);
            read_parts(mips[mips_line], &i, &dependency[mips_line][0], strLen);
            if (last_reg == 2) {
                read_parts(mips[mips_line], &i, &dependency[mips_line][1], strLen);
            } // Finish Line Parse
        }
        mips_line++;
    } // Finish Parsing

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Missing Parameters\n");
        return EXIT_FAILURE;
    }

    // Open input file
    char const* const fileName = argv[1];
    FILE * file;
    file = fopen(fileName, "r");

    // check if the file can be opened
    if (!file) {
        fprintf(stderr, "Error: Cannot open file!\n");
        return EXIT_FAILURE;
    }

    // Init
    init_array();

    // Processing
    read_mips(file);

    // Dependency Check

    //
    // and sw instructions.

    // Print Head

    fclose(file);

    return EXIT_SUCCESS;
}