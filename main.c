#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "parser.h"

int main(int argc, char **argv) {
  char *ex = "Ax(!f(x)>g(x)|h(x,Constant))";
  parse(ex);
  char in[256];
  scanf("%[^\n]s", in);
  parse(in);
}
