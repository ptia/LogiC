#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "logic.h"
#include "stack.h"
#include "parser.h"

#define MAX_ARITY 8

#define throw(err_code) do {perrno = err_code; goto finally;} while (0)
#define try(expr) do {expr; if (perrno) goto finally;} while (0)

char *next(char *str)
{
  while(*str)
    str++;
  return ++str;
}

char *tokenise(const char *str)
{
  char *toks = malloc((2 * strlen(str) + 1) * sizeof(char));
  int j = 0;
  for (int i = 0; str[i]; ) {
    if (isspace(str[i])) {
      i++;
      continue;
    } else if (strchr("!&|>-=,()", str[i])) {
      toks[j++] = str[i++];
    } else if (isalnum(str[i])) {
      while (isalnum(str[i]))
        toks[j++] = str[i++];
    } else {
      free(toks);
      perrno = UNEXPECTED_CHAR;
      return NULL;
    }
    toks[j++] = '\0';
  }
  toks[j] = EOF;
  return toks;
}

char prec(char c) {
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
    default: assert(false);
  }
}

void parse_op(char *op, struct exp_p_stack *args)
{
  if(args->top < 0) {
    perrno = ARGS_ERROR;
    return;
  }

  struct Exp *parsed = malloc(sizeof(struct Exp));
  parsed->kind = *op;
  struct Exp *argtop = pop_exp_p(args);
  switch (*op) {
    case 'A':
    case 'E':
      parsed->q_var = op + 1;
      if(!*(parsed->q_var))
        throw(UNNAMED_QUANTIFIER);
      parsed->q_arg = argtop;
      break;
    case '!':
      parsed->n_arg = argtop;
      break;
    case '&':
    case '|':
    case '>':  
    case '-':
      if(args->top < 0)
        throw(ARGS_ERROR);
      parsed->c_arg1 = pop_exp_p(args);
      parsed->c_arg2 = argtop;
      break;
    case '=':
      if(args->top < 0)
        throw(ARGS_ERROR);
      parsed->e_arg1 = pop_exp_p(args);
      parsed->e_arg2 = argtop;
      break;
    case '(':
      throw(UNMATCHED_OPEN_BRACKET);
    default: assert(false);
  }

finally:
  if (perrno) {
    free(parsed);
    free(argtop);
    return;
  }
  push_exp_p(args, parsed);
}

struct Exp *parse_frcv(char **toks, bool nested)
{ 
  if (!isalnum(**toks)) {
    perrno = UNEXPECTED_TOKEN;
    return NULL;
  }

  struct Exp *parsed = malloc(sizeof(struct Exp));

  /* fun, rel */
  if (*next(*toks) == '(') {
    parsed->kind = nested ? 'f' : 'r';
    parsed->rf_name = *toks;
    parsed->rf_args = calloc((MAX_ARITY + 1), sizeof(struct Exp *));
    *toks = next(*toks);
    for (int i = 0; (i == 0 && **toks == '(') || **toks == ','; i++) {
      if (i > MAX_ARITY)
        throw(ARITY_TOO_BIG);
      *toks = next(*toks);
      try(parsed->rf_args[i] = parse_frcv(toks, true));
      *toks = next(*toks);
    }
    if (**toks != ')')
      throw(UNMATCHED_OPEN_BRACKET);
  } /* const, var */
  else {
    parsed->kind = islower(**toks) ? 'v' : 'c';
    parsed->vc_name = *toks;
  }

finally:
  if (perrno) {
    free_exp(parsed);
    parsed = NULL;
  }
  return parsed;
}

struct Exp *parse_toks(char *toks) 
{
  struct str_stack ops; init_str_stack(&ops, 16);
  struct exp_p_stack args; init_exp_p_stack(&args, 16);

  for (char *tok = toks; *tok != EOF; tok = next(tok)) {
    /* open bracket */
    if (*tok == '(') {
      push_str(&ops, tok);
    } /* AE!&|>-= */
    else if (strchr("AE!&|>-=", *tok)) {
      while (ops.top >= 0 && prec(*gettop_str(&ops)) > prec(*tok))
        try(parse_op(pop_str(&ops), &args));
      push_str(&ops, tok);
    } /* closed bracket */
    else if (*tok == ')') {
      while(ops.top >= 0 && *gettop_str(&ops) != '(')
        try(parse_op(pop_str(&ops), &args));
      if (ops.top < 0)
        throw(UNMATCHED_CLOSED_BRACKET);
      pop_str(&ops); /* pop the open bracket */
    } /* fun, rel, const, var */
    else if (isalnum(*tok)) {
      struct Exp *exp;
      try(exp = parse_frcv(&tok, false));
      push_exp_p(&args, exp);
    }
    else if (*tok == ',') {
      throw(TOP_LEVEL_COMMA);
    } else {
      assert(false);
    }
  }

  while (ops.top >= 0)
    parse_op(pop_str(&ops), &args);
  if (args.top != 0)
    throw(ARGS_ERROR);
  

finally:
  ; struct Exp *out = perrno ? NULL : pop_exp_p(&args);
  while (args.top >= 0)
    free_exp(pop_exp_p(&args));
  destruct_str_stack(&ops);
  destruct_exp_p_stack(&args);
  return out;
}

struct parsed_exp parse(const char *str)
{
  perrno = 0;
  char *toks;
  struct Exp *parsed = NULL;
  try(toks = tokenise(str));
  try(parsed = parse_toks(toks));

finally:
  if (perrno) {
    free(toks);
    free_exp(parsed);
    toks = NULL;
    parsed = NULL;
  }
  struct parsed_exp out = {parsed, toks};
  return out;
}
