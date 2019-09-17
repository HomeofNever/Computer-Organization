#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void int_reader(char * chr, long size) {
    for (int i = 0; i < size / sizeof(int); i++) {
        int j;
        memcpy(&j, chr + i * sizeof(int), sizeof(int));
        printf("Data point # %d: %d\n", i, j);
    }
}

void unsigned_long_reader(char * chr, long size) {
    for (int i = 0; i < size / sizeof(unsigned long); i++) {
        unsigned long j;
        memcpy(&j, chr + i * sizeof(unsigned long), sizeof(unsigned long));
        printf("Data point # %d: %lu\n", i, j);
    }
}

int main(int argc, char * argv[]) {
    FILE *f = fopen("lab03-data.dat", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char * chr = malloc(fsize);
    fread(chr, 1, fsize, f);
    fclose(f);

    int_reader(chr, fsize);
    printf("\n");
    unsigned_long_reader(chr, fsize);

    return 0;
}