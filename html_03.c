#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "gf2.h"

int   desc_page_count = 0;
int   desc_page_ndx = 0;
FILE* desc_page_file[ 1024 ];
FILE* cur_page_file;

void html_open()
{
  cur_page_file     = fopen( "index.html", "w+" );
  desc_page_file[0] = cur_page_file;
  desc_page_ndx      = 0;
  desc_page_count    = 0;
  fprintf( cur_page_file, "<html>" );
  fprintf( cur_page_file, "<body style=\"white-space: nowrap;\">" );
}

void html_close() 
{
  fprintf( cur_page_file, "</body>" );
  fprintf( cur_page_file, "</html>" );
  fclose( desc_page_file[0] );
}

void html_page_push( const char *format , ... )
{
  char filename[256];
  desc_page_ndx++;
  desc_page_count++;

  sprintf( filename, "%08x.html", desc_page_count );
  fprintf( cur_page_file, "<a href='%s'>", filename );

  va_list arglist;
  va_start( arglist, format );
  vfprintf( cur_page_file, format, arglist );
  va_end( arglist );

  fprintf( cur_page_file, "</a>" );

  cur_page_file                   = fopen( filename, "w+" );
  desc_page_file[ desc_page_ndx ] = cur_page_file;
  fprintf( cur_page_file, "<html>" );
  fprintf( cur_page_file, "<body style=\"white-space: nowrap;\">" );
}

void html_page_push_silent()
{
  char filename[256];
  desc_page_ndx++;
  desc_page_count++;

  sprintf( filename, "%08x.html", desc_page_count );

  cur_page_file                   = fopen( filename, "w+" );
  desc_page_file[ desc_page_ndx ] = cur_page_file;
}

void html_page_pop()
{
  fprintf( cur_page_file, "</body>" );
  fprintf( cur_page_file, "</html>" );
  fclose( cur_page_file );
  desc_page_ndx--;
  cur_page_file = desc_page_file[ desc_page_ndx ];
}

void html_println( const char *format , ... )
{
  va_list arglist;
  va_start( arglist, format );
  vfprintf( cur_page_file, format, arglist );
  va_end( arglist );
  fprintf( cur_page_file, "<br>" );
}

void html_print( const char *format , ... )
{
  va_list arglist;
  va_start( arglist, format );
  vfprintf( cur_page_file, format, arglist );
  va_end( arglist );
}

void html_br()
{
  fprintf( cur_page_file, "<br>" );
}

void html_hr()
{
  fprintf( cur_page_file, "<hr>" );
}

void html_references_push()
{
  fprintf( cur_page_file, "See also:<ul>" );
}

void html_reference( const char* link )
{
  fprintf( cur_page_file, "<li><a href='%s'>%s</a></li>", link, link );
}

void html_ul_push( const char* title )
{
  fprintf( cur_page_file, "%s<ul>", title );
}

void html_li_push()
{
  fprintf( cur_page_file, "<li>" );
}

void html_li( const char *format , ... )
{
  va_list arglist;
  va_start( arglist, format );
  fprintf( cur_page_file, "<li>" );
  vfprintf( cur_page_file, format, arglist );
  fprintf( cur_page_file, "</li>" );
  va_end( arglist );
}

void html_li_pop()
{
  fprintf( cur_page_file, "</li>" );
}

void html_ul_pop()
{
  fprintf( cur_page_file, "</ul>" );
}

void html_references_pop()
{
  fprintf( cur_page_file, "</ul>" );
}

void html_print_p_bin( int bit_size, uint32_t* x )
{
  fprintf( cur_page_file, "b");
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    for (int i=31;i>=0;i--)
    {
      fprintf( cur_page_file, "%c",top_word&(1<<i)?'1':'0');
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void html_print_p_bin_skiplz( int bit_size, uint32_t* x )
{
  fprintf( cur_page_file, "b");
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
        fprintf( cur_page_file, "%c",top_word&(1<<i)?'1':'0');
      }
      else
      {
        if ( top_word&(1<<i) )
        {
          found_top_bit = 1;
          fprintf( cur_page_file, "1" );
        }
      }
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!found_top_bit)
  {
    fprintf( cur_page_file, "0" );
  }
}

void html_print_p_hex( int bit_size, uint32_t* x )
{
  fprintf( cur_page_file, "0x");
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    fprintf( cur_page_file, "%08x",top_word);
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
}

