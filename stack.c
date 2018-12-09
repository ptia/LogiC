/* Generic stack
 * see stack.h for examples
*/

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#define DEFINE_STACK(TYPE, TYPENAME) \
void init_ ## TYPENAME ## _stack(struct TYPENAME ## _stack *stack, int size) \
{ \
  stack->size = size; \
  stack->arr = malloc(size * sizeof(TYPE)); \
  stack->top = -1; \
} \
void destruct_ ## TYPENAME ## _stack(struct TYPENAME ## _stack *stack) \
{ \
  free(stack->arr); \
  stack->arr = NULL; \
} \
void push_ ## TYPENAME(struct TYPENAME ## _stack *stack, TYPE obj) \
{ \
  stack->top++; \
  if(stack->top >= stack->size) { \
    puts("Stack Overflow"); \
    exit(1); \
  } \
  stack->arr[stack->top] = obj; \
} \
 \
TYPE pop_ ## TYPENAME(struct TYPENAME ## _stack *stack) \
{ \
  if(stack->top < 0) { \
    puts("Stack Underflow"); \
    exit(1); \
  } \
  return stack->arr[stack->top--]; \
} \
TYPE gettop_ ## TYPENAME(struct TYPENAME ## _stack *stack) \
{ \
  if(stack->top < 0) { \
    puts("Trying to gettop from empty stack"); \
    exit(1); \
  } \
  return stack->arr[stack->top]; \
}

/* Define needed stack types here
 * Must match stack.h
*/

DEFINE_STACK(struct Exp*, exp_p)
DEFINE_STACK(char*, str)
