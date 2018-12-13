#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include "logic.h"
#include "stack.h"
#include "parser.h"
#include "eval.h"
#include "M_nat.h"

//try this: goldbach's conjecture
char *goldbachs = "Ax(lt(2,x) & Eu(x=mul(2,u)) > Ep1 Ep2( !Eu(!(u=p1)&!(u=1)&Ev(p1=mul(u,v))) & !(p1=1) &!Eu(!(u=p2)&!(u=1)&Ev(p2=mul(u,v))) & !(p2=1) & x = add(p1,p2)))";
int main()
{
  init_M();
  char in[256];

  while (1) {
    struct bind_stack h;
    init_bind_stack(&h, 32);
    
    fgets(in, 256, stdin);
    if (!strcmp(in, "q\n"))
      exit(0);

    struct parsed_exp parsed = parse(in);
    if (perrno) {
      printf("Parse error %d\n", perrno);
      destruct_bind_stack(&h);
      continue;
    }
    print_exp(parsed.exp);

    int eerno = setjmp(eval_eh);
    if (!eerno)
      puts(eval_bool(parsed.exp, &M, &h) ? "true" : "false");
    else
      printf("eval error %d\n", eerno);

    free_exp(parsed.exp);
    free(parsed.toks);
    destruct_bind_stack(&h);
  }
}
