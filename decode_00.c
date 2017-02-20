#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf2.h"
#include "html.h"

int
snprint_line( char* line, int line_len, int bit_size, uint32_t* a, uint32_t* b )
{
  int line_ndx     = 0;
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx;

  top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = a[ top_word_ndx ];
    line_ndx += snprintf( line+line_ndx, line_len, "%08x",top_word );
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  line_ndx += snprintf( line+line_ndx, line_len, " " );

  top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = b[ top_word_ndx ];
    line_ndx += snprintf( line+line_ndx, line_len, "%08x",top_word );
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  line_ndx += snprintf( line+line_ndx, line_len, "\n" );

  return line_ndx;
}

void
decode_html( int multiplicand_size, uint32_t* c )
{
  int       bit_size     = multiplicand_size * 2;
  int       factor_count = 0;
  int       c_deg        = p_gf2_deg( bit_size, c );
  uint32_t* factors      = p_gf2_nalloca( bit_size, c_deg );

  html_print("<h2>decode(size,f)</h2>");

  html_indent_push();
  html_print("<tt>");
  html_println("size = %d",multiplicand_size);
  html_print("</tt>");
  html_br();
  html_println("<i>f is interpreted as a finite field polynomial in gf(2) of degree no greater than %d (bit depth=2*size).</i>", bit_size-1);
  html_print_p("f",bit_size,c);
  html_indent_pop();
  html_br();

  html_println("f is the product of two other gf(2) polynomials (a, b)");
  html_println("a and b are each no larger than %d degrees (bit depth=%d)",multiplicand_size-1,multiplicand_size);
  html_br();

  html_page_push("Step 1: Find all irreducible factors of f");

  p_gf2_factor_html( bit_size, c, factors, &factor_count );

  html_page_pop();
  html_br();

  html_page_push("Step 2. Find all (a,b) which satisfy constraints.");
  html_print("<h2>Decode: Find all (a,b) which satisfy constraints</h2>");
  html_println("List individual factors:");
  html_br();
  html_indent_push();
  for (int i=0;i<factor_count;i++)
  {
    char fname[64];
    sprintf(fname,"f<sub>%d</sub>",i);
    html_print_p(fname, bit_size, factors+( p_gf2_wc(bit_size)*i) );
    html_br();
  }
  html_indent_pop();
  html_println("For each combination of factors (where f=ab)");
  html_br();

  uint64_t   group_count = 1 << factor_count;
  int        max_deg     = bit_size/2;
  uint32_t*  found_a     = p_gf2_nalloca( max_deg, 64 );
  uint32_t*  found_b     = p_gf2_nalloca( max_deg, 64 );
  int        found_count = 0;

  for (uint64_t i=1;i<group_count/2;i++)
  {
    uint64_t group_a = i;

    uint32_t* a = p_gf2_alloca( bit_size );
    uint32_t* b = p_gf2_alloca( bit_size );
    uint32_t* c = p_gf2_alloca( bit_size );

    char aname[64];
    char bname[64];
    sprintf(aname,"a<sub>%d</sub>",(int)(i-1));
    sprintf(bname,"b<sub>%d</sub>",(int)(i-1));

    html_indent_push();
  
    html_print("<tt>");

    html_print("%s = ",aname);
    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        html_print("(");
        html_print_p_bin( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    html_print("%s = ",aname);
    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        html_print("(");
        html_print_p_hex( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    html_print("%s = ",aname);
    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        html_print("(");
        html_print_p_poly( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    p_gf2_one( bit_size, a );
    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        p_gf2_mul( bit_size, a, factors+( p_gf2_wc(bit_size)*j) );
      }
    }
    html_print_p(aname,bit_size,a);
    html_br();

    html_print("%s = ",bname);
    for (int j=0;j<factor_count;j++)
    {
      if (!( group_a & (1 << j) ))
      {
        html_print("(");
        html_print_p_bin( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    html_print("%s = ",bname);
    for (int j=0;j<factor_count;j++)
    {
      if (!( group_a & (1 << j) ))
      {
        html_print("(");
        html_print_p_hex( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    html_print("%s = ",bname);
    for (int j=0;j<factor_count;j++)
    {
      if (!( group_a & (1 << j) ))
      {
        html_print("(");
        html_print_p_poly( bit_size, factors+( p_gf2_wc(bit_size)*j) );
        html_print(")");
      }
    }
    html_br();

    p_gf2_one( bit_size, b );
    for (int j=0;j<factor_count;j++)
    {
      if (!( group_a & (1 << j) ))
      {
        p_gf2_mul( bit_size, b, factors+( p_gf2_wc(bit_size)*j) );
      }
    }
    html_print_p(bname,bit_size,b);
    html_br();

    html_print("</tt>");
  
    p_gf2_one( bit_size, a );
    p_gf2_one( bit_size, b );

    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        p_gf2_mul( bit_size, a, factors+( p_gf2_wc(bit_size)*j) );
      }
      else
      {
        p_gf2_mul( bit_size, b, factors+( p_gf2_wc(bit_size)*j) );
      }
    }

    int a_deg  = p_gf2_deg( bit_size,a );
    int b_deg  = p_gf2_deg( bit_size,b );
    int a_fit  = (a_deg < max_deg);
    int b_fit  = (b_deg < max_deg);
    int is_fit = (a_fit && b_fit);

    html_println("(deg(%s) < %d)? %s",aname, max_deg,(a_fit)?"<span style=\"color:green\">PASS</span>":"<span style=\"color:red\">FAIL</span>" );
    html_println("(deg(%s) < %d)? %s",bname, max_deg,(b_fit)?"<span style=\"color:green\">PASS</span>":"<span style=\"color:red\">FAIL</span>" );

    if (is_fit)
    {
      int is_found = 0;
      for (int j=0;j<found_count;j++)
      {
        if ( p_gf2_eq( max_deg, a, found_a + (p_gf2_wc(max_deg)*j) ) )
        {
          is_found = 1;
          break; 
        }
      }
      if (!is_found)
      {
        p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*(found_count)), a );
        p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*(found_count)), b );
        p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*(found_count+1)), b );
        p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*(found_count+1)), a );
        found_count += 2;   
        html_println("<span style=\"color:green\">ADD combination (a,b) and (b,a) to results.</span>");
      }
      else
      {
        html_println("<span style=\"color:green\">SKIP. Combination (a,b) or (b,a) already found.</span>");
      }
    }
    else
    {
      html_println("<span style=\"color:red\">SKIP. Combination does not fit in result constraints.</span>");
    }
    html_br();

    html_indent_pop();
  }

  if (found_count == 0)
  {
    uint32_t* one = p_gf2_alloca( max_deg );
    p_gf2_one( max_deg, one );
    p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*0), c );
    p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*0), one );
    p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*1), one );
    p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*1), c );
    found_count = 2;
  }

  html_println("Result table of a b (hex):");
  html_indent_push();
  html_print("<tt><pre>");
  for (int i=0;i<found_count;i++)
  {
    uint32_t* a    = found_a + (p_gf2_wc(max_deg)*i);
    uint32_t* b    = found_b + (p_gf2_wc(max_deg)*i);

    html_print_p_hex( max_deg, a );
    html_print(" ");
    html_print_p_hex( max_deg, b );
    html_print("\n");
  }
  html_print("</pre></tt>");
  html_indent_pop();

  html_page_pop();
  html_br();

  html_page_push("Step 3. Format and sort results");
  html_print("<h2>Decode: Format and sort results</h2>");
  html_println("Input table of a b (hex):");
  html_indent_push();
  html_print("<tt><pre>");
  for (int i=0;i<found_count;i++)
  {
    uint32_t* a    = found_a + (p_gf2_wc(max_deg)*i);
    uint32_t* b    = found_b + (p_gf2_wc(max_deg)*i);

    html_print_p_hex( max_deg, a );
    html_print(" ");
    html_print_p_hex( max_deg, b );
    html_print("\n");
  }
  html_print("</pre></tt>");
  html_indent_pop();

  int   line_len = (((max_deg/4)+1)*2)+1;
  char* lines    = (char*)__builtin_alloca( line_len * found_count ); 
  memset( lines, 0, line_len * found_count );

  for (int i=0;i<found_count;i++)
  {
    uint32_t* a    = found_a + (p_gf2_wc(max_deg)*i);
    uint32_t* b    = found_b + (p_gf2_wc(max_deg)*i);
    char*     line = lines + (i*line_len);

    snprint_line( line, line_len, max_deg, a, b );
  }
  qsort( lines, found_count, line_len, (int(*)(const void*, const void*))&strcmp );

  html_println("Sort table <i>alphabetically</i>");
  html_indent_push();
  html_print("<tt><pre>");
  for (int i=0;i<found_count;i++)
  {
    char*     line = lines + (i*line_len);
    html_print( line );
  }
  html_print("</pre></tt>");
  html_indent_pop();

  html_page_pop();
  html_br();
  html_println("Step 4. Output");
  html_br();
  html_indent_push();
  html_print("<tt>");

  for (int i=0;i<found_count;i++)
  {
    char*     line = lines + (i*line_len);
    printf("%s", line);
    html_println( line );
  }

  html_print("</tt>");
  html_br();
  html_indent_pop();

  html_hr();
  html_references_push();
  html_reference("http://mathworld.wolfram.com/FiniteField.html");
  html_reference("http://www.math.umn.edu/~garrett/coding/Overheads/08_crcs.pdf");
  html_references_pop();
}

