#include <stdint.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"

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

  int       a_deg   = p_gf2_deg( size, &a );
  uint32_t* bases   = p_gf2_nalloca( size, a_deg );
  int       basis_count;

  printf("STEPS -> index.html\n");
  printf("\n");

  html_open();

  p_gf2_berlekamp_nontrivial_bases_html( size, &a, &basis_count, bases );

  html_close();

  printf("basis_count = %d\n",basis_count);
  printf("\n");

  for (int i=0;i<basis_count;i++)
  {
    char bvname[64];
    sprintf(bvname,"bv[%d]",i);
    p_gf2_print_p( bvname, size, bases + (i * (size/32))  );
    printf("\n");
  }

  for (int i=0;i<basis_count;i++)
  {
    uint32_t* bv   = bases + (i * (size/32));
    uint32_t* s0   = p_gf2_alloca( size );
    uint32_t* s1   = p_gf2_alloca( size );
    uint32_t* gcd0 = p_gf2_alloca( size );
    uint32_t* gcd1 = p_gf2_alloca( size );

    p_gf2_mov( size, s0, bv );
    p_gf2_mov( size, s1, bv );
    p_gf2_inc( size, s1 );

    p_gf2_gcd( size, &a, s0, gcd0 );
    p_gf2_gcd( size, &a, s1, gcd1 );

    char gcdname[64];

    sprintf(gcdname,"gcd( a, bv[%d]+0 )",i);
    p_gf2_print_p( gcdname, size, gcd0 );

    printf("\n");

    sprintf(gcdname,"gcd( a, bv[%d]+1 )",i);
    p_gf2_print_p( gcdname, size, gcd1 );
  }

  return 0;
}
