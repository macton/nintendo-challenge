#include <stdint.h>
#include <string.h>
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

uint32_t
p_gf2_bt( int bit_size, uint32_t* x, int bit_ndx )
{
  int word_ndx     = p_gf2_wc(bit_ndx);
  int word_bit_ndx = bit_ndx & 0x1f;
  return ( x[ word_ndx ] >> word_bit_ndx ) & 0x01;
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

