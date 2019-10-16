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
    if (carry_out) // NULL checking
        *carry_out = (d0 & d1) + (carry_in & (d0 | d1));

    return d0 ^ d1 ^ carry_in;
}


int main()
{
    int i;
    int d0[4] = { 1, 0, 0, 1 };
    int d1[4] = { 1, 1, 1, 1 };
    int ci[4] = { 0, 0, 0, 0 };
    int sum[4] = { 0, 0, 0, 0 };

    for (int i = 0; i < 4; i++) {
        if (i - 1 >= 0) {
            sum[i] = add_two_bits(d0[i], d1[i], ci[i - 1], &ci[i]);
        } else {
            sum[i] = add_two_bits(d0[i], d1[i], NULL, &ci[i]);
        }

    }

    for (int i = 0; i < 4; i++)
    {
        printf("%d ",d0[i]);
    }
    printf("+");
    for (int i = 0; i < 4; i++)
    {
        printf(" %d",d1[i]);
    }
    printf(" =");
    for (int i = 0; i < 4; i++)
    {
        printf(" %d",sum[i]);
    }
    if(ci[3] == 1) {
        printf(" with a carry\n");
    } else {
        printf(" without carry\n");
    }

    return 0;
}