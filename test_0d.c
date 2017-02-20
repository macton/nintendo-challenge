#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int size = 64;
  uint32_t in[] = { 0x7fe00033, 0x0caea3f4 };

  printf("p1 = ");
  p_gf2_print( size, in );
  printf("\n");

  uint32_t* p_a = p_gf2_alloca( size );
  uint32_t* p_b = p_gf2_alloca( size );

  p_a[1] = 0;
  p_b[1] = 0;

  // - (1+x) = (0x3)
  // - (1+x+x^2+x^3+x^5) = (0x2f)
  p_a[0] = 0x03;
  p_b[0] = 0x2f;

  p_gf2_mul( size, p_a, p_b );

  // - (1+x+x^2+x^4+x^5) = (0x37)
  p_b[0] = 0x37;
  p_gf2_mul( size, p_a, p_b );
  
  // - (1+x^6+x^7) = (0xc1)
  p_b[0] = 0xc1;
  p_gf2_mul( size, p_a, p_b );

  // - (1+x^4+x^5+x^6+x^8) = (0x171)
  p_b[0] = 0x171;
  p_gf2_mul( size, p_a, p_b );

  // - (1+x^3+x^4+x^7+x^9+x^11+x^15+x^16+x^17+x^18+x^21)= (0x278a99)
  p_b[0] = 0x278a99;
  p_gf2_mul( size, p_a, p_b );

  printf("b = ");
  p_gf2_print( size, p_a );
  printf("\n");

  printf("b = 0x%08x 0x%08x\n",p_a[0],p_a[1]);
  printf("\n");

  return 0;
}
