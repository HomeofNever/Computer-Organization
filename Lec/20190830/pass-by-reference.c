#include <stdio.h>
#include <stdlib.h>
                               // C++ equivalent:
                               //
int foo( int * x )             // int foo(int & x)
{                              // {
  return (*x) * (*x);          //   return x * x;
}                              // }

int main()                     // int main()
{                              // {
  int y = 4;                   //   int y = 4;
  return foo( &y );            //   return foo(y);
}                              // }

   // in the shell, after you execute this code, try this:
   //
   //    echo $?


   //    &y   <==  "address-of variable y"

   //    *x   <==  "dereference variable x" or "follow the pointer..."

