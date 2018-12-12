#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "stack.h"
#include "parser.h"
#include "eval.h"
#include "M_ex.h"
#define arrsize(arr) (sizeof(arr) / sizeof(arr[0]))

void test_parser(char **exps, int expc)
{
  for (int i = 0; i < expc; i++) {
    char *exp = exps[i];
    puts(exp);
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
}

void test_eval(char **exps, int expc)
{
  for (int i = 0; i < expc; i++) {
    struct bind_stack h;
    init_bind_stack(&h, 16);
    struct parsed_exp parsed = parse(exps[i]);
    print_exp(parsed.exp);
    printf("%d\n", eval_bool(parsed.exp, &M, &h));
    free_exp(parsed.exp);
    free(parsed.toks);
    destruct_bind_stack(&h);
  }
}

int main(int argc, char **argv)
{
  char *pexps[5] = {"Ax(!f(x)>g(x)|h(x,Constant))",
                    "Ax(x = x & !!x=x)",
                    "f(",
                    "a.",
                    "a>>"};
  char *eexps[3] = {"Ex(bought(Daddy, x))",
                    "Ax(x=x)",
                    "Ax(bought(Daddy, x)|bought(x, Daddy))"};
  test_parser(pexps, arrsize(pexps));
  test_eval(eexps, arrsize(eexps));
}
