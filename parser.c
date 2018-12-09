#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "logic.h"
#include "stack.h"
#include "parser.h"

#define MAX_ARITY 8

#define throw(code) do {perrno = code; goto err_end;} while (0)

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
    default: 
      perror("prec(): illegal argument\n");
      exit(1);
  }
}

void parseop(char *op, struct exp_p_stack *args)
{
  struct Exp *parsed = malloc(sizeof(struct Exp));
  parsed->kind = *op;
  if(args->top < 0) {
    perrno = ARGS_ERROR;
    goto end;
  }
  struct Exp *argtop = pop_exp_p(args);
  switch (*op) {
    case 'A':
    case 'E':
      parsed->q_var = op + 1;
      if(!(parsed->q_var)) {
        perrno = UNNAMED_QUANTIFIER;
        break;
      }
      parsed->q_arg = argtop;
      break;
    case '!':
      parsed->n_arg = argtop;
      break;
    case '&':
    case '|':
    case '>':  
    case '-':
      if(args->top < 0) {
        perrno = ARGS_ERROR;
        break;
      }
      parsed->c_arg1 = pop_exp_p(args);
      parsed->c_arg2 = argtop;
      break;
    case '=':
      if(args->top < 0) {
        perrno = ARGS_ERROR;
        break;
      }
      parsed->e_arg1 = pop_exp_p(args);
      parsed->e_arg2 = argtop;
      break;
    case '(':
      perrno = UNMATCHED_OPEN_BRACKET;
      break;
    default:
      perror("parseop(): illegal argument");
      exit(1);
  }
end:
  if (perrno) {
    free(parsed);
    return;
  }
  push_exp_p(args, parsed);
}

struct Exp *parse_toks(char **toks, bool nested) 
{
  struct str_stack ops; init_str_stack(&ops, 16);
  struct exp_p_stack args; init_exp_p_stack(&args, 16);

  for (; **toks != EOF && **toks != ','; *toks = next(*toks)) {
    /* open bracket */
    if (**toks == '(') {
      push_str(&ops, *toks);
    } /* AE!&|>-= */
    else if (strchr("AE!&|>-=", **toks)) {
      while (ops.top >= 0 && prec(*gettop_str(&ops)) > prec(**toks)) {
        char *op = pop_str(&ops);
        parseop(op, &args);
        if (perrno)
          goto err_end;
      }
      push_str(&ops, *toks);
    } /* closed bracket */
    else if (**toks == ')') {
      bool matched = false;
      for (char *op; ops.top >= 0  && *(op = pop_str(&ops)) != '('; ) {
        matched = true;
        parseop(op, &args);
        if (perrno)
          goto err_end;
      }
      if (!matched) {
        if (!nested)
          throw(UNMATCHED_OPEN_BRACKET);
        break;
      }
    } /* fun, rel */
    else if (isalnum(**toks) && *next(*toks) == '(') {
      struct Exp *exp = malloc(sizeof(struct Exp));
      exp->kind = nested ? 'f' : 'r';
      exp->rf_name = *toks;
      exp->rf_args = malloc(MAX_ARITY * sizeof(struct Exp *));
      *toks = next(*toks);
      for (int i = 0; **toks != ')'; i++) {
        *toks = next(*toks);
        exp->rf_args[i] = parse_toks(toks, true);
        if (perrno) {
          free(exp);
          goto err_end;
        }
      }
      push_exp_p(&args, exp);
    } /* const, var */
    else if (isalnum(**toks)) {
      struct Exp *exp = malloc(sizeof(struct Exp));
      exp->kind = islower(**toks) ? 'v' : 'c';
      exp->vc_name = *toks;
      push_exp_p(&args, exp);
    }
    else {
      perror("parse_toks(): unexpected token");
    }
  }
  if (!nested && **toks == ',')
    throw(TOP_LEVEL_COMMA);
  if (nested && **toks == EOF)
    throw(UNMATCHED_OPEN_BRACKET);

  while (ops.top >= 0)
    parseop(pop_str(&ops), &args);
  if (args.top != 0)
    throw(ARGS_ERROR);

err_end:
  ; struct Exp *out = perrno ? NULL : pop_exp_p(&args);
  destruct_str_stack(&ops);
  destruct_exp_p_stack(&args);
  return out;
}

struct Exp *parse(const char *str)
{
  perrno = 0;
  char *toks = tokenise(str);
  if (perrno)
    return NULL;
  return parse_toks(&toks, false);
}
