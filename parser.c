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

char precedence(char c) {
  switch (c) {
    case '=': return 8;
    case 'A': return 7;
    case 'E': return 7;
    case '!': return 6;
    case '&': return 5;
    case '|': return 4;
    case '>': return 3;
    case '-': return 2;
    case ',': return 1;
    case '(': return 0;
    default : return 100;
  }
}

void parseop(char *op, struct exp_p_stack *args)
{
  struct Exp *parsed = malloc(sizeof(struct Exp));
  parsed->kind = *op;
  struct Exp *argtop = pop_exp_p_stack(args);
  switch (*op) {
    case 'A':
    case 'E':
      parsed->q_var = op + 1;
      parsed->q_arg = argtop;
      break;
    case '!':
      parsed->n_arg = argtop;
      break;
    case '&':
    case '|':
    case '>':  
    case '-':
      parsed->c_arg1 = pop_exp_p_stack(args);
      parsed->c_arg2 = argtop;
      break;
    case '=':
      parsed->e_arg1 = pop_exp_p_stack(args);
      parsed->e_arg2 = argtop;
      break;
    case ',':
      break; //TODO
    case '(':
      puts("Parse error: unmatched (");
      exit(1);
    default:
      break; //TODO
  }
  push_exp_p_stack(args, parsed);
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
      for(char *op; ops.top >= 0 
                    && precedence(*(op = pop_str_stack(&ops))) 
                       > precedence(*tok); )
        parseop(op, &args);
      push_str_stack(&ops, tok);
    } else if (*tok == ')') {
      for(char *op; ops.top >= 0 
                    && *(op = pop_str_stack(&ops)) != '('; )
        parseop(op, &args);
    } else if (isalnum(*tok) && *next(tok) == '(') {
      //TODO func
    } else if (isupper(*tok) || isdigit(*tok)) {
      struct Exp *exp = malloc(sizeof(exp));
      exp->kind = 'c';
      exp->vc_name = tok;
      push_exp_p_stack(&args, exp);
    } else if (islower(*tok)) {
      struct Exp *exp = malloc(sizeof(exp));
      exp->kind = 'v';
      exp->vc_name = tok;
      push_exp_p_stack(&args, exp);
    } else {
      printf("Parse error: unexpected token '%s'", tok);
      exit(1);
    }
  }

  return out;
}
