#include <stdlib.h>
#include <stdio.h>
#include "logic.h"
#include "stack.h"
#include "parser.h"
#include "eval.h"

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


obj_t tony = 0;
obj_t clyde = 1;
obj_t r308 = 2;
bool bought(obj_t *args)
{
  if ((args[0] == tony && args[1] == r308)
     || (args[0] == r308 && args[1] == clyde)
     || (args[0] == clyde && args[1] == tony)
     || (args[0] == tony && args[1] == tony))
    return true;
  return false;
}

void test_eval(char **exps, int expc)
{
  obj_t objs[3] = {tony, clyde, r308};
  struct rel rels[1] = {{"bought", bought}};
  struct binding consts[3] = {{"Daddy", tony}, {"Clyde", clyde}, "room", r308};
  struct model M;
  M.objs = objs; M.objc = arrsize(objs);
  M.rels = rels; M.relc = arrsize(rels);
  M.funcs = NULL; M.funcc = 0;
  M.consts = consts; M.constc = arrsize(consts);
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
