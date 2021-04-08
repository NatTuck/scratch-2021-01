

#include <stdio.h>

#include "ast.h"
#include "parse.h"

int
main(int _argc, char* _argv[])
{
    char line[100];

    while (1) {
        char* rv = fgets(line, 96, stdin);
        if (!rv) {
            break;
        }

        calc_ast* ast = calc_parse(line);
        printf(" = %d\n", ast_eval(ast));
        free_ast(ast);
    }


    return 0;
}

