#include <stdio.h>

int main()
{
  const char *s = "hello";
  double *d = NULL;
  double dd = 3.14;
  int *p = d;

  p = &dd;
  p = s;

  printf("%s\n", s);
  printf("%p\n", p);

}
