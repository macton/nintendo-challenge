#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"

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
      printf("-");
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
p_gf2_rref_html( int bit_size, uint32_t* M, int row_count )
{
  html_print("<h2>Transform M into row reduced echelon form (rref)</h2>");
  html_println("<i>Gauss-Jordan Elimination</i>");
  html_br();

  int result_row_ndx = 0;

  int       pivot_bit_ndx = bit_size;
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

void
p_gf2_berlekamp_nontrivial_bases_html( int bit_size, uint32_t* a, int* basis_count, uint32_t* bases )
{
  html_print_p( "a", bit_size, a );

  html_print("<h2>Berlekamp: Find non-trivial bases</h2>");

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
  uint32_t* W          = p_gf2_nalloca( bit_size_w, a_deg );

  memset( W, 0, (bit_size_w/4) * a_deg );

  p_gf2_zero( bit_size*2, da );
  p_gf2_mov( bit_size, da, a );

  html_print_p( "a", bit_size, a );

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


  html_code_begin();
  html_print("for i=0;i&lt;degree(a);i++)");
  html_br();
  html_print("  Q[i] = x<sup>(i*2)</sup> mod a");
  html_br();
  html_code_end();

  html_code_begin();
  html_print("Q = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* pv = P + (((bit_size*2)/32) * i);
    uint32_t* qv = Q + ((bit_size/32) * i);

    if (i > 0)
    {
      html_print("    ");
    }
    html_print_bin_w( bit_size*2, pv, a_deg*2 );
    html_print(" mod a = ");
    html_print_bin_w( bit_size, qv, a_deg );
    html_br();
  }
  html_code_end();

  html_code_begin();
  html_print("I = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* iv = I + ((bit_size/32) * i);

    if (i > 0)
    {
      html_print("    ");
    }
    html_print_bin_w( bit_size, iv, a_deg );
    html_br();
  }
  html_code_end();

  html_code_begin();
  html_println("M = Q-I");
  html_print("M = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* mv = M + ((bit_size/32) * i);

    if (i > 0)
    {
      html_print("    ");
    }
    html_print_bin_w( bit_size, mv, a_deg );
    html_br();
  }
  html_code_end();

  html_println("Augment M with identity");
  html_code_begin();
  html_print("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* wv = W + ((bit_size_w/32) * i);

    if (i > 0)
    {
      html_print("    ");
    }
    html_print_bin_w_aug( bit_size_w, wv, a_deg*2 );
    html_br();
  }
  html_code_end();

  html_page_push("p_gf2_rref( W )");
  p_gf2_rref_html( bit_size_w, W, a_deg );
  html_page_pop();
  html_br();
  html_br();

  html_code_begin();
  html_print("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* wv = W + ((bit_size_w/32) * i);

    if (i > 0)
    {
      html_print("    ");
    }
    html_print_bin_w_aug( bit_size_w, wv, a_deg*2 );
    html_br();
  }
  html_code_end();

  html_println("Highlight null basis vectors (where left side of row is zero)");
  html_code_begin();
  html_print("W = ");
  for (int i=0;i<a_deg;i++)
  {
    uint32_t* v  = W + ( p_gf2_wc(bit_size_w) * i );
    int is_null_basis = 1;

    if (i>0)
    {
      html_print("    ");
    }

    for (int j=(a_deg+1)+(a_deg-1);j>=(a_deg+1);j--)
    {
      is_null_basis &= (!p_gf2_bt(bit_size_w,v,j));
    }
     
    if (is_null_basis)
    {
      html_print("<span style=\"color:red\">");
    } 
    for (int j=(a_deg+1)+(a_deg-1);j>=(a_deg+1);j--)
    {
      html_print("%c",p_gf2_bt(bit_size_w,v,j)?'1':'0');
    }

    html_print(" ");
    for (int j=(a_deg-1);j>=0;j--)
    {
      html_print("%c",p_gf2_bt(bit_size_w,v,j)?'1':'0');
    }
    if (is_null_basis)
    {
      html_print("</span>");
    } 
    html_print("\n");
  }
  html_code_end();

  html_println("Non-trivial null basis vectors");
  html_br();

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
        if ( !p_gf2_eq( bit_size_w, v, one ) )
        {
          uint32_t* bv = bases + (basis_ndx * (bit_size/32));

          // this move is tricky since bv and v are different sizes (v can be smaller!)
          p_gf2_zero( bit_size, bv );
          p_gf2_mov( (bit_size<bit_size_w)?bit_size:bit_size_w, bv, v );

          char nvname[64];
          sprintf(nvname,"nv<sub>%d</sub>",basis_ndx);
          html_print_p(nvname,bit_size,bv);
          html_br();

          basis_ndx++;
        }
      }
    }
    *basis_count = basis_ndx;
  }

  if ((*basis_count) == 0)
  {
    html_println("NONE");
  }
}

