/*
LANG: C
*/
#include <stdio.h>
#include <stdlib.h>

main()
{
  int a,b;
  int *buf = malloc(10000000);
  a = 10;
  b = 20;
  int i;
  for(i=0; i<100000000; i++)
    a += b;
  scanf("%d %d",&a,&b);
  printf("%d\n",a+b);
  free(buf);
}