void
decode( int multiplicand_size, uint32_t* c )
{
  int       bit_size     = multiplicand_size * 2;
  int       factor_count = 0;
  int       c_deg        = p_gf2_deg( bit_size, c );
  uint32_t* factors      = p_gf2_nalloca( bit_size, c_deg );

  p_gf2_factor( bit_size, c, factors, &factor_count );

  uint64_t   group_count = 1 << factor_count;
  int        max_deg     = bit_size/2;
  uint32_t*  found_a     = p_gf2_nalloca( max_deg, 64 );
  uint32_t*  found_b     = p_gf2_nalloca( max_deg, 64 );
  int        found_count = 0;

  for (uint64_t i=1;i<group_count/2;i++)
  {
    uint64_t group_a = i;

    uint32_t* a = p_gf2_alloca( bit_size );
    uint32_t* b = p_gf2_alloca( bit_size );
    uint32_t* c = p_gf2_alloca( bit_size );

    p_gf2_one( bit_size, a );
    p_gf2_one( bit_size, b );

    for (int j=0;j<factor_count;j++)
    {
      if ( group_a & (1 << j) )
      {
        p_gf2_mul( bit_size, a, factors+( p_gf2_wc(bit_size)*j) );
      }
      else
      {
        p_gf2_mul( bit_size, b, factors+( p_gf2_wc(bit_size)*j) );
      }
    }

    int a_deg  = p_gf2_deg( bit_size,a );
    int b_deg  = p_gf2_deg( bit_size,b );
    int a_fit  = (a_deg < max_deg);
    int b_fit  = (b_deg < max_deg);
    int is_fit = (a_fit && b_fit);

    if (is_fit)
    {
      int is_found = 0;
      for (int j=0;j<found_count;j++)
      {
        if ( p_gf2_eq( max_deg, a, found_a + (p_gf2_wc(max_deg)*j) ) )
        {
          is_found = 1;
          break; 
        }
      }
      if (!is_found)
      {
        p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*(found_count)), a );
        p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*(found_count)), b );
        p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*(found_count+1)), b );
        p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*(found_count+1)), a );
        found_count += 2;   
      }
    }
  }

  if (found_count == 0)
  {
    uint32_t* one = p_gf2_alloca( max_deg );
    p_gf2_one( max_deg, one );
    p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*0), c );
    p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*0), one );
    p_gf2_mov( max_deg, found_a + (p_gf2_wc(max_deg)*1), one );
    p_gf2_mov( max_deg, found_b + (p_gf2_wc(max_deg)*1), c );
    found_count = 2;
  }

  int   line_len = (((max_deg/4)+1)*2)+1;
  char* lines    = (char*)__builtin_alloca( line_len * found_count ); 
  memset( lines, 0, line_len * found_count );

  for (int i=0;i<found_count;i++)
  {
    uint32_t* a    = found_a + (p_gf2_wc(max_deg)*i);
    uint32_t* b    = found_b + (p_gf2_wc(max_deg)*i);
    char*     line = lines + (i*line_len);

    snprint_line( line, line_len, max_deg, a, b );
  }
  qsort( lines, found_count, line_len, (int(*)(const void*, const void*))&strcmp );

  for (int i=0;i<found_count;i++)
  {
    char*     line = lines + (i*line_len);
    printf("%s", line);
  }
}