void
p_gf2_berlekamp_nontrivial_factors_html( int bit_size, uint32_t* a, int* known_factor_count, uint32_t* known_factors )
{
  int       a_deg       = p_gf2_deg( bit_size, a );
  uint32_t* bases       = p_gf2_nalloca( bit_size, a_deg );
  uint32_t* one         = p_gf2_alloca( bit_size );
  int       basis_count;

  p_gf2_one( bit_size, one );

  html_print_p( "a", bit_size, a );
  html_br();

  html_page_push( "p_gf2_berlekamp_nontrivial_bases(a)" );
  p_gf2_berlekamp_nontrivial_bases_html( bit_size, a, &basis_count, bases );
  html_page_pop();
  html_br();

  if ( basis_count == 0 )
  {
    *known_factor_count = 0;
    html_br();
    html_print("= IRREDUCIBLE");
    html_br();
    return;
  }

  html_print("<h2>find factors</h2>");

  for (int i=0;i<basis_count;i++)
  {
    char bvname[64];
    sprintf(bvname,"bv[%d]",i);
    html_print_p( bvname, bit_size, bases + (i * (bit_size/32))  );
    html_br();
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

    html_page_push("gcd( a, bv[%d]+0 )",i);
    p_gf2_gcd_html( bit_size, a, s0, gcd0 );
    html_page_pop();
    html_br();

    html_page_push("gcd( a, bv[%d]+1 )",i);
    p_gf2_gcd_html( bit_size, a, s1, gcd1 );
    html_page_pop();
    html_br();

    char gcdname[64];

    sprintf(gcdname,"gcd( a, bv[%d]+0 )",i);
    html_print_p( gcdname, bit_size, gcd0 );
    html_br();

    sprintf(gcdname,"gcd( a, bv[%d]+1 )",i);
    html_print_p( gcdname, bit_size, gcd1 );
    html_br();

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
    
            html_print_p( "* factor[0] ", bit_size, gcd0 );
            html_br();
            html_print_p( "* factor[1] ", bit_size, f0 );
            html_br();
  
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
    
            html_print_p( "* factor[0] ", bit_size, gcd1 );
            html_br();
            html_print_p( "* factor[1] ", bit_size, f1 );
            html_br();
  
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
    html_br();
    html_print("= IRREDUCIBLE");
    html_br();
    return;
  }

  html_print("<h2>non-trivial factors of a</h2>");

  for (int j=0;j<(*known_factor_count);j++)
  {
    uint32_t* kf = known_factors + (j*(bit_size/32));

    char kfname[64];
    sprintf(kfname,"kf<sub>%d</sub>",j);

    html_print_p( kfname, bit_size, kf );
    html_br();
  }
}

void
p_gf2_berlekamp_irreducible_factors_html( int bit_size, uint32_t* a, int* irreducible_factor_count, uint32_t* irreducible_factors )
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

    char fname[64];
    sprintf(fname,"f<sub>%d</sub>",i);

    html_print_p( fname, bit_size, next_factor );
    html_br();

    html_page_push( "p_gf2_berlekamp_nontrivial_factors_html( %s )", fname );

    p_gf2_berlekamp_nontrivial_factors_html( bit_size, next_factor, &known_factor_count, known_factors );

    html_page_pop();
    html_br();
    html_br();

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

  html_print("<h2>irreducible factors</h2>");

  i = 0;
  while (i<(*irreducible_factor_count))
  {
    uint32_t* next_factor = irreducible_factors + (i * (bit_size/32));

    char fname[64];
    sprintf(fname,"f<sub>%d</sub>",i);

    html_print_p( fname, bit_size, next_factor );
    html_br();

    i++;
  }
}

