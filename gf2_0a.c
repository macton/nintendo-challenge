#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "gf2.h"

void
p_gf2_zero( int bit_size, uint32_t* x )
{
  memset( x, 0, sizeof(uint32_t) * p_gf2_wc(bit_size) );
}

void
p_gf2_one( int bit_size, uint32_t* x )
{
  p_gf2_zero( bit_size, x );
  x[0] = 1;
}

void
p_gf2_mov( int bit_size, uint32_t* x, uint32_t* y )
{
  memcpy( x, y, sizeof(uint32_t) * p_gf2_wc(bit_size) );
}

void
p_gf2_movzx( int x_bit_size, uint32_t* x, int y_bit_size, uint32_t* y )
{
  int d_bit_size = x_bit_size - y_bit_size;
  memcpy( x, y, sizeof(uint32_t) * p_gf2_wc(y_bit_size) );
  if (d_bit_size > 0)
  {
    p_gf2_zero( d_bit_size, x+p_gf2_wc(y_bit_size)  );
  }
}

void
p_gf2_sll( int bit_size, uint32_t* x, int sa )
{
  int word_count   = p_gf2_wc(bit_size);

  while (sa > 0)
  {
    int top_word_ndx = word_count-1;
    int word_sa      = sa & 0x1f;

    if (word_sa == 0)
    {
      while (top_word_ndx > 0)
      {
        x[ top_word_ndx ] = x[ top_word_ndx-1 ];
        top_word_ndx--;
      }
      x[0] = 0;
      word_sa = 32;
    }
    else
    {
      while (top_word_ndx > 0)
      {
        uint32_t top_word      = x[ top_word_ndx ] << word_sa;
        uint32_t low_word_part = x[ top_word_ndx-1 ] >> (32-word_sa);
  
        x[ top_word_ndx ] = top_word | low_word_part;
        top_word_ndx--;
      }
      x[0] <<= word_sa;
    }

    sa -= word_sa;
  }
}

void
p_gf2_add( int bit_size, uint32_t* x, uint32_t* y )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    x[ top_word_ndx ] = x[ top_word_ndx ] ^ y[ top_word_ndx ];
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void
p_gf2_mul( int bit_size, uint32_t* x, uint32_t* y )
{
  uint32_t* a = p_gf2_alloca( bit_size );

  p_gf2_mov( bit_size, a, x );
  p_gf2_zero( bit_size, x );

  for (int i=0;i<bit_size;i++)
  {
    if ( p_gf2_bt( bit_size, y, i ) )
    {
      p_gf2_add( bit_size, x, a );
    }
    p_gf2_sll( bit_size, a, 1 );
  }
}

void
p_gf2_and( int bit_size, uint32_t* x, uint32_t* y )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    x[ top_word_ndx ] = x[ top_word_ndx ] & y[ top_word_ndx ];
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void
p_gf2_oddmask( int bit_size, uint32_t* x )
{
  memset( x, 0xAA, sizeof(uint32_t) * p_gf2_wc(bit_size) );
}

void
p_gf2_srl( int bit_size, uint32_t* x, int sa )
{
  int word_count   = p_gf2_wc(bit_size);

  while (sa > 0)
  {
    int top_word_ndx = 0;
    int word_sa  = sa & 0x1f;

    if (word_sa == 0)
    {
      while (top_word_ndx < (word_count-1))
      {
        x[ top_word_ndx ] = x[ top_word_ndx+1 ];
        top_word_ndx++;
      }
      x[top_word_ndx] = 0;
      word_sa = 32;
    }
    else
    {
      while (top_word_ndx < (word_count-1))
      {
        uint32_t top_word       = x[ top_word_ndx ] >> word_sa;
        uint32_t high_word_part = x[ top_word_ndx+1 ] << (32-word_sa);
  
        x[ top_word_ndx ] = top_word | high_word_part;
        top_word_ndx++;
      }
      x[top_word_ndx] >>= word_sa;
    }

    sa -= word_sa;
  }
}

void
p_gf2_derivative( int bit_size, uint32_t* a, uint32_t* derivative )
{
  p_gf2_oddmask( bit_size, derivative );
  p_gf2_and( bit_size, derivative, a );
  p_gf2_srl( bit_size, derivative, 1 );
}

