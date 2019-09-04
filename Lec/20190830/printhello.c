#include <stdio.h>
#include <stdlib.h>

void printhello( int n )
{
  int i;
  for ( i = 0 ; i < n ; i++ )
  {
    printf( "Hello World\n" );
  }

  printf( "blah blah blah\n" );
}

int main()
{
  printhello( 5 );

  return EXIT_SUCCESS;
}