void
p_gf2_sff_html( int bit_size, uint32_t* x, int* sf_factor_count, uint32_t* sf_factors, int* sf_factors_pow, int sa )
{
  uint32_t* dx = p_gf2_alloca( bit_size );

  int start_factor_ndx = (*sf_factor_count);
  int start_factor_pow = sf_factors_pow[ start_factor_ndx ];

  static int depth = -1;
  depth++;

  html_print("<h2>gf(2) Square-free factorization (sff)</h2>");
  html_println("<i>Find and remove all squared factors from f.</i>");
  html_br();

  html_indent_push();
  html_print_p( "f", bit_size, x );
  html_indent_pop();
  html_br();

  html_page_push("p_gf2_derivative( f )");

  p_gf2_derivative_html( bit_size, x, dx );

  html_page_pop();
  html_br();
  html_br();

  html_indent_push();
  html_print_p( "f'", bit_size, dx );
  html_indent_pop();
  html_br();

  if ( p_gf2_eqz( bit_size, dx ) )
  {
    html_print("Since f' equals zero, ");

    uint32_t* sqrt_x = p_gf2_alloca( bit_size );

    html_page_push("p_gf2_sqrt( f )");

    p_gf2_sqrt_html( bit_size, x, sqrt_x );

    html_page_pop();
    html_br();
    html_br();
    html_indent_push();
    html_print_p( "sqrt(f)", bit_size, sqrt_x );
    html_indent_pop();
    html_br();

    html_println("In case sqrt(f) also has square factors...");
    html_indent_push();
    html_br();
    html_page_push("Find sff( sqrt(f) )");

    p_gf2_sff_html( bit_size, sqrt_x, sf_factor_count, sf_factors, sf_factors_pow, sa+1 );

    html_page_pop();
    html_indent_pop();
  }
  else
  {
    html_print("Since f' does not equal zero, ");
    html_page_push("find g=gcd(f,f')");

    uint32_t* a   = p_gf2_alloca( bit_size );
    uint32_t* one = p_gf2_alloca( bit_size );

    p_gf2_gcd_html( bit_size, x, dx, a );
    p_gf2_one( bit_size, one );

    html_page_pop();
    html_br();
    html_br();
    html_indent_push();
    html_print_p( "g", bit_size, a );
    html_indent_pop();

    if ( p_gf2_eq( bit_size, a, one ) )
    {
      html_br();
      html_println("Since g equals one, f has no square factors.");

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
    }
    else
    {
      html_br();
      html_println("Since g does not equal one, g is a factor of f.");
      html_indent_push();
      html_br();
      html_println("f = gh");
      html_println("h = f/g");
      html_indent_pop();

      uint32_t* b         = p_gf2_alloca( bit_size );
      uint32_t* remainder = p_gf2_alloca( bit_size );

      html_br();
      html_page_push("Find h=f/g");

      p_gf2_div_html( bit_size, x, a, b, remainder );

      html_page_pop();
      html_br();
      html_br();
      html_indent_push();
      html_print_p( "h", bit_size, b );
      html_indent_pop();
      html_br();

      html_println("In case either g or h are not square-free...");
      html_br();
      html_indent_push();
      html_page_push("Find sff(g)");

      p_gf2_sff_html( bit_size, a, sf_factor_count, sf_factors, sf_factors_pow, sa );

      html_page_pop();
      html_br();

      html_page_push("Find sff(h)");

      p_gf2_sff_html( bit_size, b, sf_factor_count, sf_factors, sf_factors_pow, sa );

      html_page_pop();
      html_br();
      html_indent_pop();
    }
  }

  {
    html_br();
    html_println("Result (f factored into square-free factors)");
    html_indent_push();
    html_println("<tt>");
    html_print("f = "); 
    for (int i=start_factor_ndx;i<*sf_factor_count;i++)
    {
      html_print("(b"); 
      html_print_p_bin( bit_size, sf_factors + ( p_gf2_wc(bit_size) * i ) );
      html_print(")");
      int pow = sf_factors_pow[i] - ((i==start_factor_ndx)?start_factor_pow:0);
      if (pow > 1)
      {
        html_print("<sup>%d</sup>",pow);
      }
    }
    html_br();
    html_print("f = "); 
    for (int i=start_factor_ndx;i<*sf_factor_count;i++)
    {
      html_print("(0x"); 
      html_print_p_hex( bit_size, sf_factors + ( p_gf2_wc(bit_size) * i ) );
      html_print(")");
      int pow = sf_factors_pow[i] - ((i==start_factor_ndx)?start_factor_pow:0);
      if (pow > 1)
      {
        html_print("<sup>%d</sup>",pow);
      }
    }
    html_br();
    html_print("f = "); 
    for (int i=start_factor_ndx;i<*sf_factor_count;i++)
    {
      html_print("("); 
      html_print_p_poly( bit_size, sf_factors + ( p_gf2_wc(bit_size) * i ) );
      html_print(")");
      int pow = sf_factors_pow[i] - ((i==start_factor_ndx)?start_factor_pow:0);
      if (pow > 1)
      {
        html_print("<sup>%d</sup>",pow);
      }
    }
    html_br();
    html_println("</tt>");
    html_indent_pop();

    html_br();
    html_hr();
    html_references_push();
    html_reference( "https://en.wikipedia.org/wiki/Factorization_of_polynomials_over_finite_fields#Square-free_factorization" );
    html_reference( "http://blog.fkraiem.org/2013/11/30/polynomial-factorisation-over-finite-fields-part-1-squarefree-factorisation/" );
    html_references_pop();
  }
  depth--;
}

