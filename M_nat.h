#include <stdbool.h>
#include "logic.h"

#define arrsize(arr) (sizeof(arr) / sizeof(arr[0]))

#define MAXI 100

bool lt(int *args)
{
  return args[0] < args[1];
}
int add(int *args)
{
  return args[0] + args[1];
}
int mul(int *args)
{
  return args[0] * args[1];
}

obj_t objs[MAXI];
struct binding consts[3] = {{"0", 0}, {"1", 1}, {"2", 2}};
struct rel rels[1] = {{"lt", lt}};
struct func funcs[2] = {{"add", add}, {"mul", mul}};

struct model M = {objs, arrsize(objs),
                  consts, arrsize(consts),
                  rels, arrsize(rels),
                  funcs, arrsize(funcs)};

void init_M()
{
  for(int i = 0; i < MAXI; i++)
    objs[i] = i;
}
