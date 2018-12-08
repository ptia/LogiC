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

#define DEFINE_STACK_HEADER(TYPE, STACKNAME) \
struct STACKNAME { \
  int size; \
  TYPE *arr; \
  int top; \
}; \
 \
void init_ ## STACKNAME(struct STACKNAME *stack, int size); \
void destruct_ ## STACKNAME(struct STACKNAME *stack); \
void push_ ## STACKNAME(struct STACKNAME *stack, TYPE obj); \
TYPE pop_ ## STACKNAME(struct STACKNAME *stack); \
TYPE gettop_ ##STACKNAME(struct STACKNAME *stack);

DEFINE_STACK_HEADER(struct Exp*, exp_p_stack)
DEFINE_STACK_HEADER(char*, str_stack)
