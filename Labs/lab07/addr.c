/* adder.c */

#include <stdio.h>
#include <stdlib.h>

/* Use inputs d0, d1, and carry_in to compute both the sum
 * (which is returned) and the carry_out (passed by reference)
 *
 * Note that carry_out could be NULL, indicating that we do
 * not want to determine carry_out (i.e., only return the sum)
 */
int add_two_bits(int d0, int d1, int carry_in, int *carry_out)
{
    /* FILL IN CODE HERE! */
    if (d0 == 1 && d1 == 1)
        *carry_out = 1;
    else
        *carry_out = NULL;

    return d0 & d1;
}

int main()
{
    int d0, d1, sum, c_out;

    printf( "Enter binary digit 0: " );
    scanf( "%d", &d0 );

    /* Clean the user-generated input */
    d0 = !!d0;

    printf( "Enter binary digit 1: " );
    scanf( "%d", &d1 );

    /* Clean the user-generated input */
    d1 = !!d1;

    sum = add_two_bits( d0, d1, 0, &c_out );

    if ( c_out )
    {
        printf( "%d + %d = %d with a carry\n", d0, d1, add_two_bits( d0, d1, 0, NULL ) );
    }
    else
    {
        printf( "%d + %d = %d without a carry\n", d0, d1, add_two_bits( d0, d1, 0, NULL ) );
    }

    return 0;
}