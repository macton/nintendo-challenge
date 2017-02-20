#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int size = 32;

  uint32_t a = 0x67;
  uint32_t b = 0x67;
  uint32_t quotient;
  uint32_t remainder;

  p_gf2_mul( size, &a, &b );
  p_gf2_div_print( size, &a, &b, &quotient, &remainder );

  return 0;
}
