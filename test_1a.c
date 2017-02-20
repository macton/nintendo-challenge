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
  uint32_t* one     = p_gf2_alloca( size );
  int       basis_count;

  p_gf2_one( size, one );

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

  uint32_t* known_factors      = p_gf2_nalloca( size, a_deg );
  int       known_factor_count = 1;

  p_gf2_mov( size, known_factors, a );

  for (int i=0;i<basis_count;i++)
  {
    uint32_t* bv   = bases + (i * (size/32));
    uint32_t* s0   = p_gf2_alloca( size );
    uint32_t* s1   = p_gf2_alloca( size );
    uint32_t* gcd0 = p_gf2_alloca( size );
    uint32_t* gcd1 = p_gf2_alloca( size );
    uint32_t* f0   = p_gf2_alloca( size );
    uint32_t* f1   = p_gf2_alloca( size );
    uint32_t* r0   = p_gf2_alloca( size );
    uint32_t* r1   = p_gf2_alloca( size );

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


    for (int j=0;j<known_factor_count;j++)
    {
      uint32_t* kf = known_factors + (j*(size/32));

      p_gf2_div( size, kf, gcd0, f0, r0 );
      if ( p_gf2_eqz( size, r0 ) ) 
      {
        if ( !p_gf2_eq( size, f0, one ) )
        {
          // replace known factor
          p_gf2_mov( size, kf, gcd0 );
  
          // add new known factor
          p_gf2_mov( size, known_factors + (known_factor_count*(size/32)), f0 );
          known_factor_count++;
  
          html_print_p( "* factor[0] ", size, gcd0 );
          html_br();
          html_print_p( "* factor[1] ", size, f0 );
          html_br();

          break;
        }
      }
    }

    for (int j=0;j<known_factor_count;j++)
    {
      uint32_t* kf = known_factors + (j*(size/32));

      p_gf2_div( size, kf, gcd1, f1, r1 );
      if ( p_gf2_eqz( size, r1 ) ) 
      {
        if ( !p_gf2_eq( size, f1, one ) )
        {
          // replace known factor
          p_gf2_mov( size, kf, gcd1 );
  
          // add new known factor
          p_gf2_mov( size, known_factors + (known_factor_count*(size/32)), f1 );
          known_factor_count++;
  
          html_print_p( "* factor[0] ", size, gcd1 );
          html_br();
          html_print_p( "* factor[1] ", size, f1 );
          html_br();

          break;
        }
      }
    }
  }

  html_print("<h2>non-trivial factors of a</h2>");

  for (int j=0;j<known_factor_count;j++)
  {
    uint32_t* kf = known_factors + (j*(size/32));

    char kfname[64];
    sprintf(kfname,"kf<sub>%d</sub>",j);

    html_print_p( kfname, size, kf );
    html_br();
  }

  html_close();

  return 0;
}
