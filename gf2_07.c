#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"

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
  printf("b");
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
p_gf2_print_bin_w( int bit_size, uint32_t* x, int width )
{
  for (int i=width-1;i>=0;i--)
  {
    printf("%c",p_gf2_bt( bit_size, x, i )?'1':'0');
  }
}

void
p_gf2_print_bin_w_aug( int bit_size, uint32_t* x, int width )
{
  for (int i=width-1;i>=0;i--)
  {
    printf("%c",p_gf2_bt( bit_size, x, i )?'1':'0');
    if ( i == (width/2) ) {
      printf(" ");
    }
  }
}

void 
p_gf2_print_hex( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  printf("0x");
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
  printf( "%s = ", name);
  p_gf2_print_bin( bit_size, x );
  printf("\n");

  printf( "%s = ", name);
  p_gf2_print_hex( bit_size, x );
  printf("\n");

  printf( "%s = ", name);
  p_gf2_print( bit_size, x );
  printf("\n");
}

void 
p_gf2_print_hex_skiplz( int bit_size, uint32_t* x )
{
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  int found_word   = 0;
  printf("0x");
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    if (top_word)
    { 
      printf( "%x",top_word );
      found_word = 1;
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!found_word)
  {
    printf( "0" );
  }
}

void 
p_gf2_print_bin_skiplz( int bit_size, uint32_t* x )
{
  int word_count    = p_gf2_wc(bit_size);
  int top_word_ndx  = word_count-1;
  int found_top_bit = 0;
  printf("b");
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
p_gf2_rref_html( int bit_size, uint32_t* M, int row_count )
{
  html_print("<h2>Transform M into row reduced echelon form (rref)</h2>");
  html_println("<i>Gauss-Jordan Elimination</i>");
  html_br();

  int result_row_ndx = 0;

  int       pivot_bit_ndx = bit_size-1;
  int       pivot_row_ndx = 0;

  html_println("Mark first row (*) as next result row");
  html_indent_push();
  html_code_begin();
  html_print("M = ");
  for (int i=0;i<row_count;i++)
  {
    uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );

    html_print( "%s",(result_row_ndx == i)?"* ":"  ");

    if (i>0)
    {
      html_print("    ");
    }
     
    for (int j=bit_size-1;j>=0;j--)
    {
      html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');
    }
    html_print("\n");
  }
  html_indent_pop();
  html_code_end();

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

      html_println("Highlight next pivot row and column");
      html_println("<i>Find first row with leftmost bit set starting from next result row.</i>");
      html_indent_push();
      html_code_begin();
      html_print("M = ");
      for (int i=0;i<row_count;i++)
      {
        uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );

    
        if (i>0)
        {
          html_print("    ");
        }
        html_print( "%s",(result_row_ndx == i)?"* ":"  ");
         
        if (i==pivot_row_ndx)
        {
          html_print("<span style=\"color:red\">");
        }
        for (int j=bit_size-1;j>=0;j--)
        {
          if (j==pivot_bit_ndx)
          {
            html_print("<span style=\"color:red\">");
          }

          html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');

          if (j==pivot_bit_ndx)
          {
            html_print("</span>");;
          }
        }
        if (i==pivot_row_ndx)
        {
          html_print("</span>");;
        }
        html_print("\n");
      }
      html_indent_pop();
      html_code_end();
      html_br();

      if ( pivot_row_ndx != result_row_ndx )
      {
        int prev_pivot_row_ndx = pivot_row_ndx;

        p_gf2_mov( bit_size, t, pivot_row );
        p_gf2_mov( bit_size, pivot_row, result_row );
        p_gf2_mov( bit_size, result_row, t );
        pivot_row_ndx = result_row_ndx;
        result_row_ndx++;

        html_println("Swap pivot row and next result row; Increment next result row.");
        html_indent_push();
        html_code_begin();
        html_print("M = ");
        for (int i=0;i<row_count;i++)
        {
          uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );
      
          if (i>0)
          {
            html_print("    ");
          }
          html_print( "%s",(result_row_ndx == i)?"* ":"  ");
           
          if ( (i==pivot_row_ndx) || (i==prev_pivot_row_ndx))
          {
            html_print("<span style=\"color:red\">");
          }
          for (int j=bit_size-1;j>=0;j--)
          {
            if (j==pivot_bit_ndx)
            {
              html_print("<span style=\"color:red\">");
            }
            html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');
            if (j==pivot_bit_ndx)
            {
              html_print("</span>");;
            }
          }
          if ( (i==pivot_row_ndx) || (i==prev_pivot_row_ndx))
          {
            html_print("</span>");;
          }
          html_print("\n");
        }
        html_indent_pop();
        html_code_end();
        html_br();
      }
      else
      {
        result_row_ndx++;

        html_println("Increment next result row.");
        html_indent_push();
        html_code_begin();
        html_print("M = ");
        for (int i=0;i<row_count;i++)
        {
          uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );
      
          if (i>0)
          {
            html_print("    ");
          }
          html_print( "%s",(result_row_ndx == i)?"* ":"  ");
           
          if (i==pivot_row_ndx)
          {
            html_print("<span style=\"color:red\">");
          }
          for (int j=bit_size-1;j>=0;j--)
          {
            html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');
          }
          if (i==pivot_row_ndx)
          {
            html_print("</span>");;
          }
          html_print("\n");
        }
        html_indent_pop();
        html_code_end();
        html_br();
      }

      int row_bit_size = (row_count+31)&(~0x1f);
      uint32_t* row_add_marker = p_gf2_alloca( row_bit_size );
      p_gf2_zero( row_bit_size, row_add_marker );

      for (int j=0;j<row_count;j++)
      {
        if ( j != pivot_row_ndx )
        {
          uint32_t* v  = M + ( p_gf2_wc(bit_size) * j );
          if ( p_gf2_bt( bit_size, v, pivot_bit_ndx ) )
          {
            uint32_t* row_bit = p_gf2_alloca( row_bit_size );
            p_gf2_one( row_bit_size, row_bit );
            p_gf2_sll( row_bit_size, row_bit, j );
            p_gf2_add( row_bit_size, row_add_marker, row_bit );

            p_gf2_add( bit_size, v, result_row );
          }
        }
      }

      html_println("Clear pivot column.");
      html_println("<i>Add pivot row to all other rows with bit set in pivot column</i>");
      html_indent_push();
      html_code_begin();
      html_print("M = ");
      for (int i=0;i<row_count;i++)
      {
        uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );
    
        if (i>0)
        {
          html_print("    ");
        }
        html_print( "%s",(result_row_ndx == i)?"* ":"  ");
         
        if (i==pivot_row_ndx)
        {
          html_print("<span style=\"color:red\">");
        }
        for (int j=bit_size-1;j>=0;j--)
        {
          if (j==pivot_bit_ndx)
          {
            html_print("<span style=\"color:red\">");
          }
          if (( j < pivot_bit_ndx) && p_gf2_bt( row_bit_size, row_add_marker, i))
          {
            html_print("<span style=\"color:red\">");
          }
          html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');
          if (j==pivot_bit_ndx)
          {
            html_print("</span>");;
          }
          if (( j < pivot_bit_ndx) && p_gf2_bt( row_bit_size, row_add_marker, i))
          {
            html_print("</span>");;
          }
        }
        if (i==pivot_row_ndx)
        {
          html_print("</span>");;
        }
        html_print("\n");
      }
      html_indent_pop();
      html_code_end();
      html_br();

      if ( result_row_ndx == row_count )
      {
        break;
      }
    }
  }

  html_println("Done.");
  html_indent_push();
  html_code_begin();
  html_print("M = ");
  for (int i=0;i<row_count;i++)
  {
    uint32_t* v  = M + ( p_gf2_wc(bit_size) * i );

    if (i>0)
    {
      html_print("    ");
    }
     
    for (int j=bit_size-1;j>=0;j--)
    {
      html_print("%c",p_gf2_bt(bit_size,v,j)?'1':'0');
    }
    html_print("\n");
  }
  html_indent_pop();
  html_code_end();
  html_br();
}
