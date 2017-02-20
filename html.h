#pragma once

void html_open();
void html_close();
void html_page_push( const char *format , ... );
void html_page_push_silent();
void html_page_pop();
void html_println( const char *format , ... );
void html_print( const char *format , ... );
void html_br();
void html_hr();
void html_references_push();
void html_reference( const char* link );
void html_ul_push( const char* title );
void html_li_push();
void html_li( const char *format , ... );
void html_li_pop();
void html_ul_pop();
void html_references_pop();
void html_print_p_bin( int bit_size, uint32_t* x );
void html_print_p_bin_skiplz( int bit_size, uint32_t* x );
void html_print_p_hex( int bit_size, uint32_t* x );
void html_print_p_hex_skiplz( int bit_size, uint32_t* x );
void html_print_p_poly( int bit_size, uint32_t* x );
void html_print_p_poly_input( int bit_size, uint32_t* x );
void html_print_p( const char* name, int bit_size, uint32_t* x );
void html_print_p_factors( char* name, int bit_size, int factor_count, uint32_t* factors );
void html_print_bin_w_aug( int bit_size, uint32_t* x, int width );
void html_print_bin_w( int bit_size, uint32_t* x, int width );
void html_indent_push();
void html_indent_pop();
void html_code_begin();
void html_code_end();

