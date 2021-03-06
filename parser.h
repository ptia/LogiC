#pragma once
#include "logic.h"

#define UNEXPECTED_CHAR 1
#define UNEXPECTED_TOKEN 2
#define UNMATCHED_CLOSED_BRACKET 3
#define UNMATCHED_OPEN_BRACKET 4
#define TOP_LEVEL_COMMA 5
#define ARGS_ERROR 6
#define UNNAMED_QUANTIFIER 7
#define ARITY_TOO_BIG 8
int perrno;

struct parsed_exp {
  struct Exp *exp;
  char *toks;
};
struct parsed_exp parse(const char *str);