void html_print_p_hex_skiplz( int bit_size, uint32_t* x )
{
  fprintf( cur_page_file, "0x");
  int word_count   = p_gf2_wc(bit_size);
  int top_word_ndx = word_count-1;
  int found_word   = 0;
  do
  {
    uint32_t top_word = x[ top_word_ndx ];
    if (top_word)
    { 
      fprintf( cur_page_file, "%x",top_word);
      found_word = 1;
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!found_word)
  {
    fprintf( cur_page_file, "0" );
  }
}

void html_print_p_poly( int bit_size, uint32_t* x )
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
          fprintf( cur_page_file, "%s1",has_prev?" + ":"");
        }
        else if (( top_word_ndx == 0 ) && (i==1))
        {
          fprintf( cur_page_file, "%sx",has_prev?" + ":"");
        }
        else
        {
          fprintf( cur_page_file, "%sx<sup>%d</sup>",has_prev?" + ":"",(top_word_ndx*32)+i);
        }
        has_prev = 1;
      }
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!has_prev)
  {
    fprintf( cur_page_file, "0");
  }
}

void html_print_p_poly_input( int bit_size, uint32_t* x )
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
          fprintf( cur_page_file, "%s1",has_prev?" + ":"");
        }
        else if (( top_word_ndx == 0 ) && (i==1))
        {
          fprintf( cur_page_file, "%sx",has_prev?" + ":"");
        }
        else
        {
          fprintf( cur_page_file, "%sx^%d",has_prev?" + ":"",(top_word_ndx*32)+i);
        }
        has_prev = 1;
      }
    }
    top_word_ndx--;
  }
  while (top_word_ndx >= 0);
  if (!has_prev)
  {
    fprintf( cur_page_file, "0");
  }
}

void html_print_p( const char* name, int bit_size, uint32_t* x )
{
  html_print( "<tt>" );
  html_print( "%s = ", name);
  html_print_p_bin_skiplz( bit_size, x );
  html_br();

  html_print( "%s = ", name);
  html_print_p_hex_skiplz( bit_size, x );
  html_br();

  html_print( "%s = ", name);
  html_print_p_poly( bit_size, x );
  html_br();

  html_print( "%s = ", name);
  html_print_p_poly_input( bit_size, x );
  html_br();
  html_print( "</tt>" );
}
 
void html_print_p_factors( char* name, int bit_size, int factor_count, uint32_t* factors )
{
  html_print("<tt>");
  html_print("%s = ", name);
  for (int i=0;i<factor_count;i++)
  {
    if (p_gf2_eqz(bit_size,factors+(p_gf2_wc(bit_size)*i)))
    {
      continue;
    }
    html_print("(b");
    html_print_p_bin_skiplz(bit_size,factors+(p_gf2_wc(bit_size)*i));
    html_print(")");
  }
  html_br();
  html_print("%s = ", name);
  for (int i=0;i<factor_count;i++)
  {
    if (p_gf2_eqz(bit_size,factors+(p_gf2_wc(bit_size)*i)))
    {
      continue;
    }
    html_print("(0x");
    html_print_p_hex_skiplz(bit_size,factors+(p_gf2_wc(bit_size)*i));
    html_print(")");
  }
  html_br();
  html_print("%s = ", name);
  for (int i=0;i<factor_count;i++)
  {
    if (p_gf2_eqz(bit_size,factors+(p_gf2_wc(bit_size)*i)))
    {
      continue;
    }
    html_print("(");
    html_print_p_poly(bit_size,factors+(p_gf2_wc(bit_size)*i));
    html_print(")");
  }
  html_br();
  html_print("</tt>");
}

void html_indent_push()
{
  html_print("<div style=\"margin-left:4em;\">");
}

void html_indent_pop()
{
  html_print("</div>");
}

void html_code_begin()
{
  html_print("<tt><pre>");
}

void html_code_end()
{
  html_print("</pre></tt>");
}

void
html_print_bin_w( int bit_size, uint32_t* x, int width )
{
  for (int i=width-1;i>=0;i--)
  {
    fprintf(cur_page_file,"%c",p_gf2_bt( bit_size, x, i )?'1':'0');
  }
}

void
html_print_bin_w_aug( int bit_size, uint32_t* x, int width )
{
  for (int i=width-1;i>=0;i--)
  {
    fprintf(cur_page_file,"%c",p_gf2_bt( bit_size, x, i )?'1':'0');
    if ( i == (width/2) ) {
      fprintf(cur_page_file," ");
    }
  }
}
