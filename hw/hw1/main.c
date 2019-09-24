/* hw1.c */
/* NAME: Xinhao Luo */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Matrix{
    int row;
    int col;
    int **data;
};

void initMatrix(struct Matrix *m) {
  m->data = calloc(m->row, sizeof(int *));
  for (int i = 0; i < m->row; i++) {
    // init each slot
    m->data[i] = calloc(m->col, sizeof(int));
  }
}

void freeMatrix(struct Matrix *m) {
  for (int i = 0; i < m->row; i++) {
    free(m->data[i]);
  }
  free(m->data);
}

void readMatrix(struct Matrix *m) {
  for (int i = 0; i < m->row; i++) {
    for (int j = 0; j < m->col; j++) {
      scanf("%d", &m->data[i][j]);
    }
  }
}

int length(int num)
{
  int count = 0;
  while(num != 0)
  {
    num /= 10;
    count++;
  }
  return count;
}

void calculateLength(int *a, struct Matrix *m) {
  for (int j = 0; j < m->col; j++) {
    int tmp = -1;
    for (int i = 0; i < m->row; i++) {
      int l = length(m->data[i][j]);
      if (l > tmp) {
        tmp = l;
      }
    }
    a[j] = tmp;
  }
}

void printMatrix(int *lengths, struct Matrix *m) {
  for (int i = 0; i < m->row; i++) {
    printf("[");
    for (int j = 0; j < m->col; j++) {
      // First, acquire current length
      int len = length(m->data[i][j]);
      // # of space acquired
      int diff = lengths[j] - len;
      for (int k = 0; k < diff; k++) {
        printf(" ");
      }
      printf("%d", m->data[i][j]);
      // There should be space between numbers
      // Except the last one (no number behind)
      if (j != m->col - 1) {
        printf(" ");
      }
    }
    printf("]\n");
  }
}

void multipleMatrix(struct Matrix *a, struct Matrix *b, struct Matrix *result) {
  for (int i = 0; i < a->row; i++) {
    for (int j = 0; j < a->col; j++) {
      for (int k = 0; k < b->col; k++) {
        result->data[i][k] += a->data[i][j] * b->data[j][k];
      }
    }
  }
}

int main(int argc, char *argv[])
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
        return EXIT_FAILURE;
    }

    // init
    struct Matrix a, b, result;
    int *aLengths, *bLengths, *resultLengths;

    a.row = ax;
    a.col = ay;
    initMatrix(&a);
    aLengths = calloc(a.col, sizeof(int));

    b.row = bx;
    b.col = by;
    initMatrix(&b);
    bLengths = calloc(b.col, sizeof(int));

    result.row = ax;
    result.col = by;
    initMatrix(&result);
    resultLengths = calloc(result.col, sizeof(int));

    // User Input
    printf("Please enter the values for the first matrix (%dx%d):\n", ax, ay);
    readMatrix(&a);
    calculateLength(aLengths, &a);

    printf("Please enter the values for the second matrix (%dx%d):\n", bx, by);
    readMatrix(&b);
    calculateLength(bLengths, &b);

    // Calculation
    multipleMatrix(&a, &b, &result);
    calculateLength(resultLengths, &result);

    // Result
    printf("\n");
    printMatrix(aLengths, &a);
    printf("multiplied by\n");
    printMatrix(bLengths, &b);
    printf("equals\n");
    printMatrix(resultLengths, &result);

    // Free as a bird!
    free(aLengths);
    free(bLengths);
    free(resultLengths);
    freeMatrix(&a);
    freeMatrix(&b);
    freeMatrix(&result);

    return EXIT_SUCCESS;
}
