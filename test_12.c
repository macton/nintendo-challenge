#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int       size    = 32;
  uint32_t  a       = 0x31;
  int       a_deg   = p_gf2_deg( size, &a );
  uint32_t* da      = p_gf2_alloca( size * 2 );

  // P = x^(i*2); -- note: size*2
  uint32_t* P       = p_gf2_nalloca( size*2, a_deg );

  // Q = P mod a; -- note: size
  uint32_t* Q       = p_gf2_nalloca( size, a_deg );

  p_gf2_zero( size*2, da );
  p_gf2_mov( size, da, &a );

  p_gf2_print_p( "a", size, &a );
  printf("\n");

  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((size*2)/32) * i);
    uint32_t* qv = Q + ((size/32) * i);

    uint32_t* s = p_gf2_alloca( size * 2 );
    uint32_t* t = p_gf2_alloca( size * 2 );

    // P[i] = x^(i*2)
    p_gf2_one( size*2, pv );
    p_gf2_sll( size*2, pv, i*2 );

    // Q[i] = P[i] mod a
    p_gf2_div( size*2, pv, da, t, s );
    p_gf2_mov( size, qv, s );
  }

  printf("Q = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((size*2)/32) * i);
    uint32_t* qv = Q + ((size/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    printf("x^%d mod a = ",i*2);
    p_gf2_print_bin_w( size*2, pv, (a_deg * 2) );
    printf(" mod a = ");
    p_gf2_print_bin_w( size, qv, a_deg );
    printf("\n");

  }
  return 0;
}
