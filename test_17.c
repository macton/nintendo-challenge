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
  uint32_t* da      = p_gf2_alloca( size * 2 );

  // P = x^(i*2); -- note: size*2
  uint32_t* P       = p_gf2_nalloca( size*2, a_deg );

  // Q = P mod a; -- note: size
  uint32_t* Q       = p_gf2_nalloca( size, a_deg );

  // I = identity -- note:size, really a square matrix of a_deg x a_deg
  uint32_t* I       = p_gf2_nalloca( size, a_deg );

  // M = Q-I
  uint32_t* M       = p_gf2_nalloca( size, a_deg );

  // W = augmented M (a_deg*2)
  int       size_w  = (((a_deg*2) + 31) >> 5) << 5;
  uint32_t* W       = p_gf2_nalloca( size_w, a_deg );

  p_gf2_zero( size*2, da );
  p_gf2_mov( size, da, &a );

  p_gf2_print_p( "a", size, &a );
  printf("\n");

  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((size*2)/32) * i);
    uint32_t* qv = Q + ((size/32) * i);
    uint32_t* iv = I + ((size/32) * i);
    uint32_t* mv = M + ((size/32) * i);
    uint32_t* wv = W + ((size_w/32) * i);

    uint32_t* s = p_gf2_alloca( size * 2 );
    uint32_t* t = p_gf2_alloca( size * 2 );

    // P[i] = x^(i*2)
    p_gf2_one( size*2, pv );
    p_gf2_sll( size*2, pv, i*2 );

    // Q[i] = P[i] mod a
    p_gf2_div( size*2, pv, da, t, s );
    p_gf2_mov( size, qv, s );

    // I[i] = 1 << i
    p_gf2_one( size, iv );
    p_gf2_sll( size, iv, i );

    // M[i] = Q[i] - I[i]
    p_gf2_mov( size, mv, qv );    
    p_gf2_add( size, mv, iv );

    // W[i] = (M[i] << deg(a)) | (1 << i)
    p_gf2_mov( size, wv, mv );
    p_gf2_sll( size_w, wv, a_deg );
    p_gf2_add( size, wv, iv );
  }

  printf("Q = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((size*2)/32) * i);
    uint32_t* qv = Q + ((size/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    p_gf2_print_bin_w( size*2, pv, a_deg*2 );
    printf(" mod a = ");
    p_gf2_print_bin_w( size, qv, a_deg );
    printf("\n");
  }
  printf("\n");

  printf("I = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* iv = I + ((size/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    p_gf2_print_bin_w( size, iv, a_deg );
    printf("\n");
  }
  printf("\n");

  printf("M = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* mv = M + ((size/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    p_gf2_print_bin_w( size, mv, a_deg );
    printf("\n");
  }
  printf("\n");

  printf("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* wv = W + ((size_w/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    p_gf2_print_bin_w_aug( size_w, wv, a_deg*2 );
    printf("\n");
  }
  printf("\n");

  printf("w = {");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* wv = W + ((size_w/32) * i);
    printf("{");

    for (int j=(a_deg*2)-1;j>=0;j--)
    {
      printf("%c",p_gf2_bt( size_w, wv, j )?'1':'0' );
      if ( j > 0 )
      {
        printf(",");
      }
    }

    printf("}");
    if (i < (a_deg-1))
    {
      printf(",");
    }
  }
  printf("}\n");
  printf("\n");

  printf("STEPS rref[W] -> index.html\n");

  html_open();

  p_gf2_rref_html( size_w, W, a_deg );

  html_println("W in augmented form");
  html_indent_push();
  html_code_begin();
  html_print("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* v  = W + ( p_gf2_wc(size_w) * i );

    if (i>0)
    {
      html_print("    ");
    }
     
    for (int j=(a_deg+1)+(a_deg-1);j>=(a_deg+1);j--)
    {
      html_print("%c",p_gf2_bt(size_w,v,j)?'1':'0');
    }

    html_print(" ");
    for (int j=(a_deg-1);j>=0;j--)
    {
      html_print("%c",p_gf2_bt(size_w,v,j)?'1':'0');
    }
    html_print("\n");
  }
  html_indent_pop();
  html_code_end();

  html_println("Highlight null basis vectors (where left side of row is zero)");
  html_indent_push();
  html_code_begin();
  html_print("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* v  = W + ( p_gf2_wc(size_w) * i );
    int is_null_basis = 1;

    if (i>0)
    {
      html_print("    ");
    }

    for (int j=(a_deg+1)+(a_deg-1);j>=(a_deg+1);j--)
    {
      is_null_basis &= (!p_gf2_bt(size_w,v,j));
    }
     
    if (is_null_basis)
    {
      html_print("<span style=\"color:red\">");
    } 
    for (int j=(a_deg+1)+(a_deg-1);j>=(a_deg+1);j--)
    {
      html_print("%c",p_gf2_bt(size_w,v,j)?'1':'0');
    }

    html_print(" ");
    for (int j=(a_deg-1);j>=0;j--)
    {
      html_print("%c",p_gf2_bt(size_w,v,j)?'1':'0');
    }
    if (is_null_basis)
    {
      html_print("</span>");
    } 
    html_print("\n");
  }
  html_indent_pop();
  html_code_end();

  html_println("Null basis vectors");
  html_br();
  html_indent_push();
  int basis_ndx = 0;
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* v   = W + ( p_gf2_wc(size_w) * i );
    uint32_t* zv  = p_gf2_alloca( size_w );

    p_gf2_mov( size_w, zv, v );
    p_gf2_srl( size_w, zv, a_deg+1 );

    if ( p_gf2_eqz( size_w, zv ) )
    {
      char nvname[64];
      sprintf(nvname,"nv<sub>%d</sub>",basis_ndx);
      html_print_p(nvname,size_w,v);
      html_br();

      basis_ndx++;
    }
  }
  html_indent_pop();

  html_hr();
  html_references_push();
  html_reference("http://math.stackexchange.com/questions/1612616/how-to-find-null-space-basis-directly-by-matrix-calculation");
  html_reference("http://people.tamu.edu/~aestrugo/data/null_space.pdf");
  html_reference("http://pages.pacificcoast.net/~cazelais/251/gauss-jordan.pdf");
  html_reference("http://matrix.reshish.com/gauss-jordanElimination.php");
  html_references_pop();

  html_close();

  printf("\n");
  printf("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* wv = W + ((size_w/32) * i);

    if (i > 0)
    {
      printf("    ");
    }
    p_gf2_print_bin_w_aug( size_w, wv, a_deg*2 );
    printf("\n");
  }
  printf("\n");

  return 0;
}
