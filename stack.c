/* Generic stack
 * see stack.h for examples
*/

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#define DEFINE_STACK(TYPE, STACKNAME) \
void init_ ## STACKNAME(struct STACKNAME *stack, int size) \
{ \
  stack->size = size; \
  stack->arr = malloc(size * sizeof(TYPE)); \
  stack->top = -1; \
} \
 \
void push_ ## STACKNAME(struct STACKNAME *stack, TYPE obj) \
{ \
  stack->top++; \
  if(stack->top >= stack->size) { \
    puts("Stack Overflow"); \
    exit(1); \
  } \
  stack->arr[stack->top] = obj; \
} \
 \
TYPE pop_ ## STACKNAME(struct STACKNAME *stack) \
{ \
  if(stack->top < 0) { \
    puts("Stack Underflow"); \
    exit(1); \
  } \
  return stack->arr[stack->top--]; \
} \
TYPE gettop_ ## STACKNAME(struct STACKNAME *stack) \
{ \
  if(stack->top < 0) { \
    puts("Trying to gettop from empty stack"); \
    exit(1); \
  } \
  return stack->arr[stack->top]; \
}
// TODO: destructor

// Define needed stack types here
DEFINE_STACK(struct Exp*, exp_p_stack)
DEFINE_STACK(char*, str_stack)
