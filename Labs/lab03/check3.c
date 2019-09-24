#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char * argv[]) {
    char str [128];
    FILE *f = fopen("book-1984.txt", "rb");
    if (f == NULL) perror ("Error opening file");
    else {
        while (fgets (str , 128 , f) != NULL ) {
            char * c = str;
            char count = 0;
            for (; *c != '\0'; c++) {
                if (isalnum(*c)) {
                    printf("%c", *c);
                    count++;
                }
            }
            if (count != 0) {
                printf(" [%d alnum chars]\n", count);
            }
        }
    }

    fclose (f);

    return 0;
}