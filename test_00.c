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
  uint32_t* b    = (uint32_t*)__builtin_alloca( test_count * 2 * sizeof(uint32_t) );

  memset(b,0,test_count*2*sizeof(uint32_t));

  for (int i=0;i<test_count;i++)
  {
    encode( &a[i],&b[i*2],32);

    printf("a 0x%08x 0x%08x ",a[i],a[i+1]);
    print_32b(a[i]);
    printf(" ");
    print_32b(a[i+1]);
    printf("\n");

    printf("b 0x%08x 0x%08x ",b[(i*2)+0], b[(i*2)+1]);
    print_32b(b[(i*2)+0]);
    printf(" ");
    print_32b(b[(i*2)+1]);
    printf("\n");
  }


  return 0;
}

