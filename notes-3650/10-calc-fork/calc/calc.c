

#include <stdio.h>

#include "ast.h"
#include "tokens.h"
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

        list* toks = tokenize(line);
        calc_ast* ast = parse(toks);
        printf(" = %d\n", ast_eval(ast));
        free_ast(ast);
        free_list(toks);
    }

    return 0;
}

