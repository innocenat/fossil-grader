/*
LANG: C++
*/
#include <stdio.h>

main()
{
  int a,b;
  scanf("%d %d",&a,&b);
  int sum = b;
  for(int i=0; i<a; i++)
    sum ++;
  printf("%d\n",sum);
}
