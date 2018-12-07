#pragma once

struct Exp {
  char kind;
  union {
    struct { char *q_var; struct Exp *q_arg; };      // quant
    struct { struct Exp *c_arg1, *c_arg2; };         // conn
    struct { struct Exp *n_arg; };                   // not
    struct { char *vc_name; };                       // var, const
    struct { char *rf_name; struct Exp **rf_args; }; // rel, func
    struct { struct Exp *e_arg1, e_arg2; };          // eq
  };
};
