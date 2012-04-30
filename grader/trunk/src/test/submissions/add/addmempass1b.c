/*
LANG: C
*/
#include <stdio.h>
#include <stdlib.h>

main()
{
  int a,b;
  int *buf = malloc(10000000);
  scanf("%d %d",&a,&b);
  printf("%d\n",a+b);
  free(buf);
}
