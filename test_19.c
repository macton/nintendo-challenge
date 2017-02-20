#include <stdint.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"

int
main( void )
{
  int       size    = 64;
  uint32_t  a[2]    = { 0x249ff33f, 0x0000f7d9 };
  int       a_deg   = p_gf2_deg( size, a );
  uint32_t* bases   = p_gf2_nalloca( size, a_deg );
  int       basis_count;

  printf("OUTPUT -> index.html\n");

  html_open();

  p_gf2_berlekamp_nontrivial_bases_html( size, a, &basis_count, bases );

  html_print("<h2>find factors</h2>");

  html_print_p( "a", size, a );
  html_br();

  for (int i=0;i<basis_count;i++)
  {
    char bvname[64];
    sprintf(bvname,"bv[%d]",i);
    html_print_p( bvname, size, bases + (i * (size/32))  );
    html_br();
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

    p_gf2_gcd( size, a, s0, gcd0 );
    p_gf2_gcd( size, a, s1, gcd1 );

    char gcdname[64];

    sprintf(gcdname,"gcd( a, bv[%d]+0 )",i);
    html_print_p( gcdname, size, gcd0 );
    html_br();

    sprintf(gcdname,"gcd( a, bv[%d]+1 )",i);
    html_print_p( gcdname, size, gcd1 );
    html_br();
  }

  html_close();

  return 0;
}
