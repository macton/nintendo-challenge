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

  return 0;
}
