/* Generic stack
 * !! CAREFUL OF DUPLICATES !!
 *
 * ex.:
 * #import "stack.h"
 * DEFINE_STACK(int, int_stack, 256)
 * DEFINE_STACK(int*, intp_stack, 32)
 * struct intp_stack m_stack;
*/

#pragma once

#include <stdlib.h>
#include <stdio.h>

#define DEFINE_STACK(TYPE, STACKNAME, SIZE) \
struct STACKNAME { \
  TYPE arr[SIZE]; \
  int top; \
}; \
 \
void init_ ## STACKNAME(struct STACKNAME *stack) \
{ \
  stack->top = -1; \
} \
 \
void push_ ## STACKNAME(struct STACKNAME *stack, TYPE obj) \
{ \
  stack->top++; \
  if(stack->top >= SIZE) { \
    puts("Stack Overflow!"); \
    exit(1); \
  } \
  stack->arr[stack->top] = obj; \
} \
 \
TYPE pop_ ## STACKNAME(struct STACKNAME *stack) \
{ \
  if(stack->top < 0) { \
    puts("Stack Underflow!"); \
    exit(1); \
  } \
  return stack->arr[stack->top--]; \
}
