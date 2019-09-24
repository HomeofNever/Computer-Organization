#include "stdio.h"
#include "stdlib.h"
#include "string.h"
 
#define CACHE 500

long fib_native(long a) 
{
    if (a == 0) {
        return 2;
    } else if (a == 1) {
        return 5;
    } else {
        return fib_native(a - 1) + fib_native(a - 2);
    }
}

long fib_array(long a, long *cache) 
{
    if (a == 0) {
        return 2;
    } else if (a == 1) {
        return 5;
    } else if (cache[a] != 0) {
        return cache[a];
    } else {
        cache[a] = fib_array(a - 1, cache) + fib_array(a - 2, cache);
        return cache[a];
    }
}


int main(int argc, char *argv[]) {
    // init
    long cache[CACHE];
    for(int i = 0; i < CACHE; i++){
      cache[i] = 0;
    }

    for (int i = 1; i < argc; i++) {
        long num = atol(argv[i]);
        // long result = fib_native(num);
        long result = fib_array(num, cache);
        printf("fib(%ld) is %ld\n", num, result);
    }
}