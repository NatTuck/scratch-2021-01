// A recursive descent parser for simple arithmetic expressions
//
// Note: Makes everything right-associative

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "tokens.h"

// store the currently processed token stream
list *parse_tokens = NULL;

// Initialize the parser state with the given token stream.
void init_parser(list *tokens) {
    parse_tokens = tokens;
}

// Is the token stream empty?
int is_done() {
    return parse_tokens == empty;
}

// Advance the parser to the next token.
void advance() {
    if (!is_done())
        parse_tokens = rest(parse_tokens);
}

// Get the current token from the token stream.
char *current_token() {
    assert(!is_done());
    return first(parse_tokens);
}

// Parse a value and return the corresponding AST value.
ast *parse_value() {
    char *token = current_token();
    if (!isdigit(token[0])) 
        return NULL;

    int value = atoi(token);

    advance();

    return make_ast_value(value);
}

// Parse an expression (which can be a value or an operator expression) and
// return the corresponding AST value.
ast *parse_expr() {
    ast *left = parse_value();

    if (!left) {
        fprintf(stderr, "Expected a number. Got \"%s\"\n", current_token());
        return left;
    }
    if (is_done()) {
        return left;
    }

    // FIXME: Better check for valid operator tokens?
    char *token = current_token();
    if (strlen(token) != 1) {
        fprintf(stderr, "Expected an operator. Got \"%s\"\n", token);
        free_ast(left);
        return NULL;
    }

    char op = token[0];
    
    advance();
    if (is_done()) {
        fprintf(stderr, "Unexpected end of token stream after \"%s\"\n", token);
        free_ast(left);
        return NULL;
    }

    ast *right = parse_expr();
    if (!right) {
        free_ast(left);
        return NULL;
    }

    return make_ast_op(op, left, right);
}

// Convenience function: parse the given string and return the expression AST.
ast *parse(char *str) {
    list *tokens = tokenize(str);

    init_parser(tokens);
    ast *a = parse_expr();
    free_list(tokens);
    return a;
}

/*
int main(int argc, char **argv) {
    assert(argc > 1);

    ast *expr = parse(argv[1]);

    assert(expr);

    print_ast(expr);

    free_ast(expr);

    return 0;
}
*/

// vim:sw=4
