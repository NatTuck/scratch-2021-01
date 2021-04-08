#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

calc_ast*
make_ast_value(int vv)
{
    calc_ast* ast = malloc(sizeof(calc_ast));
    ast->op = '=';
    ast->arg0 = 0;
    ast->arg1 = 0;
    ast->value = vv;
    return ast;
}

calc_ast*
make_ast_op(char op, calc_ast* a0, calc_ast* a1)
{
    calc_ast* ast = malloc(sizeof(calc_ast));
    ast->op = op;
    ast->arg0 = a0;
    ast->arg1 = a1;
    return ast;
}

void
free_ast(calc_ast* ast)
{
    if (ast) {
        if (ast->arg0) {
            free(ast->arg0);
        }

        if (ast->arg1) {
            free(ast->arg1);
        }

        free(ast);
    }
}

int
ast_eval(calc_ast* ast)
{
    switch (ast->op) {
    case '=':
        return ast->value;
    case '+':
        return ast_eval(ast->arg0) + ast_eval(ast->arg1);
    case '-':
        return ast_eval(ast->arg0) - ast_eval(ast->arg1);
    case '*':
        return ast_eval(ast->arg0) * ast_eval(ast->arg1);
    case '/':
        return ast_eval(ast->arg0) / ast_eval(ast->arg1);
    default:
        fprintf(stderr, "Unknown op: %c\n", ast->op);
        exit(1);
    }
}