void
p_gf2_derivative_html( int bit_size, uint32_t* a, uint32_t* derivative )
{
  html_print("<h2>gf(2) Calculating the derivative</h2>");
  html_println("Since all even powers will create even coefficients (which in gf(2) are zero), they can be discounted.");
  html_println("Finding the derivative is simply masking the odd powers and shifting right one place.");
  html_br();

  html_print_p( "f", bit_size, a );
  html_br();

  p_gf2_oddmask( bit_size, derivative );

  html_println("odd = Mask for odd bits");
  html_print_p( "odd", bit_size, derivative );
  html_br();

  p_gf2_and( bit_size, derivative, a );

  html_println("a = odd & f");
  html_print_p( "a", bit_size, derivative );
  html_br();

  p_gf2_srl( bit_size, derivative, 1 );

  html_println("f' = a >> 1");
  html_print_p( "f'", bit_size, derivative );
  html_br();
}

void
p_gf2_sqrt_html( int bit_size, uint32_t* x, uint32_t* sqrt_x )
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

  html_print("<h2>gf(2) sqrt(f)</h2>");
  html_print_p("f",bit_size,x);
  html_br();
  html_println("Separate each bit of f. f=");
  html_print("<tt><pre>");
  p_gf2_one( bit_size, x_bit );
  for (int i=0;i<bit_size;i++)
  {
    uint32_t* a   = p_gf2_alloca( bit_size );

    p_gf2_mov( bit_size, a, x_bit );
    p_gf2_and( bit_size, a, x );
    p_gf2_sll( bit_size, x_bit, 1 );

    if (!p_gf2_eqz(bit_size,a))
    {
      html_print_p_bin( bit_size, a );
      html_print(" ");
      html_print_p_hex( bit_size, a );
      html_print(" ");
      html_print_p_poly( bit_size, a );
      html_br();
    }
  }
  html_print("</pre></tt>");

  html_br();
  html_println("Shift each bit right by half the degree. sqrt(f)=");
  html_print("<tt><pre>");
  p_gf2_one( bit_size, x_bit );
  for (int i=0;i<bit_size;i++)
  {
    uint32_t* a   = p_gf2_alloca( bit_size );

    p_gf2_mov( bit_size, a, x_bit );
    p_gf2_and( bit_size, a, x );
    p_gf2_srl( bit_size, a, i >> 1 );
    p_gf2_sll( bit_size, x_bit, 1 );

    if (!p_gf2_eqz(bit_size,a))
    {
      html_print_p_bin( bit_size, a );
      html_print(" ");
      html_print_p_hex( bit_size, a );
      html_print(" ");
      html_print_p_poly( bit_size, a );
      html_br();
    }
  }
  html_print("</pre></tt>");

  html_br();
  html_print_p("sqrt(f)",bit_size,sqrt_x);
  html_br();
}

