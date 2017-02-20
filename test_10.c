#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int size = 64;

  uint32_t a[2] = { 0x7fe00033, 0x0caea3f4 };
  uint32_t b[2];
  uint32_t gcd[2];

  p_gf2_print_p( "f   ", size, a );
  printf("\n");

  p_gf2_derivative( size, a, b );
  p_gf2_print_p( "f'  ", size, b );
  printf("\n");

  p_gf2_gcd( size, a, b, gcd );
  p_gf2_print_p( "gcd ", size, gcd );
  printf("\n");
  
  return 0;
}
