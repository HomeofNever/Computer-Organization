#include <stdio.h>
#include <string.h>
#include <math.h>

void first(int num) {
    for (int i = 0; i < num; i++){
        for (int j = 0; j < i * 2 + 1; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void second(int num) {
    for (int i = 0; i < num; i++){
        int size = i * 2 + 1;
        char str[size + 1];
        memset(str, '*', size * sizeof(char));
        str[size] = '\0';
        printf("%s\n", str);
    }
}

void calc(int height) {
    int base = height * 2 - 1;
    printf("The hypotenuse is %.2f", sqrt(base * base + height * height));
}

int main() {
    printf("What is n? ");
    int num;
    scanf("%d", &num);
    // first(num);
    second(num);
    calc(num);
}