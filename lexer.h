#ifndef _LEXER
#define _LEXER

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "memory.h"

#define SYMBOL_NIL "nil"
#define SYMBOL_TRUE_SHORT "#t"
#define SYMBOL_TRUE "#true"
#define SYMBOL_FALSE_SHORT "#f"
#define SYMBOL_FALSE "#false"


typedef enum _TokenType {
	TOKEN_TYPE_SYMBOL,
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_TRUE,
	TOKEN_TYPE_FALSE,
	TOKEN_TYPE_NIL,
	TOKEN_TYPE_SPECIAL,
	TOKEN_TYPE_ERROR
} TokenType;


typedef struct _Token {
	TokenType type;
	int is_decimal_point;
	char *value;
	struct _Token *next;
} Token;

typedef struct _Lexer {
	int error;
	Token *tokens;
} Lexer;

int lexer_tokens_count(char *str);

Lexer *lexer_make();
void lexer_free(Lexer *lexer);
Token *lexer_token_make(char *value, int value_len, TokenType type);
Lexer *lexer_token_push(Lexer *lexer, Token *token);

char *lexer_token_terminated_symbol(char *str);
char *lexer_token_make_string(char *str, Token **token);
char *lexer_token_make_symbol(char *str, Token **token);
char *lexer_token_make_special(char *str, Token **token);
char *lexer_token_make_number(char *str, Token **token);

void lexer_token_make_error(char *message, Token **token);

Lexer *lexer_tokenize(char *str);


void lexer_tokens_print(Lexer *lexer);
Lexer *lexer_tokens_reverse(Lexer *lexer);

#endif
