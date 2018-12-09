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
  puts(ex2);
  parse(ex2);
  puts(ex3);
  parse(ex3);
  char in[256];
  scanf("%[^\n]s", in);
  parse(in);
}
