#pragma once

enum Exp_t {
  e_and, e_or, e_arr, e_darr,
  e_fa, e_te,
  e_not,
  e_const, e_var,
  e_rel, e_func,
  e_eq
};

struct Exp {
  enum Exp_t type;
  union {
    struct { char *q_var; struct Exp *q_arg; };      // quant
    struct { struct Exp *c_arg1, *c_arg2; };         // conn
    struct { struct Exp *n_arg; };                   // not
    struct { char *vc_name; };                       // var, const
    struct { char *rf_name; struct Exp **rf_args; }; // rel, func
    struct { struct Exp *e_arg1, e_arg2; };          // eq
  };
};