void
p_gf2_gcd_html( int bit_size, uint32_t* a, uint32_t* b, uint32_t* gcd )
{
  int depth = 0;

  html_print("<h2>gf(2) Calculating the gcd (greatest common divisor)</h2>");
  html_println("<i>Euclid's Algorithm</i>");
  html_br();

  html_println( "c = gcd(a<sub>0</sub>,b<sub>0</sub>)");
  html_br();

  html_indent_push();
  html_print_p( "a<sub>0</sub>", bit_size, a );
  html_br();

  html_print_p( "b<sub>0</sub>", bit_size, b );
  html_br();
  html_indent_pop();

  if ( p_gf2_eqz( bit_size, b ) )
  {
    html_println( "N/A - b is zero" );
    p_gf2_mov( bit_size, gcd, a );
    return;
  }

  uint32_t* x         = p_gf2_alloca( bit_size );
  uint32_t* y         = p_gf2_alloca( bit_size );
  uint32_t* remainder = p_gf2_alloca( bit_size );
  uint32_t* one       = p_gf2_alloca( bit_size );

  p_gf2_mov( bit_size, x, a );
  p_gf2_mov( bit_size, y, b );
  p_gf2_one( bit_size, one );

  int step = 0;

  char aname[32];
  char bname[32];
  char rname[32];
  sprintf(aname,"a<sub>%d</sub>", depth);
  sprintf(bname,"b<sub>%d</sub>", depth);
  sprintf(rname,"r<sub>%d</sub>", depth);

  while (1)
  {
    html_page_push( "Find r<sub>%d</sub> = a<sub>%d</sub> % b<sub>%d</sub>", depth, depth, depth );

    p_gf2_mod_html( bit_size, x, y, remainder );

    html_page_pop();
    html_br();
    html_br();

    html_indent_push();
    html_print_p( rname, bit_size, remainder );
    html_indent_pop();
    html_br();

    if ( p_gf2_eqz( bit_size, remainder ) )
    {
      html_indent_push();
      html_println("%s is zero, so c=gcd(a,b)=b<sub>%d</sub>", rname, depth);
      html_indent_pop();
      html_br();
      html_print_p( "c", bit_size, y );

      p_gf2_mov( bit_size, gcd, y );
      break;
    }

    if ( p_gf2_eq( bit_size, remainder, one ) )
    {
      html_indent_push();
      html_println("%s is one, so gcd(a,b) is one (i.e. coprime/relatively prime)", rname);
      html_indent_pop();

      p_gf2_mov( bit_size, gcd, one );
      break;
    }

    p_gf2_mov( bit_size, x, y );
    p_gf2_mov( bit_size, y, remainder );

    html_indent_push();
    html_println("%s is not zero nor one, so continue...", rname);
    html_br();
    html_println( "a<sub>%d</sub>  = b<sub>%d</sub>",depth+1,depth);

    sprintf(aname,"a<sub>%d</sub>", depth+1);
    sprintf(bname,"b<sub>%d</sub>", depth+1);
    sprintf(rname,"r<sub>%d</sub>", depth+1);

    html_print_p( aname, bit_size, x );
    html_br();
    html_println( "%s = r<sub>%d</sub>",bname,depth);
    html_print_p( bname, bit_size, y );
    html_br();
    html_indent_pop();

    depth++;
  }

  html_br();
  html_hr();
  html_references_push();
  html_reference("https://en.wikipedia.org/wiki/Euclidean_algorithm");
  html_reference("https://crypto.stanford.edu/pbc/notes/numbertheory/euclid.html");
  html_reference("http://math.stackexchange.com/questions/360442/gcd-of-polynomials-over-gfp");
  html_references_pop();
}

void
p_gf2_div_html( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder )
{
  html_print("<h2>gf(2) Polynomial long division</h2>");
  html_println("c = a/b");
  html_br();
  html_print_p("a",bit_size,dividend);
  html_br();
  html_print_p("b",bit_size,divisor);
  html_br();

  html_print("<tt><pre>");
  html_print("  ");
  html_print_p_bin_skiplz(bit_size,dividend);
  html_print(" | ");
  html_print_p_bin_skiplz(bit_size,divisor);
  html_print("\n");

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

      html_print("- ");
      for (int j=0;j<i;j++)
      {
        html_print(" ");
      }
      html_print_p_bin_skiplz(bit_size,divisor);
      for (int j=0;j<(dividend_deg-i-divisor_deg);j++)
      {
        html_print(" ");
      }
      html_print(" | 1");
      html_print("\n");
      html_print("  ");
      html_print("-");
      for (int j=0;j<dividend_deg+1;j++)
      {
        html_print("-");
      }
      int rem_deg = p_gf2_deg( bit_size, remainder );
      html_print(" |\n");
      html_print("  ");
      for (int j=0;j<(dividend_deg-rem_deg);j++)
      {
        html_print(" ");
      }
      html_print_p_bin_skiplz(bit_size,remainder);
      html_print(" |\n");
    }
    else
    {
      html_print("- ");
      for (int j=0;j<i;j++)
      {
        html_print(" ");
      }
      html_print("b");
      for (int j=0;j<divisor_deg+1;j++)
      {
        html_print("0");
      }
      for (int j=0;j<(dividend_deg-i-divisor_deg);j++)
      {
        html_print(" ");
      }
      html_print(" | 0");
      html_print("\n");
    }
  }

  html_print("</pre></tt>");
  html_br();
  html_print_p("c<sub>quotient</sub>",bit_size,quotient);
  html_br();
  html_print_p("c<sub>remainder</sub>",bit_size,remainder);
}

void
p_gf2_mod_html( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* remainder )
{
  html_print("<h2>gf(2) mod</h2>");
  html_println("c = a \% b");
  html_br();
  html_println("Calculate c = a/b, result is c<sub>remainder</sub>");

  uint32_t* quotient = p_gf2_alloca( bit_size );
  p_gf2_div_html( bit_size, dividend, divisor, quotient, remainder );
}
