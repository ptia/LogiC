#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "logic.h"
#include "stack.h"

#define MAX_ARITY 8

char *next(char *str)
{
  while(*str)
    str++;
  return ++str;
}

char *syms = "AE!&|>-=";
char *seps = "!&|>-=,()";

char *tokenise(char *str)
{
  char *toks = malloc((2 * strlen(str) + 1) * sizeof(char));
  int j = 0;
  for (int i = 0; str[i]; ) {
    if (isspace(str[i])) {
      i++;
      continue;
    } else if (strchr(seps, str[i])) {
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
  toks[j] = EOF;
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
    case '(': return 0;
    default: exit(1);
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
    case '(':
      puts("Parse error: unmatched (");
      exit(1);
    default:
      printf("Parse error: unexpected operator %s", op);
      exit(1);
  }
  push_exp_p_stack(args, parsed);
}

struct Exp *parse_toks(char **toks) 
{
  struct Exp *out;
  struct str_stack ops; init_str_stack(&ops, 16);
  struct exp_p_stack args; init_exp_p_stack(&args, 16);

  for (; **toks != EOF && **toks != ','; *toks = next(*toks)) {
    if (**toks == '(') {
      push_str_stack(&ops, *toks);
    }
    else if (strchr(syms, **toks)) {
      while (ops.top >= 0 
             && precedence(*gettop_str_stack(&ops)) > precedence(**toks)) {
        char *op = pop_str_stack(&ops);
        parseop(op, &args);
      }
      push_str_stack(&ops, *toks);
    }
    else if (**toks == ')') {
      bool matched = false;
      for (char *op; ops.top >= 0  && *(op = pop_str_stack(&ops)) != '('; ) {
        matched = true;
        parseop(op, &args);
      }
      if (!matched)
        goto end;
    }
    else if (isalnum(**toks) && *next(*toks) == '(') {
      struct Exp *exp = malloc(sizeof(struct Exp));
      exp->kind = 'f';
      exp->rf_name = *toks;
      exp->rf_args = malloc(MAX_ARITY * sizeof(struct Exp *));
      *toks = next(*toks);
      for (int i = 0; **toks != ')'; i++) {
        *toks = next(*toks);
        exp->rf_args[i] = parse_toks(toks);
      }
      push_exp_p_stack(&args, exp);
    }
    else if (isupper(**toks) || isdigit(**toks)) {
      struct Exp *exp = malloc(sizeof(struct Exp));
      exp->kind = 'c';
      exp->vc_name = *toks;
      push_exp_p_stack(&args, exp);
    }
    else if (islower(**toks)) {
      struct Exp *exp = malloc(sizeof(struct Exp));
      exp->kind = 'v';
      exp->vc_name = *toks;
      push_exp_p_stack(&args, exp);
    }
    else {
      printf("Parse error: unexpected token '%s'", *toks);
      exit(1);
    }
  }

  for (char *op; ops.top >= 0; ) {
    op = pop_str_stack(&ops);
    parseop(op, &args);
  }

end:
  out = pop_exp_p_stack(&args);
  destruct_str_stack(&ops);
  destruct_exp_p_stack(&args);
  return out;
}

struct Exp *parse(char *str)
{
  char *toks = tokenise(str);
  return parse_toks(&toks);
}