uint32_t
p_gf2_eqz( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    if (x[top_word_ndx])
    {
      return 0;
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  return 1;
}

uint32_t
p_gf2_eq( int bit_size, uint32_t* x, uint32_t* y )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    if (x[top_word_ndx] != y[top_word_ndx])
    {
      return 0;
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  return 1;
}

uint32_t
p_gf2_bt( int bit_size, uint32_t* x, int bit_ndx )
{
  int word_ndx     = p_gf2_wc(bit_ndx);
  int word_bit_ndx = bit_ndx & 0x1f;
  return ( x[ word_ndx ] >> word_bit_ndx ) & 0x01;
}

void
p_gf2_inc( int bit_size, uint32_t* x )
{
  x[0] ^= 0x01;
}

int
p_gf2_deg( int bit_size, uint32_t* x )
{
  if ( p_gf2_eqz( bit_size, x ) )
  {
    return 0;
  }

  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    if (top_word)
    {
      break;
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);

  uint32_t top_word    = x[ top_word_ndx ];
  int      top_bit_ndx = 31-__builtin_clz( top_word );

  return (top_word_ndx << 5)+top_bit_ndx;
}

void
p_gf2_div( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder )
{
  int dividend_deg = p_gf2_deg( bit_size, dividend );
  int divisor_deg  = p_gf2_deg( bit_size, divisor );
  int count        = dividend_deg - divisor_deg;

  p_gf2_zero( bit_size, quotient );
  p_gf2_mov( bit_size, remainder, dividend );

  for (int i=0;i<=count;i++)
  {
    p_gf2_sll( bit_size, quotient, 1 );

    int m_deg = dividend_deg-i;
    int m_set = p_gf2_bt( bit_size, remainder, m_deg );

    if (m_set)
    {
      p_gf2_inc( bit_size, quotient );

      uint32_t* d = p_gf2_alloca( bit_size );

      p_gf2_mov( bit_size, d, divisor );
      p_gf2_sll( bit_size, d, count-i );
      p_gf2_add( bit_size, remainder, d );
    }
  }
}

void
p_gf2_mod( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* remainder )
{
  uint32_t* quotient = p_gf2_alloca( bit_size );
  p_gf2_div( bit_size, dividend, divisor, quotient, remainder );
}

void
p_gf2_gcd( int bit_size, uint32_t* a, uint32_t* b, uint32_t* gcd )
{
  if ( p_gf2_eqz( bit_size, b ) )
  {
    p_gf2_mov( bit_size, gcd, a );
    return;
  }

  uint32_t* x         = p_gf2_alloca( bit_size );
  uint32_t* y         = p_gf2_alloca( bit_size );
  uint32_t* quotient  = p_gf2_alloca( bit_size );
  uint32_t* remainder = p_gf2_alloca( bit_size );
  uint32_t* one       = p_gf2_alloca( bit_size );

  p_gf2_mov( bit_size, x, a );
  p_gf2_mov( bit_size, y, b );
  p_gf2_one( bit_size, one );

  while (1)
  {
    p_gf2_div( bit_size, x, y, quotient, remainder );

    // (remainder == 0) found GCD
    if ( p_gf2_eqz( bit_size, remainder ) )
    {
      p_gf2_mov( bit_size, gcd, y );
      return;
    }

    // (remainder == 1) relatively prime
    if ( p_gf2_eq( bit_size, remainder, one ) )
    {
      p_gf2_mov( bit_size, gcd, one );
      return;
    }

    p_gf2_mov( bit_size, x, y );
    p_gf2_mov( bit_size, y, remainder );
  }
}

void
p_gf2_sqrt( int bit_size, uint32_t* x, uint32_t* sqrt_x )
{
  uint32_t* x_bit = p_gf2_alloca( bit_size );

  p_gf2_one( bit_size, x_bit );
  p_gf2_zero( bit_size, sqrt_x );

  for (int i=0;i<bit_size;i++)
  {
    uint32_t* a   = p_gf2_alloca( bit_size );

    p_gf2_mov( bit_size, a, x_bit );
    p_gf2_and( bit_size, a, x );
    p_gf2_srl( bit_size, a, i >> 1 );
    p_gf2_add( bit_size, sqrt_x, a );
    p_gf2_sll( bit_size, x_bit, 1 );
  }
}

void
p_gf2_sff( int bit_size, uint32_t* x, int* sf_factor_count, uint32_t* sf_factors, int* sf_factors_pow, int sa )
{
  uint32_t* dx = p_gf2_alloca( bit_size );

  p_gf2_derivative( bit_size, x, dx );

  if ( p_gf2_eqz( bit_size, dx ) )
  {
    uint32_t* sqrt_x = p_gf2_alloca( bit_size );

    p_gf2_sqrt( bit_size, x, sqrt_x );
    p_gf2_sff( bit_size, sqrt_x, sf_factor_count, sf_factors, sf_factors_pow, sa+1 );
  }
  else
  {
    uint32_t* a   = p_gf2_alloca( bit_size );
    uint32_t* one = p_gf2_alloca( bit_size );

    p_gf2_gcd( bit_size, x, dx, a );
    p_gf2_one( bit_size, one );

    if ( p_gf2_eq( bit_size, a, one ) )
    {
      int count = *sf_factor_count;
      for (int i=0;i<count;i++)
      {
        if (p_gf2_eq( bit_size, x, sf_factors + ( p_gf2_wc(bit_size) * i ) ))
        {
          sf_factors_pow[ i ] += 1 << sa;
          return;
        }
      }
      p_gf2_mov( bit_size, sf_factors + ( p_gf2_wc(bit_size) * count ), x );
      sf_factors_pow[ count ] = 1 << sa;
      (*sf_factor_count)++;
      return;
    }

    uint32_t* a0        = p_gf2_alloca( bit_size );
    uint32_t* b         = p_gf2_alloca( bit_size );
    uint32_t* remainder = p_gf2_alloca( bit_size );

    p_gf2_div( bit_size, x, a, b, remainder );
    p_gf2_sff( bit_size, a, sf_factor_count, sf_factors, sf_factors_pow, sa );
    p_gf2_sff( bit_size, b, sf_factor_count, sf_factors, sf_factors_pow, sa );
  }
}

int
p_gf2_rref_pivot( int bit_size, uint32_t* M, int row_count, int deg )
{
  if ( deg < 0 )
  {
    return -1;
  }

  for (int i=0;i<row_count;i++)
  {
    uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );
    if ( p_gf2_bt( bit_size, v, deg ) )
    {
      return i;
    }
  }
  return -1;
}

