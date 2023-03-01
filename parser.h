#ifndef _PARSER
#define _PARSER

#include "types.h"
#include "lexer.h"

LISP_OBJECT *parser_parse_str(char *str);
LISP_OBJECT *parser_parse_tokens(Token **token);

LISP_OBJECT *parser_make_list(Token **token);
LISP_OBJECT *parser_make_number(Token **token);
LISP_OBJECT *parser_make_string(Token **token);
LISP_OBJECT *parser_make_symbol(Token **token);
LISP_OBJECT *parser_make_true(Token **token);
LISP_OBJECT *parser_make_false(Token **token);
LISP_OBJECT *parser_make_nil(Token **token);

#endif
