#include <stdio.h>
long gcd(long a,long b) 
{
    if (b < 0 || b > 0)
    {
        return gcd(b,a % b);
    }
    else
    {   
        return a;
    }
 
}
int main(int argc, char *argv[]) {
    long a,b;
    printf("Input the first number: ");
    scanf("%ld",&a);
    printf("Input the second number: ");
    scanf("%ld",&b);

    printf("The result is: %ld",gcd(a, b));
}