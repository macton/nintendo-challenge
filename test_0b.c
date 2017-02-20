#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rand_ints.h"
#include "gf2.h"

void
print_32b( uint32_t a )
{
  for (int i=31;i>=0;i--)
  {
    printf("%c",a&(1<<i)?'1':'0');
  }
}


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
encode2( uint32_t* a, uint32_t* b, int size ) 
{
  int       deg = size * 2;
  uint32_t* p_a = p_gf2_alloca( deg );
  uint32_t* p_b = b;

  p_gf2_movzx( deg, p_a, size, a+0 );
  p_gf2_movzx( deg, p_b, size, a+(size/32) );
  p_gf2_mul( deg, p_b, p_a );
}

int fail_count = 0;

void
test32( void )
{
  printf("========================================================\n");
  printf("==== 32 BIT TESTS                                   ====\n");
  printf("========================================================\n");

  int test_count = g_RandIntCount-1;
  uint32_t* a    = g_RandInts;
  uint32_t  b0[2];
  uint32_t  b[2];

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

    encode2( &a[i],b,32);

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
}

void
test64( void )
{
  printf("========================================================\n");
  printf("==== 64 BIT TESTS                                   ====\n");
  printf("========================================================\n");

  int test_count = g_RandIntCount-3;
  uint32_t* a    = g_RandInts;
  uint32_t  b0[4];
  uint32_t  b[4];

  for (int i=0;i<test_count;i++)
  {
    printf("a  ");
    print_32b(a[3]);
    printf(" ");
    print_32b(a[2]);
    printf(" ");
    print_32b(a[1]);
    printf(" ");
    print_32b(a[0]);
    printf("\n");

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;

    encode( &a[i], b, 64 );

    printf("b  ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);
    printf("\n");

    b0[0] = b[0];
    b0[1] = b[1];
    b0[2] = b[2];
    b0[3] = b[3];

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;

    encode2( &a[i], b, 64 );

    printf("b' ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);

    if ((b0[0] != b[0]) || (b0[1] != b[1]) || (b0[2] != b[2]) || (b0[3] != b[3]))
    {
      printf(" <- FAIL");
      fail_count++;
    }

    printf("\n");
    printf("\n");
  }
}

void
test128( void )
{
  printf("========================================================\n");
  printf("==== 128 BIT TESTS                                  ====\n");
  printf("========================================================\n");

  int test_count = g_RandIntCount-7;
  uint32_t* a    = g_RandInts;
  uint32_t  b0[8];
  uint32_t  b[8];

  for (int i=0;i<test_count;i++)
  {
    printf("a  ");
    print_32b(a[7]);
    printf(" ");
    print_32b(a[6]);
    printf(" ");
    print_32b(a[5]);
    printf(" ");
    print_32b(a[4]);
    printf(" ");
    print_32b(a[3]);
    printf(" ");
    print_32b(a[2]);
    printf(" ");
    print_32b(a[1]);
    printf(" ");
    print_32b(a[0]);
    printf("\n");

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    b[5] = 0;
    b[6] = 0;
    b[7] = 0;

    encode( &a[i], b, 128 );

    printf("b  ");
    print_32b(b[7]);
    printf(" ");
    print_32b(b[6]);
    printf(" ");
    print_32b(b[5]);
    printf(" ");
    print_32b(b[4]);
    printf(" ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);
    printf("\n");

    b0[0] = b[0];
    b0[1] = b[1];
    b0[2] = b[2];
    b0[3] = b[3];
    b0[4] = b[4];
    b0[5] = b[5];
    b0[6] = b[6];
    b0[7] = b[7];

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    b[5] = 0;
    b[6] = 0;
    b[7] = 0;

    encode2( &a[i], b, 128 );

    printf("b' ");
    print_32b(b[6]);
    printf(" ");
    print_32b(b[5]);
    printf(" ");
    print_32b(b[4]);
    printf(" ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);

    if ( (b0[0] != b[0]) || (b0[1] != b[1]) || (b0[2] != b[2]) || (b0[3] != b[3])
      || (b0[4] != b[4]) || (b0[5] != b[5]) || (b0[6] != b[6]) || (b0[7] != b[7]))
    {
      printf(" <- FAIL");
      fail_count++;
    }

    printf("\n");
    printf("\n");
  }
}

void
test256( void )
{
  printf("========================================================\n");
  printf("==== 256 BIT TESTS                                  ====\n");
  printf("========================================================\n");

  int test_count = g_RandIntCount-15;
  uint32_t* a    = g_RandInts;
  uint32_t  b0[16];
  uint32_t  b[16];

  for (int i=0;i<test_count;i++)
  {
    printf("a  ");
    print_32b(a[15]);
    printf(" ");
    print_32b(a[14]);
    printf(" ");
    print_32b(a[13]);
    printf(" ");
    print_32b(a[12]);
    printf(" ");
    print_32b(a[11]);
    printf(" ");
    print_32b(a[10]);
    printf(" ");
    print_32b(a[9]);
    printf(" ");
    print_32b(a[8]);
    printf(" ");
    print_32b(a[7]);
    printf(" ");
    print_32b(a[6]);
    printf(" ");
    print_32b(a[5]);
    printf(" ");
    print_32b(a[4]);
    printf(" ");
    print_32b(a[3]);
    printf(" ");
    print_32b(a[2]);
    printf(" ");
    print_32b(a[1]);
    printf(" ");
    print_32b(a[0]);
    printf("\n");

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    b[5] = 0;
    b[6] = 0;
    b[7] = 0;
    b[8] = 0;
    b[9] = 0;
    b[10] = 0;
    b[11] = 0;
    b[12] = 0;
    b[13] = 0;
    b[14] = 0;
    b[15] = 0;

    encode( &a[i], b, 256 );

    printf("b  ");
    print_32b(b[15]);
    printf(" ");
    print_32b(b[14]);
    printf(" ");
    print_32b(b[13]);
    printf(" ");
    print_32b(b[12]);
    printf(" ");
    print_32b(b[11]);
    printf(" ");
    print_32b(b[10]);
    printf(" ");
    print_32b(b[9]);
    printf(" ");
    print_32b(b[8]);
    printf(" ");
    print_32b(b[7]);
    printf(" ");
    print_32b(b[6]);
    printf(" ");
    print_32b(b[5]);
    printf(" ");
    print_32b(b[4]);
    printf(" ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);
    printf("\n");

    b0[0] = b[0];
    b0[1] = b[1];
    b0[2] = b[2];
    b0[3] = b[3];
    b0[4] = b[4];
    b0[5] = b[5];
    b0[6] = b[6];
    b0[7] = b[7];
    b0[8] = b[8];
    b0[9] = b[9];
    b0[10] = b[10];
    b0[11] = b[11];
    b0[12] = b[12];
    b0[13] = b[13];
    b0[14] = b[14];
    b0[15] = b[15];

    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    b[5] = 0;
    b[6] = 0;
    b[7] = 0;
    b[8] = 0;
    b[9] = 0;
    b[10] = 0;
    b[11] = 0;
    b[12] = 0;
    b[13] = 0;
    b[14] = 0;
    b[15] = 0;

    encode2( &a[i], b, 256 );

    printf("b' ");
    print_32b(b[15]);
    printf(" ");
    print_32b(b[14]);
    printf(" ");
    print_32b(b[13]);
    printf(" ");
    print_32b(b[12]);
    printf(" ");
    print_32b(b[11]);
    printf(" ");
    print_32b(b[10]);
    printf(" ");
    print_32b(b[9]);
    printf(" ");
    print_32b(b[8]);
    printf(" ");
    print_32b(b[7]);
    printf(" ");
    print_32b(b[6]);
    printf(" ");
    print_32b(b[5]);
    printf(" ");
    print_32b(b[4]);
    printf(" ");
    print_32b(b[3]);
    printf(" ");
    print_32b(b[2]);
    printf(" ");
    print_32b(b[1]);
    printf(" ");
    print_32b(b[0]);
    printf("\n");


    if ( (b0[0] != b[0]) || (b0[1] != b[1]) || (b0[2] != b[2]) || (b0[3] != b[3])
      || (b0[4] != b[4]) || (b0[5] != b[5]) || (b0[6] != b[6]) || (b0[7] != b[7])
      || (b0[8] != b[8]) || (b0[9] != b[9]) || (b0[10] != b[10]) || (b0[11] != b[11])
      || (b0[12] != b[12]) || (b0[13] != b[13]) || (b0[14] != b[14]) || (b0[15] != b[15]))
    {
      printf(" <- FAIL");
      fail_count++;
    }

    printf("\n");
    printf("\n");
  }
}

int
main( void )
{
  test32();
  test64();
  test128();
  test256();

  printf("========================================================\n");
  printf("==== RESULT                                         ====\n");
  printf("========================================================\n");

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

