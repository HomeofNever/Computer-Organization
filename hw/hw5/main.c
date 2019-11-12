#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 5
#define MAX_CYCLE 9
#define MAX_LINE_LENGTH 128
// Types
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
char mips[MAX_LINES][MAX_LINE_LENGTH];
int mips_line = 0;
int is_nop1[MAX_LINES], is_nop2[MAX_LINES], has_nop[MAX_LINES];
int cycle = 1;

void init_array() {
    for (int i = 0; i < MAX_LINES; i++) {
        is_nop1[i] = UNREG;
        is_nop2[i] = UNREG;
        has_nop[i] = UNREG;
    }
}

// A record is a number - immediate value(int), a $s register(for vars), or a $t register(for temp).
struct Record {
    int type;
    int data;
};

void print_one_stimulation() {
    printf("CPU Cycles ===>\t1\t2\t3\t4\t5\t6\t7\t8\t9\n");

}

void print_style() {
    printf("START OF SIMULATION\n");
    printf("\n");

    printf("\n");
    printf("END OF SIMULATION");
}

void read_mips(FILE *file) {
    struct Record prev_reg = {.type=UNREG, .data=UNREG};
    struct Record current_reg1 = {.type=UNREG, .data=UNREG};
    struct Record current_reg2 = {.type=UNREG, .data=UNREG};

    while (fgets(mips[mips_line], MAX_LINE_LENGTH, file)) {
        unsigned long strLen = strlen(mips[mips_line]);
        // The last one should be an ENDLINE
        if (mips[mips_line][strLen - 1] != ENDLINE) {
            fprintf(stderr, "Parsing error: expecting endline but find '%c'", mips[mips_line][strLen - 1]);
            break;
        }

        // Parsing line: Identify dependency
            
    }
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

    // add, addi, sub, and, andi, or, ori, lw,
    // and sw instructions.

    // Print Head

    fclose(file);

    return EXIT_SUCCESS;
}