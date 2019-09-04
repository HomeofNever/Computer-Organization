#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strlen, strcpy, etc. */

char * example_string = "this is an example string";


/* reverses a string in place
   returns a pointer to the string       [0] [1] [2] [3] [4] [5]
*/                             /*       +------------------------+ */
char *reverse( char * s )      /* s --> | H | e | l | l | o | \0 | */
{                              /*       +------------------------+ */
  char * buf;
  int i, len;
  len = strlen( s );	/* find the length of the input string s */

  buf = malloc( len + 1 );     /* for "Hello" dynamically allocate 6 bytes */

  for ( i = 0 ; i < len ; i++ )
  {
     buf[i] = s[len-i-1];
  }
  buf[i] = '\0';	/* null terminate the string! */
  strcpy( s, buf );	/* copy the reversed string from buf back into s */

  free( buf );          /* similar to delete in C++, free up the dynamically
                               allocated memory */

  return s;
}

int main( int argc, char* argv[] )
{
  printf( "The example string is: %s\n", example_string );

  if ( argc < 2 )
  {
    printf( "Invalid usage - must supply a string\n" );
    return EXIT_FAILURE;
  }

  printf( "%s\n", reverse( argv[1] ) );

  return EXIT_SUCCESS;
}


#if 0
                    +--+
     argv =====>[0] | ====> "./a.out\0"
                    +--+
                [1] | ====> "Hello\0"
                    +--+
                [2] | ====> "there\0"
                    +--+
                [3] |  |
                    +--+
#endif
