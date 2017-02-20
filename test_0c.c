#include <stdint.h>
#include <stdio.h>
#include "gf2.h"

void
print_test( int size, uint32_t* in, uint32_t* out, int out_count )
{
  printf("IN:\n");
  printf("  %d\n",size);
  printf("  ");
  p_gf2_print( size*2, in );
  printf("\n");
  printf("\n");

  printf("OUT:\n");
  int wc = size/32;
  for (int i=0;i<out_count;i++)
  {
    printf("  (",i);
    p_gf2_print( size, out + (2*i*wc) );
    printf(") (");
    p_gf2_print( size, out + (2*i*wc)+wc );
    printf(")\n");
  }
}

void
print_test1( void )
{
  int size = 32;
  uint32_t in[]  = { 0x000073af, 0x00000000 };
  uint32_t out[] = { 0x00000001, 0x000073af,
                     0x00000083, 0x000000e5,
                     0x000000e5, 0x00000083,
                     0x000073af, 0x00000001 };
  int out_count = 4;

  printf("== TEST CASE 1 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

void
print_test2( void )
{
  int size = 32;
  uint32_t in[]  = { 0x738377c1, 00000000 };
  uint32_t out[] = { 0x00000001, 0x738377c1,
                     0x0000b0c5, 0x0000cd55,
                     0x0000cd55, 0x0000b0c5,
                     0x738377c1, 0x00000001 };
  int out_count = 4;

  printf("== TEST CASE 2 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

void
print_test3( void )
{
  int size = 32;
  uint32_t in[]  = { 0x46508fb7, 0x6677e201 };
  uint32_t out[] = { 0xb0c152f9, 0xebf2831f,
                     0xebf2831f, 0xb0c152f9 };

  int out_count = 2;

  printf("== TEST CASE 3 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

void
print_test4( void )
{
  int size = 64;
  uint32_t in[] = { 0xf3268b49, 0x661859eb, 0x0b324559, 0x65ee6bda };
  uint32_t out[] = { 0x0cf5c2bf, 0x9aba68ef, 0xc18fb79b, 0xde70eef7,
                     0xc18fb79b, 0xde70eef7, 0x0cf5c2bf, 0x9aba68ef };
  int out_count = 2;

  printf("== TEST CASE 4 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

void
print_test5( void )
{
  int size = 128;
  uint32_t in[] = { 0xa91db473, 0xfcea8db4, 0xf3bb434a, 0x8dba2f16, 0x51abc87e, 0x92c44759, 0x5c1a16d3, 0x6111c6f4 };
  uint32_t out[] = { 0xa30d28bd, 0xbda19675, 0x3f95d074, 0xb6f69434, 0xc58f4047, 0xd73fe36a, 0x24be2846, 0xe2ebe432,
                     0xc58f4047, 0xd73fe36a, 0x24be2846, 0xe2ebe432, 0xa30d28bd, 0xbda19675, 0x3f95d074, 0xb6f69434 };
  int out_count = 2;

  printf("== TEST CASE 5 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

void
print_test6( void )
{
  int size = 256;
  uint32_t in[] = { 0x4af6fc33, 0x39029380, 0x465c5267, 0xc72f6a8b, 0x0906e6d0, 0xca60550f, 0x14a5e47c, 0x42ad10fb, 0x4a3bb446, 0xbb74360a, 0x5ea02b9c, 0x23c68553, 0x3fade253, 0xe270ba24, 0x39e141ad, 0x6c38c43d };
  uint32_t out[] = { 0x320a18d5, 0xb61b13f6, 0x1aaaa61c, 0x0afe2a41, 0x1a4ff107, 0x84cc2efc, 0x956ff31d, 0xfa595299, 0x33749a7f, 0x6cc9659d, 0xdc503569, 0xef4d0ef5, 0x73b746c5, 0xb8fb36d3, 0x7616e9d6, 0xb21251c4,
                     0x33749a7f, 0x6cc9659d, 0xdc503569, 0xef4d0ef5, 0x73b746c5, 0xb8fb36d3, 0x7616e9d6, 0xb21251c4, 0x320a18d5, 0xb61b13f6, 0x1aaaa61c, 0x0afe2a41, 0x1a4ff107, 0x84cc2efc, 0x956ff31d, 0xfa595299 };
  int out_count = 2;

  printf("== TEST CASE 6 ==\n");
  print_test( size, in, out, out_count );
  printf("\n");
}

int
main( void )
{
  print_test1();
  print_test2();
  print_test3();
  print_test4();
  print_test5();
  print_test6();

  return 0;
}
