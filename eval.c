#include <string.h>
#include <assert.h>
#include "stack.h"
#include "eval.h"

bool (*lookup_r(struct rel *rels, int relc, char *key)) (obj_t*)
{
  for (int i = 0; i < relc; i++)
    if (!strcmp(rels[i].name, key))
      return rels[i].func;
  assert(0);
}

obj_t (*lookup_f(struct func *funcs, int funcc, char *key)) (obj_t*)
{
  for (int i = 0; i < funcc; i++)
    if (!strcmp(funcs[i].name, key))
      return funcs[i].func;
  assert(0);
}

obj_t lookup_b(struct binding *bs, int bc, char *key)
{
  /* start from top of assignment stack arrays */
  for (int i = bc - 1; i >= 0; i--)
    if (!strcmp(bs[i].key, key))
      return bs[i].obj;
  assert(0);
}

obj_t eval_obj(struct Exp *exp, struct model *M, struct bind_stack *h)
{
  switch (exp->kind) {
    case 'c':
      return lookup_b(M->consts, M->constc, exp->vc_name);
    case 'v':
      return lookup_b(h->arr, h->top + 1, exp->vc_name);
    case 'f':
      ; obj_t args[MAX_ARITY];
      for (int i = 0; exp->rf_args[i]; i++)
        args[i] = eval_obj(exp->rf_args[i], M, h);
      return (*lookup_f(M->funcs, M->funcc, exp->rf_name)) (args);
    default:
      assert(0);
  }
}

bool eval_bool(struct Exp *exp, struct model *M, struct bind_stack *h)
{
  switch (exp->kind) {
    case 'A':
      for (int i = 0; i < M->objc; i++) {
        struct binding b = {exp->q_var, M->objs[i]};
        push_bind(h, b);
        if (!eval_bool(exp->q_arg, M, h))
          return false;
        pop_bind(h);
      }
      return true;
    case 'E':
      for (int i = 0; i < M->objc; i++) {
        struct binding b = {exp->q_var, M->objs[i]};
        push_bind(h, b);
        if (eval_bool(exp->q_arg, M, h))
          return true;
        pop_bind(h);
      }
      return false;
    case '!':
      return !eval_bool(exp->n_arg, M, h);
    case '&':
      return eval_bool(exp->c_arg1, M, h) && eval_bool(exp->c_arg2, M, h);
    case '|':
      return eval_bool(exp->c_arg1, M, h) || eval_bool(exp->c_arg2, M, h);
    case '>':
      return !eval_bool(exp->c_arg1, M, h) || eval_bool(exp->c_arg2, M, h);
    case '-':
      return eval_bool(exp->c_arg1, M, h) == eval_bool(exp->c_arg2, M, h);
    case 'r':
      ; obj_t args[MAX_ARITY];
      for (int i = 0; exp->rf_args[i]; i++)
        args[i] = eval_obj(exp->rf_args[i], M, h);
      return (*lookup_r(M->rels, M->relc, exp->rf_name)) (args);
    case '=':
      return eval_obj(exp->e_arg1, M, h) == eval_obj(exp->e_arg2, M, h);
    default:
      assert(0);
  }
}

bool eval(struct Exp *exp, struct model *M, struct bind_stack *h)
{
  eerrno = 0;

}
