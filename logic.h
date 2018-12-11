#pragma once

#include <stdbool.h>
#include "stack.h"

#define MAX_ARITY 8

/* Syntax */
struct Exp {
  char kind;
  union {
    struct { char *q_var; struct Exp *q_arg; };      // quant
    struct { struct Exp *c_arg1, *c_arg2; };         // conn
    struct { struct Exp *n_arg; };                   // not
    struct { char *vc_name; };                       // var, const
    struct { char *rf_name; struct Exp **rf_args; }; // rel, func
    struct { struct Exp *e_arg1, *e_arg2; };         // eq
  };
};
void print_exp(const struct Exp *exp);
void free_exp(struct Exp *exo);

/* Semantics */
typedef int obj_t; /* Any type will do here (enums are useful)! */
struct binding {char *key; obj_t obj; };
struct rel {char *name; bool (*func) (obj_t*); };
struct func {char *name; obj_t (*func) (obj_t*); };
struct assignment {struct binding *bs; int bc; };
struct model {
  obj_t *objs; int objc;
  struct binding *consts; int constc;
  struct rel *rels; int relc;
  struct func *funcs; int funcc;
};
