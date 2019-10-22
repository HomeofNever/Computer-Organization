/* lab08.c */

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

/* define BIT type as a char (i.e., one byte) */
typedef char BIT;

/* function prototypes (basic gates) */
BIT not_gate(BIT);

BIT or_gate(BIT, BIT);

BIT and_gate(BIT, BIT);

BIT xor_gate(BIT, BIT);

/* function prototypes (combinational circuits) */
BIT multiplexer(BIT, BIT, BIT, BIT, BIT, BIT);

void decoder(BIT, BIT, BIT *, BIT *, BIT *, BIT *);

/* function prototypes (ALUs) */
BIT alu_1bit(BIT, BIT, BIT, BIT, BIT, BIT *);

void alu_4bit(BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT, BIT *, BIT *, BIT *, BIT *);

/* function prototype (floating point binary conversion) */
void ieee754encode(float value, char *encoded);

/* main() provides some unit testing */
int main() {
    BIT A, B;
    BIT I0, I1, I2, I3;
    BIT O0, O1, O2, O3;
    BIT S0, S1;
    BIT OP0, OP1, CIN, COUT;
    BIT A0, A1, A2, A3, B0, B1, B2, B3, C0, C1, C2, C3, E0, E1, E2, E3;

    /* Unit test for NOT gate:
     *      _
     *  A | A
     * ---|---
     *  0 | 1
     *  1 | 0
     */
    printf("===== Unit test for not_gate =====\n");
    printf("not_gate( A ) | RESULT\n");
    printf("--------------|--------\n");
    printf("not_gate( 0 ) | %d\n", not_gate(0));
    printf("not_gate( 1 ) | %d\n", not_gate(1));


    /* Unit test for OR gate:
     *
     *  A  B | A OR B
     * ------|--------
     *  0  0 |   0
     *  0  1 |   1
     *  1  0 |   1
     *  1  1 |   1
     */
    printf("\n===== Unit test for or_gate =====\n");
    printf("or_gate( A, B ) | RESULT\n");
    printf("----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("or_gate( %d, %d ) | %d\n", A, B, or_gate(A, B));


    /* Unit test for AND gate:
     *
     *  A  B | A AND B
     * ------|--------
     *  0  0 |    0
     *  0  1 |    0
     *  1  0 |    0
     *  1  1 |    1
     */
    printf("\n===== Unit test for and_gate =====\n");
    printf("and_gate( A, B ) | RESULT\n");
    printf("-----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("and_gate( %d, %d ) | %d\n", A, B, and_gate(A, B));


    /* Unit test for XOR gate:
     *
     *  A  B | A XOR B
     * ------|--------
     *  0  0 |    0
     *  0  1 |    1
     *  1  0 |    1
     *  1  1 |    0
     */
    printf("\n===== Unit test for xor_gate =====\n");
    printf("xor_gate( A, B ) | RESULT\n");
    printf("-----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("xor_gate( %d, %d ) | %d\n", A, B, xor_gate(A, B));


    /* Unit test for 4-input multiplexer */
    printf("\n===== Unit test for 4-input multiplexer =====\n");
    printf("multiplexer( S0, S1 ) | ( I0, I1, I2, I3 ) | RESULT ( I0 | I1 | I2 | I3 )\n");
    printf("----------------------|--------------------|------------------------------\n");
    for (S0 = 0; S0 < 2; S0++)
        for (S1 = 0; S1 < 2; S1++)
            for (I0 = 0; I0 < 2; I0++)
                for (I1 = 0; I1 < 2; I1++)
                    for (I2 = 0; I2 < 2; I2++)
                        for (I3 = 0; I3 < 2; I3++)
                            printf("multiplexer( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) | %2d\n",
                                   S0, S1, I0, I1, I2, I3, multiplexer(I0, I1, I2, I3, S0, S1));


    /* Unit test for 2-input decoder:
     *
     *  I0 I1 | O0 O1 O2 O3
     * -------|-------------
     *   0  0 |  1  0  0  0
     *   0  1 |  0  1  0  0
     *   1  0 |  0  0  1  0
     *   1  1 |  0  0  0  1
     */
    printf("\n===== Unit test for 2-input decoder =====\n");
    printf("decoder( I0, I1 ) | ( O0, O1, O2, O3 )\n");
    printf("------------------|--------------------\n");
    /*
     * TO DO: Implement test cases
     */
    for (I0 = 0; I0 < 2; I0++) {
        for (I1 = 0; I1 < 2; I1++) {
            decoder(I0, I1, &O0, &O1, &O2, &O3);
            printf("decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) \n",
                   I0, I1, O0, O1, O2, O3);
        }
    }


    /* Unit test for 1-bit ALU:
     *  OP0 OP1 |  Operation
     * ---------|--------------
     *    0   0 |   A * B (AND)
     *    0   1 |   A + B (OR)
     *    1   0 |   A + B (ADD)
     *    1   1 |   A - B (SUB)
     *
     *  OP0 OP1 CIN  A  B | RESULT COUT
     * -------------------|-------------
     *    0   0   0  0  0 |      0    0
     *    0   0   0  0  1 |      0    0
     *    0   0   0  1  0 |      0    0
     *    0   0   0  1  1 |      1    1
     *    0   0   1  0  0 |      0    0
     *    0   0   1  0  1 |      0    1
     *    0   0   1  1  0 |      0    1
     *    0   0   1  1  1 |      1    1
     *    1   0   0  0  0 |      0    0
     *    1   0   0  0  1 |      1    0
     *    1   0   0  1  0 |      1    0
     *    1   0   0  1  1 |      1    1
     *    1   0   1  0  0 |      0    0
     *    1   0   1  0  1 |      1    1
     *    1   0   1  1  0 |      1    1
     *    1   0   1  1  1 |      1    1
     *    0   1   0  0  0 |      0    0
     *    0   1   0  0  1 |      1    0
     *    0   1   0  1  0 |      1    0
     *    0   1   0  1  1 |      0    1
     *    0   1   1  0  0 |      1    0
     *    0   1   1  0  1 |      0    1
     *    0   1   1  1  0 |      0    1
     *    0   1   1  1  1 |      1    1
     *    1   1   0  0  0 |      1    0
     *    1   1   0  0  1 |      0    0
     *    1   1   0  1  0 |      0    1
     *    1   1   0  1  1 |      1    0
     *    1   1   1  0  0 |      0    1
     *    1   1   1  0  1 |      1    0
     *    1   1   1  1  0 |      1    1
     *    1   1   1  1  1 |      0    1
     */
    printf("\n===== Unit test for 1-bit alu =====\n");
    printf("alu_1bit( OP0, OP1, CIN, A, B ) | ( RESULT, COUT )\n");
    printf("--------------------------------|------------------\n");
    /*
     * TO DO: Implement test cases
     */
    for (OP0 = 0; OP0 < 2; OP0++) {
        for (OP1 = 0; OP1 < 2; OP1++) {
            for (CIN = 0; CIN < 2; CIN++) {
                for (A = 0; A < 2; A++) {
                    for (B = 0; B < 2; B++) {
                        BIT result = alu_1bit(OP0, OP1, CIN, A, B, &COUT);
                        printf("alu_1bit( %2d, %2d, %2d, %2d, %2d ) | ( %2d, %2d ) \n",
                               OP0, OP1, CIN, A, B, result, COUT);
                    }
                }
            }
        }
    }


    int failed = 0, total = 0, A4bit, B4bit, C4bit, expected = 0;
    printf("\n===== Unit test for 4-bit alu =====\n");
    printf(" alu_4bit( OP0, OP1, A0, A1, A2, A3, B0, B1, B2, B3 ) | ( C0, C1, C2, C3 )\n");
    printf("------------------------------------------------------|--------------------\n");
    /*
     * TO DO: Implement test cases. Verify by comparing with the "expected"
     * values and only print a message if there is a mismatch.
     * Make sure you test all ALU operations on all possible combinations of input values.
     * Print statistics of failed and total test cases in the end.
     */
    for (OP0 = 0; OP0 < 2; OP0++) {
        for (OP1 = 0; OP1 < 2; OP1++) {
            for (A0 = 0; A0 < 2; A0++) {
                for (A1 = 0; A1 < 2; A1++) {
                    for (A2 = 0; A2 < 2; A2++) {
                        for (A3 = 0; A3 < 2; A3++) {
                            for (B0 = 0; B0 < 2; B0++) {
                                for (B1 = 0; B1 < 2; B1++) {
                                    for (B2 = 0; B2 < 2; B2++) {
                                        for (B3 = 0; B3 < 2; B3++) {
                                            total++;
                                            alu_4bit(OP0, OP1, A0, A1, A2, A3, B0, B1, B2, B3,
                                                    &C0, &C1, &C2, &C3);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("%d test cases out of %d failed.\n", failed, total);

    return EXIT_SUCCESS;
}


BIT not_gate(BIT A) {
    /* TO DO: implement logical NOT */

    return !A;
}


BIT or_gate(BIT A, BIT B) {
    /* TO DO: implement logical OR */

    return A | B;
}


BIT and_gate(BIT A, BIT B) {
    /* TO DO: implement logical AND */

    return A & B;
}


BIT xor_gate(BIT A, BIT B) {
    /* TO DO: implement logical XOR */

    return A ^ B;
}


BIT multiplexer(BIT I0, BIT I1, BIT I2, BIT I3, BIT S0, BIT S1) {
    /* TO DO: implement a 4-input multiplexer */

    return or_gate(
            or_gate(
                    and_gate(
                            and_gate(not_gate(S0),
                                     not_gate(S1)),
                            I0), // not S0, not S1, I0
                    and_gate(
                            and_gate(not_gate(S0),
                                     S1),
                            I1) // not S0, S1, I1
            ),
            or_gate(
                    and_gate(
                            and_gate(S0,
                                     not_gate(S1)),
                            I2
                    ), // S0, not S1, I2
                    and_gate(
                            and_gate(S0,
                                     S1),
                            I3
                    ) // S0, S1, I3
            )
    );
}


void decoder(BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3) {
    /* TO DO: implement a 2-input decoder */
    *O0 = and_gate(not_gate(I0), not_gate(I1));
    *O1 = and_gate(not_gate(I0), I1);
    *O2 = and_gate(I0, not_gate(I1));
    *O3 = and_gate(I0, I1);

    return;
}

BIT full_adder_1bit(BIT A, BIT B, BIT CIN, BIT *COUT) {
    *COUT = or_gate(
            and_gate(A, B),
            and_gate(
                    xor_gate(A, B),
                    CIN
            )
    );

    return xor_gate( // (A XOR B) XOR CIN
            xor_gate(A, B),
            CIN
    );
}

BIT alu_1bit(BIT OP0, BIT OP1, BIT CIN, BIT A, BIT B, BIT *COUT) {
    /* TO DO: implement a 1-bit ALU */
    BIT binvert = xor_gate(and_gate(OP0, OP1), B); // Calculate Binvert
    CIN = or_gate(and_gate(OP0, OP1), CIN); // Set CIN to 1 when sub
    BIT result = full_adder_1bit(A, binvert, CIN, COUT);
    return multiplexer(
            and_gate(A, B), // 0 0
            or_gate(A, B), // 0 1
            result, // 1 0
            result, // 1 1
            OP0,
            OP1);
}


void alu_4bit(BIT OP0, BIT OP1, BIT A0, BIT A1, BIT A2, BIT A3,
              BIT B0, BIT B1, BIT B2, BIT B3,
              BIT *C0, BIT *C1, BIT *C2, BIT *C3) {
    /* TO DO: implement a 4-bit ALU */
    BIT COUT;
    *C0 = alu_1bit(OP0, OP1, 0, A0, B0, &COUT);
    *C1 = alu_1bit(OP0, OP1, COUT, A1, B1, &COUT);
    *C2 = alu_1bit(OP0, OP1, COUT, A2, B2, &COUT);
    *C3 = alu_1bit(OP0, OP1, COUT, A3, B3, &COUT);

    return;
}

void ieee754encode(float value, char *encoded) {
    /* TO DO: implement a function to encode a floating-point value
     * in the IEEE 754-1985 form
     *  input: 57.750000
     *  sign: 0
     *  exponent: 10000100
     *  fraction: 11001110000000000000000
     *  output: 01000010011001110000000000000000
     */

    printf("input: %f", value);

    char sign = value > 0 ? '1' : '0';
    printf("sign: %c\n", sign);

    int int_part = (int)value;
    float float_part = value - (int)value;
    int exponent[10];
    int fraction[24];
    int i = 0, j = 0;

    // Initialize array, fill empty spaces to -1
    for (i = 0; i < 10; i++) {
        exponent[i] = -1;
    }
    for (i = 0; i < 24; i++) {
        fraction[i] = -1;
    }


    // Convert int_part to binary
    i = 0;
    while(int_part != 0 && i < 10 )
    {
        exponent[i] = int_part % 2;
        int_part = int_part / 2;
        i++;
    }

    // Convert float_part to binary
    j = 0;
    while(float_part != 0 && j < 24)
    {
        fraction[j] = (int)(float_part*2);
        float_part = float_part * 2;
        float_part = float_part - (int)float_part;
        j++;
    }

    int int_count = i;
    int float_count = j;
    int fraction_part_count = i + j;
    int exponent_number = i - 1 + 127;
    int fraction_pointer = 9;



    i = 8;
    while(exponent_number != 0 && i >= 1 )
    {
        encoded[i] = ((exponent_number % 2 == 1) ? '1':'0');
        exponent_number = exponent_number / 2;
        i--;
    }

    printf("exponent: ");
    for (int k = 1; k < 9; ++k) {
        printf("%c",encoded[k]);
    }
    printf("\n");


    printf("fraction: ");
    int skip_first_1 = 0;
    for (i = int_count-1; i >= 0; --i) {
        if(tmp1[i]==-1)
        {
            continue;
        }
        if(skip_first_1==0 && tmp1[i] == 1)
        {
            skip_first_1 = 1;
            continue;
        }
        if(tmp1[i]==1)
        {
            encoded[fraction_pointer] = '1';
        }
        else
        {
            encoded[fraction_pointer] = '0';
        }
        printf("%d",tmp1[i]);
        fraction_pointer ++;
    }

    for (i = float_count-1; i >= 0 ; --i) {
        if(tmp2[i]==-1){
            continue;
        }
        if(tmp2[i]==1)
        {
            encoded[fraction_pointer] = '1';
        }
        else
        {
            encoded[fraction_pointer] = '0';
        }
        printf("%d",tmp2[i]);
        fraction_pointer ++;
    }

    for (;fraction_pointer < 32; ++fraction_pointer) {
        printf("0");
        encoded[fraction_pointer] = '0';
    }

    printf("\n");

    return;
}