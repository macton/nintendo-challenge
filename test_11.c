#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

int
main( void )
{
  int       size             = 64;
  uint32_t  a[2]             = { 0x7fe00033, 0x0caea3f4 };

  int       a_deg            = p_gf2_deg( size, a );
  int       sf_factor_count  = 0;
  uint32_t* sf_factors       = p_gf2_nalloca( size, a_deg );
  int*      sf_factors_pow   = (int*)__builtin_alloca( sizeof(int) * a_deg );

  p_gf2_print_p( "a", size, a );
  printf("\n");

  p_gf2_sff( size, a, &sf_factor_count, sf_factors, sf_factors_pow, 0 );
  p_gf2_print_p( "sff(a) = ", size, a );
  printf("\n");

  printf("\n");
  for (int i=0;i<sf_factor_count;i++)
  {
    printf( "sff(a,%d) = ",i);
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf("(");
    }
    p_gf2_print( size, sf_factors + (size/32) * i );
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf(")^%d",sf_factors_pow[i]);
    }
    printf("\n");
  }

  printf("\n");
  for (int i=0;i<sf_factor_count;i++)
  {
    printf( "sff(a,%d) = ",i);
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf("(");
    }
    p_gf2_print_bin_skiplz( size, sf_factors + (size/32) * i );
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf(")^%d",sf_factors_pow[i]);
    }
    printf("\n");
  }

  printf("\n");
  for (int i=0;i<sf_factor_count;i++)
  {
    printf( "sff(a,%d) = ",i);
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf("(");
    }
    p_gf2_print_hex_skiplz( size, sf_factors + (size/32) * i );
    if ( sf_factors_pow[i] > 1 ) 
    {
      printf(")^%d",sf_factors_pow[i]);
    }
    printf("\n");
  }

  return 0;
}
