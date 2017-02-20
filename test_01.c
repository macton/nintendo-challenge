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

  uint32_t  b[ 2 * test_count ];
  uint32_t  count_a[ 64 ];
  uint32_t  count_b[ 64 ];

  memset(b,0,test_count*2*sizeof(uint32_t));
  memset(count_a,0,64*sizeof(uint32_t));
  memset(count_b,0,64*sizeof(uint32_t));

  for (int i=0;i<test_count;i++)
  {
    encode( &a[i],&b[i*2],32);

    for (int j=0;j<32;j++) 
    {
      if (a[i] & ( 1ULL<<j )) 
      {
        count_a[j]++;
      }
    }

    for (int j=0;j<32;j++) 
    {
      if (a[i+1] & ( 1ULL<<j )) 
      {
        count_a[32+j]++;
      }
    }

    for (int j=0;j<32;j++) 
    {
      if (b[(i*2)+0] & ( 1ULL<<j )) 
      {
        count_b[j]++;
      }
    }

    for (int j=0;j<32;j++) 
    {
      if (b[(i*2)+1] & ( 1ULL<<j )) 
      {
        count_b[32+j]++;
      }
    }
  }

  for (int j=0;j<64;j++)
  {
    printf("%d: %d %d %0.6f\n",j,count_a[j],count_b[j],(float)(count_a[j]*count_b[j])/(float)(count_a[j]*test_count));
  }


  return 0;
}

