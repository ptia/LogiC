#pragma once

#include <stdbool.h>
#include "logic.h"
#include "stack.h"

#define FREE_VARIABLE 1
#define NO_FUNC 2
#define NO_REL 3
#define NO_CONST 4
#define NO_BINDING 5
int eerrno;

bool eval_bool(struct Exp *exp, struct model *M, struct bind_stack *h);
