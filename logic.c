#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <assert.h>
#include "logic.h"

void free_exp(struct Exp *exp)
{
  if (exp == NULL)
    return;
  switch(exp->kind) {
    case 'A':
    case 'E':
      free_exp(exp->q_arg);
      break;
    case '!':
      free_exp(exp->n_arg);
      break;
    case '&':
    case '|':
    case '>':
    case '-':
      free_exp(exp->c_arg1);
      free_exp(exp->c_arg2);
      break;
    case '=':
      free_exp(exp->e_arg1);
      free_exp(exp->e_arg2);
      break;
    case 'f':
    case 'r':
      for (int i = 0; exp->rf_args[i] != NULL; i++)
        free_exp(exp->rf_args[i]);
      free(exp->rf_args);
  }
  free(exp);
}

void printsym(char c)
{
  switch (c) {
    case 'A': printf("∀"); break;
    case 'E': printf("∃"); break;
    case '&': printf("∧"); break;
    case '|': printf("∨"); break;
    case '>': printf("→"); break;
    case '-': printf("↔"); break;
    default: assert(0);
  }
}
void print_exp_helper(const struct Exp *exp) 
{
  switch (exp->kind) {
    case 'A':
    case 'E':
      printsym(exp->kind);
      printf("%s(", exp->q_var);
      print_exp_helper(exp->q_arg);
      putchar(')');
      break;
    case '!':
      printf("¬(");
      print_exp_helper(exp->n_arg);
      putchar(')');
      break;
    case '&':
    case '|':
    case '>':
    case '-':
      putchar('(');
      print_exp_helper(exp->c_arg1);
      putchar(')');
      printsym(exp->kind);
      putchar('(');
      print_exp_helper(exp->c_arg2);
      putchar(')');
      break;
    case '=':
      print_exp_helper(exp->c_arg1);
      printf("=");
      print_exp_helper(exp->c_arg2);
      break;
    case 'f':
    case 'r':
      printf("%s(", exp->rf_name);
      for (int i = 0; exp->rf_args[i] != NULL; i++) {
        if (i)
          putchar(',');
        print_exp_helper(exp->rf_args[i]);
      }
      putchar(')');
      break;
    case 'c':
    case 'v':
      printf("%s", exp->vc_name);
      break;
  }
}

void print_exp(const struct Exp *exp)
{
  print_exp_helper(exp);
  putchar('\n');
}
