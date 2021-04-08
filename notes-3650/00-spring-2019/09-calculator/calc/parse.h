#ifndef CALC_PARSE_H
#define CALC_PARSE_H

#include "ast.h"
#include "svec.h"

calc_ast* parse(svec* tokens);

#endif
