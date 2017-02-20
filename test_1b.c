#include <stdint.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"

int
main( void )
{
  int       size          = 64;
  uint32_t  a[2]          = { 0x249ff33f, 0x0000f7d9 };
  int       a_deg         = p_gf2_deg( size, a );

  printf("OUTPUT -> index.html\n");

  html_open();

  uint32_t* irreducible_factors      = p_gf2_nalloca( size, a_deg+1 );
  int       irreducible_factor_count = 1;
  p_gf2_mov( size, irreducible_factors, a );

  int i=0;
  while (i<irreducible_factor_count)
  {
    uint32_t* next_factor   = irreducible_factors + (i * (size/32));
    uint32_t* known_factors = irreducible_factors + (irreducible_factor_count * (size/32));
    int       known_factor_count;

    char fname[64];
    sprintf(fname,"f<sub>%d</sub>",i);

    html_print_p( fname, size, next_factor );
    html_br();

    html_page_push( "p_gf2_berlekamp_nontrivial_factors_html( %s )", fname );

    p_gf2_berlekamp_nontrivial_factors_html( size, next_factor, &known_factor_count, known_factors );

    html_page_pop();
    html_br();
    html_br();

    // remove next_factor if it added new known_factors
    if ( known_factor_count > 1 )
    {
      int  after_factors_count = (irreducible_factor_count+known_factor_count) - (i+1);
      for (int j=0;j<after_factors_count;j++)
      {
        p_gf2_mov( size, next_factor + (j*(size/32)), next_factor + ((j+1)*(size/32)) );
      }

      irreducible_factor_count += (known_factor_count-1);
    }
    else
    {
      i++;
    }
  }

  html_print("<h2>irreducible factors</h2>");

  i = 0;
  while (i<irreducible_factor_count)
  {
    uint32_t* next_factor = irreducible_factors + (i * (size/32));

    char fname[64];
    sprintf(fname,"f<sub>%d</sub>",i);

    html_print_p( fname, size, next_factor );
    html_br();

    i++;
  }

  html_print("<h2>verify</h2>");
  uint32_t*  b = p_gf2_alloca( size );

  p_gf2_mov( size, b, irreducible_factors );

  i = 1;
  while (i<irreducible_factor_count)
  {
    uint32_t* next_factor = irreducible_factors + (i * (size/32));
    p_gf2_mul( size, b, next_factor );
    i++;
  }

  html_println("b = product of irreducible factors");
  html_br();
  html_print_p( "b", size, b );
  html_br();
  
  if ( p_gf2_eq( size, b, a ) )
  {
    html_println( "<span style='color:green'>OK!</span>" );
    printf("OK!\n");
  }
  else
  {
    html_println( "<span style='color:red'>FAILED!</span>" );
    printf("FAILED!\n");
  }
  html_br();

  html_close();

  return 0;
}