void
p_gf2_rref( int bit_size, uint32_t* M, int row_count )
{
  int result_row_ndx = 0;

  int       pivot_bit_ndx = bit_size;
  int       pivot_row_ndx = 0;

  while (1)
  {
    do {
      pivot_bit_ndx--;
      pivot_row_ndx = p_gf2_rref_pivot( bit_size, M+(p_gf2_wc(bit_size)*result_row_ndx), row_count-result_row_ndx, pivot_bit_ndx );
    } while (( pivot_bit_ndx >= 0) && ( pivot_row_ndx < 0 ));

    if ( pivot_bit_ndx < 0 )
    {
      break;
    }

    pivot_row_ndx += result_row_ndx;

    if ( pivot_row_ndx > 0 )
    {
      uint32_t* pivot_row     = M + ( p_gf2_wc(bit_size) * pivot_row_ndx );
      uint32_t* result_row    = M + ( p_gf2_wc(bit_size) * result_row_ndx );
      uint32_t* t             = p_gf2_alloca( bit_size );

      if ( pivot_row_ndx != result_row_ndx )
      {
        p_gf2_mov( bit_size, t, pivot_row );
        p_gf2_mov( bit_size, pivot_row, result_row );
        p_gf2_mov( bit_size, result_row, t );
        pivot_row_ndx = result_row_ndx;
      }

      for (int j=0;j<row_count;j++)
      {
        if ( j != result_row_ndx )
        {
          uint32_t* v  = M + ( p_gf2_wc(bit_size) * j );
          if ( p_gf2_bt( bit_size, v, pivot_bit_ndx ) )
          {
            p_gf2_add( bit_size, v, result_row );
          }
        }
      }

      result_row_ndx++;
      if ( result_row_ndx == row_count )
      {
        break;
      }
    }
  }
}

