#include <stdlib.h>
#include <stdio.h>
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
    init_bind_stack(&h, 256);
    
    fgets(in, 256, stdin);
    struct parsed_exp parsed = parse(in);

    if (perrno) {
      printf("Parse error %d\n", perrno);
      continue;
    }
    
    print_exp(parsed.exp);
    printf("%d\n", eval_bool(parsed.exp, &M, &h));

    free_exp(parsed.exp);
    free(parsed.toks);
    destruct_bind_stack(&h);
  }
}
