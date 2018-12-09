/* Generic stack
 *
 * stack.c: (at the end)
 * DEFINE_STACK(int, int_stack, 256)
 * DEFINE_STACK(int*, intp_stack, 32)
 *
 * stack.h: (at the end)
 * DEFINE_STACK_HEADER(struct Exp*, exp_p_stack, 32)
 * DEFINE_STACK_HEADER(char*, str_stack, 32)
 *
 * somewhere_else.c:
 * #import stack.h
 * struct intp_stack m_stack;
*/

#pragma once

#define DEFINE_STACK_HEADER(TYPE, TYPENAME) \
struct TYPENAME ## _stack { \
  int size; \
  TYPE *arr; \
  int top; \
}; \
 \
void init_ ## TYPENAME ## _stack(struct TYPENAME ## _stack *stack, int size); \
void destruct_ ## TYPENAME ## _stack(struct TYPENAME ## _stack *stack); \
void push_ ## TYPENAME(struct TYPENAME ## _stack *stack, TYPE obj); \
TYPE pop_ ## TYPENAME(struct TYPENAME ## _stack *stack); \
TYPE gettop_ ##TYPENAME(struct TYPENAME ## _stack *stack);

DEFINE_STACK_HEADER(struct Exp*, exp_p)
DEFINE_STACK_HEADER(char*, str)
