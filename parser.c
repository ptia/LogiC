#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "logic.h"
#include "stack.h"
DEFINE_STACK(struct Exp*, exp_p_stack, 32)
DEFINE_STACK(char*, str_stack, 32)

char *next(char *str)
{
  while(*str)
    str++;
  return ++str;
}

char *syms = "!&|>-=,";

char *tokenise(char *str)
{
  char *toks = malloc((2 * strlen(str) + 1) * sizeof(char));
  int j = 0;
  for (int i = 0; str[i]; ) {
    if (isspace(str[i])) {
      i++;
      continue;
    } else if (strchr(syms, str[i]) || strchr("()", str[i])) {
      toks[j++] = str[i++];
    } else if (isalnum(str[i])) {
      while (isalnum(str[i]))
        toks[j++] = str[i++];
    } else {
      printf("Token error: unexpected '%c'", str[i]);
      exit(1);
    }
    toks[j++] = '\0';
  }
  toks[j] = '\0';
  return toks;
}

struct Exp *parse(char *str) 
{
  struct Exp *out;
  char *toks = tokenise(str);

  struct str_stack ops;
  struct exp_p_stack args;
  init_str_stack(&ops);
  init_exp_p_stack(&args);

  for (char *tok = toks; *tok; tok = next(tok)) {
    printf("parsing tok %s\n", tok);
    if (*tok == '(') {
      push_str_stack(&ops, tok);
    } else if (strchr(syms, *tok)) {
      //TODO symbol
    } else if (*tok == ')') {
      //TODO closed bracket
    } else if (isalnum(*tok) && *next(tok) == '(') {
      //TODO func
    } else if (isupper(*tok) || isdigit(*tok)) {
      struct Exp *exp = malloc(sizeof(exp));
      exp->type = e_const;
      exp->vc_name = tok;
      push_exp_p_stack(&args, exp);
    } else if (islower(*tok)) {
      struct Exp *exp = malloc(sizeof(exp));
      exp->type = e_var;
      exp->vc_name = tok;
      push_exp_p_stack(&args, exp);
    } else {
      printf("Parse error: unexpected token '%s'", tok);
      exit(1);
    }
  }

  return out;
}
