#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int       size    = 32;
  uint32_t  a       = 0x37;
  uint32_t  a1      = 0xc1;

  printf("Start with known irreducible factors (a0,a1)...\n");

  p_gf2_print_p( "a0", size, &a );
  printf("\n");

  p_gf2_print_p( "a1", size, &a1 );
  printf("\n");

  printf("a = a0 * a1\n");
  printf("\n");
  p_gf2_mul( size, &a, &a1 );

  p_gf2_print_p( "a", size, &a );
  printf("\n");

  return 0;
}
