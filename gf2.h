#pragma once

#define p_gf2_wc( bit_size )             ((bit_size) >> 5)
#define p_gf2_alloca( bit_size )         (uint32_t*)__builtin_alloca( p_gf2_wc(bit_size)*sizeof(uint32_t) )
#define p_gf2_nalloca( bit_size, count ) (uint32_t*)__builtin_alloca( p_gf2_wc(bit_size)*sizeof(uint32_t)*count )

void     p_gf2_zero( int bit_size, uint32_t* x );
void     p_gf2_one( int bit_size, uint32_t* x );
void     p_gf2_movzx( int x_bit_size, uint32_t* x, int y_bit_size, uint32_t* y );
void     p_gf2_mov( int bit_size, uint32_t* x, uint32_t* y );
void     p_gf2_sll( int bit_size, uint32_t* x, int sa );
void     p_gf2_add( int bit_size, uint32_t* x, uint32_t* y );
uint32_t p_gf2_bt( int bit_size, uint32_t* x, int bit_ndx );
void     p_gf2_mul( int bit_size, uint32_t* x, uint32_t* y );
void     p_gf2_derivative( int bit_size, uint32_t* a, uint32_t* derivative );
void     p_gf2_and( int bit_size, uint32_t* x, uint32_t* y );
void     p_gf2_oddmask( int bit_size, uint32_t* x );
void     p_gf2_srl( int bit_size, uint32_t* x, int sa );
uint32_t p_gf2_eq( int bit_size, uint32_t* x, uint32_t* y );
uint32_t p_gf2_eqz( int bit_size, uint32_t* x );
void     p_gf2_gcd( int bit_size, uint32_t* a, uint32_t* b, uint32_t* gcd );
void     p_gf2_div( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder );
void     p_gf2_mod( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* remainder );
void     p_gf2_inc( int bit_size, uint32_t* x );
int      p_gf2_deg( int bit_size, uint32_t* x );
void     p_gf2_sff( int bit_size, uint32_t* x, int* sf_factor_count, uint32_t* sf_factors, int* sf_factors_pow, int sa );
void     p_gf2_sqrt( int bit_size, uint32_t* x, uint32_t* sqrt_x );
int      p_gf2_rref_pivot( int bit_size, uint32_t* M, int row_count, int deg );
void     p_gf2_rref( int bit_size, uint32_t* M, int row_count );
void     p_gf2_berlekamp_nontrivial_bases( int bit_size, uint32_t* a, int* basis_count, uint32_t* bases );
void     p_gf2_berlekamp_nontrivial_factors( int bit_size, uint32_t* a, int* known_factor_count, uint32_t* known_factors );
void     p_gf2_berlekamp_irreducible_factors( int bit_size, uint32_t* a, int* irreducible_factor_count, uint32_t* irreducible_factors );
void     p_gf2_factor( int bit_size, uint32_t* x, uint32_t* irreducible_factors, int* irreducible_factor_count );

// Debug prints
void     p_gf2_print( int bit_size, uint32_t* x );
void     p_gf2_print_p( const char* name, int bit_size, uint32_t* x );
void     p_gf2_print_hex( int bit_size, uint32_t* x );
void     p_gf2_print_hex_skiplz( int bit_size, uint32_t* x );
void     p_gf2_print_bin( int bit_size, uint32_t* x );
void     p_gf2_print_bin_w( int bit_size, uint32_t* x, int width );
void     p_gf2_print_bin_w_aug( int bit_size, uint32_t* x, int width );
void     p_gf2_print_bin_skiplz( int bit_size, uint32_t* x );

// Variations that print debugging info
void     p_gf2_div_print( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder );
void     p_gf2_rref_html( int bit_size, uint32_t* M, int row_count );
void     p_gf2_berlekamp_nontrivial_bases_html( int bit_size, uint32_t* a, int* basis_count, uint32_t* bases );
void     p_gf2_berlekamp_nontrivial_factors_html( int bit_size, uint32_t* a, int* known_factor_count, uint32_t* known_factors );
void     p_gf2_berlekamp_irreducible_factors_html( int bit_size, uint32_t* a, int* irreducible_factor_count, uint32_t* irreducible_factors );
void     p_gf2_derivative_html( int bit_size, uint32_t* a, uint32_t* derivative );
void     p_gf2_sqrt_html( int bit_size, uint32_t* x, uint32_t* sqrt_x );
void     p_gf2_sff_html( int bit_size, uint32_t* x, int* sf_factor_count, uint32_t* sf_factors, int* sf_factors_pow, int sa );
void     p_gf2_gcd_html( int bit_size, uint32_t* a, uint32_t* b, uint32_t* gcd );
void     p_gf2_div_html( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* quotient, uint32_t* remainder );
void     p_gf2_mod_html( int bit_size, uint32_t* dividend, uint32_t* divisor, uint32_t* remainder );
void     p_gf2_factor_html( int bit_size, uint32_t* x, uint32_t* irreducible_factors, int* irreducible_factor_count );
