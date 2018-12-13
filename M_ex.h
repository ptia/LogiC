#include <stdlib.h>
#include "logic.h"

#define arrsize(arr) (sizeof(arr) / sizeof(arr[0]))

obj_t tony = 0;
obj_t clyde = 1;
obj_t r308 = 2;
bool bought(obj_t *args)
{
  if ((args[0] == tony && args[1] == r308)
     || (args[0] == r308 && args[1] == clyde)
     || (args[0] == clyde && args[1] == tony)
     || (args[0] == tony && args[1] == tony))
    return true;
  return false;
}
obj_t objs[3] = {0, 1, 2};
struct binding consts[3] = {{"Daddy", 0}, {"Clyde", 1}, {"room", 2}};
struct rel rels[1] = {{"bought", bought}};
struct model M = {objs, arrsize(objs),
                  consts, arrsize(consts),
                  rels, arrsize(rels),
                  NULL, 0};
