#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "parser.h"

int main(int argc, char **argv) {
  char *ex1 = "Ax(!f(x)>g(x)|h(x,Constant))";
  char *ex2 = "Ax(x = x & !!x=x)";
  parse(ex2);
  /*char in[256];
  scanf("%[^\n]s", in);
  parse(in);*/
}
