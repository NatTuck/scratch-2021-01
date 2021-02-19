#ifndef CALC_AST_H
#define CALC_AST_H

#include <stdlib.h>

typedef struct calc_ast {
    char op;
    // Op is either:
    //  one of: +, -, *, / for an operation
    //      or: '=' for value
    struct calc_ast* arg0;
    struct calc_ast* arg1;
    int value;
} calc_ast;

calc_ast* make_ast_value(int vv);
calc_ast* make_ast_op(char op, calc_ast* a0, calc_ast* a1);
void free_ast(calc_ast* ast);
int ast_eval(calc_ast* ast);
void print_ast(calc_ast* ast);

#endif