void
p_gf2_berlekamp_nontrivial_bases( int bit_size, uint32_t* a, int* basis_count, uint32_t* bases )
{
  int       a_deg   = p_gf2_deg( bit_size, a );
  uint32_t* da      = p_gf2_alloca( bit_size * 2 );

  // P = x^(i*2); -- note: bit_size*2
  uint32_t* P       = p_gf2_nalloca( bit_size*2, a_deg );

  // Q = P mod a; -- note: bit_size
  uint32_t* Q       = p_gf2_nalloca( bit_size, a_deg );

  // I = identity -- note:bit_size, really a square matrix of a_deg x a_deg
  uint32_t* I       = p_gf2_nalloca( bit_size, a_deg );

  // M = Q-I
  uint32_t* M       = p_gf2_nalloca( bit_size, a_deg );

  // W = augmented M (a_deg*2)
  int       bit_size_w  = (((a_deg*2) + 31) >> 5) << 5;
  uint32_t* W           = p_gf2_nalloca( bit_size_w, a_deg );

  // Zero W: values copied from M may be smaller.
  memset( W, 0, (bit_size_w/4) * a_deg );

  p_gf2_zero( bit_size*2, da );
  p_gf2_mov( bit_size, da, a );

  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((bit_size*2)/32) * i);
    uint32_t* qv = Q + ((bit_size/32) * i);
    uint32_t* iv = I + ((bit_size/32) * i);
    uint32_t* mv = M + ((bit_size/32) * i);
    uint32_t* wv = W + ((bit_size_w/32) * i);

    uint32_t* s = p_gf2_alloca( bit_size * 2 );
    uint32_t* t = p_gf2_alloca( bit_size * 2 );

    // P[i] = x^(i*2)
    p_gf2_one( bit_size*2, pv );
    p_gf2_sll( bit_size*2, pv, i*2 );

    // Q[i] = P[i] mod a
    p_gf2_div( bit_size*2, pv, da, t, s );
    p_gf2_mov( bit_size, qv, s );

    // I[i] = 1 << i
    p_gf2_one( bit_size, iv );
    p_gf2_sll( bit_size, iv, i );

    // M[i] = Q[i] - I[i]
    p_gf2_mov( bit_size, mv, qv );    
    p_gf2_add( bit_size, mv, iv );

    // W[i] = (M[i] << deg(a)) | (1 << i)
    p_gf2_mov( bit_size, wv, mv );
    p_gf2_sll( bit_size_w, wv, a_deg );
    p_gf2_add( bit_size, wv, iv );
  }

  p_gf2_rref( bit_size_w, W, a_deg );

  uint32_t* one = p_gf2_alloca( bit_size_w );
  p_gf2_one( bit_size_w, one );

  {
    int basis_ndx = 0;
    for (int i=0;i<a_deg;i++)
    {
      uint32_t* v   = W + ( p_gf2_wc(bit_size_w) * i );
      uint32_t* zv  = p_gf2_alloca( bit_size_w );
  
      p_gf2_mov( bit_size_w, zv, v );
      p_gf2_srl( bit_size_w, zv, a_deg+1 );
  
      if ( p_gf2_eqz( bit_size_w, zv ) )
      {
        if ( !p_gf2_eq( bit_size, v, one ) )
        {
          uint32_t* bv = bases + (basis_ndx * (bit_size/32));

          // this move is tricky since bv and v are different sizes (v can be smaller!)
          p_gf2_zero( bit_size, bv );
          p_gf2_mov( (bit_size<bit_size_w)?bit_size:bit_size_w, bv, v );

          basis_ndx++;
        }
      }
    }
    *basis_count = basis_ndx;
  }
}

