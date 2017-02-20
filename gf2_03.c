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
p_gf2_print( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  int has_prev     = 0;
  
  do
  {
    uint32_t top_word = x[ top_word_ndx ];

    for (int i=31;i>=0;i--)
    {
      if ( top_word & (1<<i) )
      {
        if (( top_word_ndx == 0 ) && (i==0))
        {
          printf( "%s1",has_prev?" + ":"");
        }
        else if (( top_word_ndx == 0 ) && (i==1))
        {
          printf( "%sx",has_prev?" + ":"");
        }
        else
        {
          printf( "%sx^%d",has_prev?" + ":"",(top_word_ndx*32)+i);
        }
        has_prev = 1;
      }
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!has_prev)
  {
    printf("0");
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
p_gf2_div_print( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder )
{
  printf("==== p_gf2_div_print ====\n");
  printf("c = a/b\n");
  printf("\n");

  p_gf2_print_p("a",bit_size,dividend);
  printf("\n");

  p_gf2_print_p("b",bit_size,divisor);
  printf("\n");

  printf("  ");
  p_gf2_print_bin_skiplz(bit_size,dividend);
  printf(" | ");
  p_gf2_print_bin_skiplz(bit_size,divisor);
  printf("\n");

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

      printf("- ");
      for (int j=0;j<i;j++)
      {
        printf(" ");
      }
      p_gf2_print_bin_skiplz(bit_size,divisor);
      for (int j=0;j<(dividend_deg-i-divisor_deg);j++)
      {
        printf(" ");
      }
      printf(" | 1");
      printf("\n");
      printf("  ");
      for (int j=0;j<dividend_deg+1;j++)
      {
        printf("-");
      }
      int rem_deg = p_gf2_deg( bit_size, remainder );
      printf(" |\n");
      printf("  ");
      for (int j=0;j<(dividend_deg-rem_deg);j++)
      {
        printf(" ");
      }
      p_gf2_print_bin_skiplz(bit_size,remainder);
      printf(" |\n");
    }
    else
    {
      printf("- ");
      for (int j=0;j<i;j++)
      {
        printf(" ");
      }
      for (int j=0;j<divisor_deg+1;j++)
      {
        printf("0");
      }
      for (int j=0;j<(dividend_deg-i-divisor_deg);j++)
      {
        printf(" ");
      }
      printf(" | 0");
      printf("\n");
    }
  }
  printf("\n");

  p_gf2_print_p("a/b",bit_size,quotient);
  printf("\n");

  p_gf2_print_p("a%b",bit_size,remainder);
  printf("\n");

  printf("\n");
}

void
p_gf2_print_bin( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    for (int i=31;i>=0;i--)
    {
      printf("%c",top_word&(1<<i)?'1':'0');
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void 
p_gf2_print_hex( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    printf("%08x",top_word);
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void 
p_gf2_print_p( const char* name, int bit_size, uint32_t* x )
{
  printf( "%s = b", name);
  p_gf2_print_bin( bit_size, x );
  printf("\n");

  printf( "%s = 0x", name);
  p_gf2_print_hex( bit_size, x );
  printf("\n");

  printf( "%s = ", name);
  p_gf2_print( bit_size, x );
  printf("\n");
}

void 
p_gf2_print_bin_skiplz( int bit_size, uint32_t* x )
{
  int word_count    = p_gf2_wc(bit_size);
  int top_word_ndx  = word_count-1;
  int found_top_bit = 0;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    for (int i=31;i>=0;i--)
    {
      if (found_top_bit)
      {
        printf("%c",top_word&(1<<i)?'1':'0');
      }
      else
      {
        if ( top_word&(1<<i) )
        {
          found_top_bit = 1;
          printf( "1" );
        }
      }
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!found_top_bit)
  {
    printf( "0" );
  }
}
