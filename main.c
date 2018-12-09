#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "parser.h"

int main(int argc, char **argv) {
  char *ex1 = "Ax(!f(x)>g(x)|h(x,Constant))";
  char *ex2 = "Ax(x = x & !!x=x)";
  char *ex3 = "a - x = x";

  puts(ex1);
  parse(ex1);
  printf("perrno %d\n", perrno);

  puts(ex2);
  parse(ex2);
  printf("perrno %d\n", perrno);

  puts(ex3);
  parse(ex3);
  printf("perrno %d\n", perrno);

  char in[256];
  scanf("%[^\n]s", in);
  parse(in);
  printf("perrno %d\n", perrno);
}
