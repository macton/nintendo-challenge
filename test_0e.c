#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int size      = 64;
  uint32_t a[2] = { 0x249ff33f, 0x0000f7d9 };
  uint32_t b[2];

  p_gf2_derivative( size, a, b );

  printf("a  = ");
  p_gf2_print( size, a );
  printf("\n");
  printf("\n");

  printf("a' = ");
  p_gf2_print( size, b );
  printf("\n");

  return 0;
}
