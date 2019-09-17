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
            int len = strlen(str);
            int count = 0;
            for (int i = 0; i < len; i++) {
                if (isalnum(str[i])) {
                    printf("%c", str[i]);
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