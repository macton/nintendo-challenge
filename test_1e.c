#include <stdint.h>
#include <stdio.h>
#include "gf2.h"
#include "html.h"
#include "decode.h"

void
print_test1( void )
{
  int      size = 32;
  uint32_t a[]  = { 0x000073af, 0x00000000 };

  printf("==== TEST 1 ====\n");
  html_page_push("TEST 1");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test2( void )
{
  int      size = 32;
  uint32_t a[]  = { 0x738377c1, 00000000 };

  printf("==== TEST 2 ====\n");
  html_page_push("TEST 2");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test3( void )
{
  int      size = 32;
  uint32_t a[]  = { 0x46508fb7, 0x6677e201 };

  printf("==== TEST 3 ====\n");
  html_page_push("TEST 3");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test4( void )
{
  int      size = 64;
  uint32_t a[] = { 0xf3268b49, 0x661859eb, 0x0b324559, 0x65ee6bda };

  printf("==== TEST 4 ====\n");
  html_page_push("TEST 4");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test5( void )
{
  int      size = 128;
  uint32_t a[]  = { 0xa91db473, 0xfcea8db4, 0xf3bb434a, 0x8dba2f16, 0x51abc87e, 0x92c44759, 0x5c1a16d3, 0x6111c6f4 };

  printf("==== TEST 5 ====\n");
  html_page_push("TEST 5");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test6( void )
{
  int      size = 256;
  uint32_t a[]  = { 0x4af6fc33, 0x39029380, 0x465c5267, 0xc72f6a8b, 0x0906e6d0, 0xca60550f, 0x14a5e47c, 0x42ad10fb, 0x4a3bb446, 0xbb74360a, 0x5ea02b9c, 0x23c68553, 0x3fade253, 0xe270ba24, 0x39e141ad, 0x6c38c43d };

  printf("==== TEST 6 ====\n");
  html_page_push("TEST 6");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

void
print_test7( void )
{
  int      size = 32;
  uint32_t a[]  = { 0x7fe00033, 0x0caea3f4  };

  printf("==== TEST 7 ====\n");
  html_page_push("TEST 7");
  decode_html( size, a );
  html_page_pop();
  html_br();
}

int
main( void )
{
  printf("OUTPUT -> index.html\n");

  html_open();

  // print_test1();
  // print_test2();
  // print_test3();
  // print_test4();
  // print_test5();
  // print_test6();
  print_test7();

  html_close();

  return 0;
}
