#pragma once

#include <stdbool.h>
#include <setjmp.h>
#include "logic.h"
#include "stack.h"

#define UNEXPECTED_EXP 2
#define NO_BIND 3
#define NO_REL 4
#define NO_FUNC 5

jmp_buf eval_eh;

bool eval_bool(struct Exp *exp, struct model *M, struct bind_stack *h);
