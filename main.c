#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "parser.h"

void parse_print_free(char *exp)
{
  struct parsed_exp parsed;
  parsed = parse(exp);
  if (perrno)
    printf("perrno %d\n", perrno);
  else {
    print_exp(parsed.exp);
    free_exp(parsed.exp);
    free(parsed.toks);
  }
}

int main(int argc, char **argv)
{
  char *exps[] = {"Ax(!f(x)>g(x)|h(x,Constant))",
                  "Ax(x = x & !!x=x)",
                  "f(",
                  "a>>",
                  "a.",
                  NULL};
  for (int i = 0; exps[i] != NULL; i++) {
    puts(exps[i]);
    parse_print_free(exps[i]);
  }
}
