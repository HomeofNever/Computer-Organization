#include "stdio.h"
#include "stdlib.h"
 
long fib1(long i){
    if(i == 0){
        return 0;
    }
    if (i == 1) {
        return 1;
    }
    return fib1(i - 1) + fib1(i - 2);
}
 
long fib2(long n)
{
    long i, t1 = 0, t2 = 1, next, current;
      
    for (i = 0; i <= n; ++i)
    {
        current = t1;
        next = t1 + t2;
        t1 = t2;
        t2 = next;
    }
    return current;
}
 
int main(int argc, char *argv[]) 
{
    long fib,result;
    printf("Input the fib number: ");
    scanf("%ld",&fib);
     
    //result = fib1(fib);
    result = fib2(fib);
    printf("The result is: %ld",result);
     
    return 0;
}