/* hw1.c */
/* NAME: Xinhao Luo */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printArr( int x, int y, int* arr ) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%d ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

int main( int argc, char * argv[] )
{
/* Ensure we have the correct number of command-line arguments */
    if ( argc != 5 ) {
        fprintf( stderr, "ERROR: Invalid inputs!\n" );
        return EXIT_FAILURE;
    }

    int ax = atoi(argv[1]);
    int ay = atoi(argv[2]);
    int bx = atoi(argv[3]);
    int by = atoi(argv[4]);

    if (ay != bx) {
        fprintf( stderr, "ERROR: Invalid inputs!\n");
    }

    // init int arr
    int a[ax][ay];
    int b[bx][by];

    printf("Please enter the values for the first matrix (%dx%d)\n", ax, ay);
    for (int i = 0; i < ax; i++) {
        for (int j = 0; j < ay; j++) {
            scanf("%d", (&(&a)[i])[j]);
        }
    }
    printArr(ax, ay, a);
    printf("Please enter the values for the second matrix (%dx%d)", bx, by);

    printf("%d, %d, %d, %d", ax, ay, bx, by);

    return EXIT_SUCCESS;
}