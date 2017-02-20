#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
  uint32_t a[ 2 ];
  uint32_t b[ 2 ];

  a[0] = 0xffffffff;
  a[1] = 0x0f;

  printf("a 0x%08x 0x%08x ",a[1],a[0]);
  print_32b(a[1]);
  printf(" ");
  print_32b(a[0]);
  printf("\n");

  printf("\n");

  a[0] = 0xffffffff;
  a[1] = 0x01;
  b[0] = 0;
  b[1] = 0;

  encode( a,b,32 );

  printf("b 0x%08x 0x%08x ",b[1], b[0]);
  print_32b(b[1]);
  printf(" ");
  print_32b(b[0]);
  printf("\n");

  a[0] = 0xffffffff;
  a[1] = 0x02;
  b[0] = 0;
  b[1] = 0;

  encode( a,b,32 );

  printf("b 0x%08x 0x%08x ",b[1], b[0]);
  print_32b(b[1]);
  printf(" ");
  print_32b(b[0]);
  printf("\n");

  a[0] = 0xffffffff;
  a[1] = 0x04;
  b[0] = 0;
  b[1] = 0;

  encode( a,b,32 );

  printf("b 0x%08x 0x%08x ",b[1], b[0]);
  print_32b(b[1]);
  printf(" ");
  print_32b(b[0]);
  printf("\n");

  a[0] = 0xffffffff;
  a[1] = 0x08;
  b[0] = 0;
  b[1] = 0;

  encode( a,b,32 );

  printf("b 0x%08x 0x%08x ",b[1], b[0]);
  print_32b(b[1]);
  printf(" ");
  print_32b(b[0]);
  printf("\n");

  printf("=\n");

  a[0] = 0xffffffff;
  a[1] = 0x0f;
  b[0] = 0;
  b[1] = 0;

  encode( a,b,32 );

  printf("b 0x%08x 0x%08x ",b[1], b[0]);
  print_32b(b[1]);
  printf(" ");
  print_32b(b[0]);
  printf("\n");

  return 0;
}

