#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rand_ints.h"

void
encode( uint32_t* a, uint32_t* b, int size )
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++) 
    {
      b[(i + j) / 32] ^= ( (a[i / 32] >> (i % 32)) & (a[j / 32 + size / 32] >> (j % 32)) & 1 ) << ((i + j) % 32);   // Magic centaurian operation
    }
  }
}

void
encode2_32( uint32_t* a, uint32_t* b ) 
{
  uint64_t a0 = (uint64_t)a[0];
  uint64_t a1 = (uint64_t)a[1];
  uint64_t c  = 0;

  for (int i=0;i<32;i++)
  {
    c ^= (a0 & (1<<i)) * a1;
  }

  b[0] = (uint32_t)c;
  b[1] = (uint32_t)(c >> 32);
}

void
print_32b( uint32_t a )
{
  for (int i=31;i>=0;i--)
  {
    printf("%c",a&(1<<i)?'1':'0');
  }
}

int
main( void )
{
  int test_count = g_RandIntCount-1;
  uint32_t* a    = g_RandInts;
  uint32_t  b0[2];
  uint32_t  b[2];
  int fail_count = 0;

  for (int i=0;i<test_count;i++)
  {
    printf("a  0x%08x 0x%08x ",a[1],a[0]);
    print_32b(a[1]);
    printf(" ");
    print_32b(a[0]);
    printf("\n");

    b[0] = 0;
    b[1] = 0;

    encode( &a[i],b,32);

    printf("b  0x%08x 0x%08x ",b[1], b[0]);
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);
    printf("\n");

    b0[0] = b[0];
    b0[1] = b[1];

    b[0] = 0;
    b[1] = 0;

    encode2_32( &a[i],b);

    printf("b' 0x%08x 0x%08x ",b[1], b[0]);
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);

    if ((b0[0] != b[0]) || (b0[1] != b[1]))
    {
      printf(" <- FAIL");
      fail_count++;
    }

    printf("\n");
    printf("\n");
  }
  if ( fail_count )
  {
    printf("FAIL COUNT %d\n",fail_count);
  }
  else
  {
    printf("ALL TESTS PASSED!\n");
  }

  return 0;
}