void
p_gf2_berlekamp_nontrivial_factors( int bit_size, uint32_t* a, int* known_factor_count, uint32_t* known_factors )
{
  int       a_deg       = p_gf2_deg( bit_size, a );
  uint32_t* bases       = p_gf2_nalloca( bit_size, a_deg );
  uint32_t* one         = p_gf2_alloca( bit_size );
  int       basis_count;

  p_gf2_one( bit_size, one );
  p_gf2_berlekamp_nontrivial_bases( bit_size, a, &basis_count, bases );

  if ( basis_count == 0 )
  {
    *known_factor_count = 0;
    return;
  }

  *known_factor_count = 1;
  p_gf2_mov( bit_size, known_factors, a );

  for (int i=0;i<basis_count;i++)
  {
    uint32_t* bv   = bases + (i * (bit_size/32));
    uint32_t* s0   = p_gf2_alloca( bit_size );
    uint32_t* s1   = p_gf2_alloca( bit_size );
    uint32_t* gcd0 = p_gf2_alloca( bit_size );
    uint32_t* gcd1 = p_gf2_alloca( bit_size );
    uint32_t* f0   = p_gf2_alloca( bit_size );
    uint32_t* f1   = p_gf2_alloca( bit_size );
    uint32_t* r0   = p_gf2_alloca( bit_size );
    uint32_t* r1   = p_gf2_alloca( bit_size );

    p_gf2_mov( bit_size, s0, bv );
    p_gf2_mov( bit_size, s1, bv );
    p_gf2_inc( bit_size, s1 );

    p_gf2_gcd( bit_size, a, s0, gcd0 );
    p_gf2_gcd( bit_size, a, s1, gcd1 );

    for (int j=0;j<(*known_factor_count);j++)
    {
      uint32_t* kf = known_factors + (j*(bit_size/32));

      p_gf2_div( bit_size, kf, gcd0, f0, r0 );
      if ( p_gf2_eqz( bit_size, r0 ) ) 
      {
        if ( !p_gf2_eq( bit_size, f0, one ) )
        {
          if ( !p_gf2_eq( bit_size, gcd0, one ) ) // relatively prime
          {
            // replace known factor
            p_gf2_mov( bit_size, kf, gcd0 );
    
            // add new known factor
            p_gf2_mov( bit_size, known_factors + ((*known_factor_count)*(bit_size/32)), f0 );
            (*known_factor_count)++;
    
            break;
          }
        }
      }
    }

    for (int j=0;j<(*known_factor_count);j++)
    {
      uint32_t* kf = known_factors + (j*(bit_size/32));

      p_gf2_div( bit_size, kf, gcd1, f1, r1 );
      if ( p_gf2_eqz( bit_size, r1 ) ) 
      {
        if ( !p_gf2_eq( bit_size, f1, one ) )
        {
          if ( !p_gf2_eq( bit_size, gcd1, one ) ) // relatively prime
          {
            // replace known factor
            p_gf2_mov( bit_size, kf, gcd1 );
  
            // add new known factor
            p_gf2_mov( bit_size, known_factors + ((*known_factor_count)*(bit_size/32)), f1 );
            (*known_factor_count)++;
    
            break;
          }
        }
      }
    }
  }

  // all the potential factors were relatively prime
  if ( (*known_factor_count) == 1 )
  {
    *known_factor_count = 0;
    return;
  }
}

void
p_gf2_berlekamp_irreducible_factors( int bit_size, uint32_t* a, int* irreducible_factor_count, uint32_t* irreducible_factors )
{
  int a_deg = p_gf2_deg( bit_size, a );

  (*irreducible_factor_count) = 1;
  p_gf2_mov( bit_size, irreducible_factors, a );

  int i=0;
  while (i<(*irreducible_factor_count))
  {
    uint32_t* next_factor   = irreducible_factors + (i * (bit_size/32));
    uint32_t* known_factors = irreducible_factors + ((*irreducible_factor_count) * (bit_size/32));
    int       known_factor_count;

    p_gf2_berlekamp_nontrivial_factors( bit_size, next_factor, &known_factor_count, known_factors );

    // remove next_factor if it added new known_factors
    if ( known_factor_count > 1 )
    {
      int  after_factors_count = ((*irreducible_factor_count)+known_factor_count) - (i+1);
      for (int j=0;j<after_factors_count;j++)
      {
        p_gf2_mov( bit_size, next_factor + (j*(bit_size/32)), next_factor + ((j+1)*(bit_size/32)) );
      }

      (*irreducible_factor_count) += (known_factor_count-1);
    }
    else
    {
      i++;
    }
  }
}

