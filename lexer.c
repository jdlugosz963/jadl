#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Lexer *lexer_make()
{
	Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
	return lexer;
}

void lexer_free(Lexer *lexer)
{
	Token *token = lexer->tokens;
	Token *token_next = NULL;

	while (token)
	{
		token_next = token->next;
		jadl_free(token->value);
		jadl_free(token);
		token = token_next;
	}
	jadl_free(lexer);
}

Token *lexer_token_make(char *value, int value_len, TokenType type)
{
	Token *token = jadl_malloc(sizeof(token));
	token->value = jadl_malloc(sizeof(char) * value_len + 1);
	token->value[value_len] = '\0';
	token->type = type;
	strncpy(token->value, value, value_len);
	token->is_decimal_point = 0;
	return token;
}

Lexer *lexer_token_push(Lexer *lexer, Token *token)
{
	token->next = lexer->tokens;
	lexer->tokens = token;
	return lexer;
}

Lexer *lexer_tokens_reverse(Lexer *lexer)
{
	Token *token = lexer->tokens;
	Token *token_next = NULL;
	lexer->tokens=NULL;

	while (token)
	{
		token_next = token->next;

		lexer_token_push(lexer, token);

		token = token_next;
	}
	return lexer;
}

char *lexer_token_make_string(char *str, Token **token)
{
	char *end = str;
	int is_string_read = 1;
	str += 1;

	do
	{
		end = strchr(end+1, '"');
		if (!end)
		{
			lexer_token_make_error("Cannot find end of string!", token);
			is_string_read = 0;
			break;
		}
		is_string_read = 1;
	} while(*(end-1) == '\\');

	if(is_string_read)
	{
		int str_len = (end - str);
		*token = lexer_token_make(str, str_len, TOKEN_TYPE_STRING);
	}

        return (!is_string_read) ? NULL : end + 1;
}

char *lexer_token_make_number(char *str, Token **token)
{
	char *end = lexer_token_terminated_symbol(str);

	char *next = str;
	int is_decimal_point=0;
	int is_number_read=1;

	while(next<end)
	{
		if (!is_decimal_point && *next == '.') 
			is_decimal_point=1;
		else if(*next == '.') {
			lexer_token_make_error(
				"Cannot read number, becouse it has too many decimal points!", token);
			is_number_read = 0;
			break;
		}
		else if(!isdigit(*next)) {
			lexer_token_make_error("Cannot read number, becouse it isnt digit!", token);
			is_number_read = 0;
			break;
		}
		
		is_number_read = 1;
		next+=1;
	}

	if(is_number_read)
	{
		int str_len = (end - str + 1); *token = lexer_token_make(str, str_len, TOKEN_TYPE_NUMBER);
		(*token)->is_decimal_point = is_decimal_point;
	}

	return (is_number_read) ? end + 1 : NULL;
}

char *lexer_token_terminated_symbol(char *str)
{
	static char *chars_to_terminate = " ()[],;\"`";
	char *terminated = strpbrk(str, chars_to_terminate);
	terminated = (terminated == NULL) ? &str[strlen(str) - 1] : terminated - 1;
	return terminated;
}

char *lexer_token_make_symbol(char *str, Token **token)
{
	char *end = lexer_token_terminated_symbol(str);

	int str_len = (end - str + 1);

	*token = lexer_token_make(str, str_len, TOKEN_TYPE_SYMBOL);

	if(strncmp(str, SYMBOL_NIL, str_len) == 0) 
		(*token)->type = TOKEN_TYPE_NIL;
	else if(strncmp(str, SYMBOL_FALSE, str_len) == 0 ||
		strncmp(str, SYMBOL_FALSE_SHORT, str_len) == 0) 
		(*token)->type = TOKEN_TYPE_FALSE;
	else if(strncmp(str, SYMBOL_TRUE, str_len) == 0 ||
		strncmp(str, SYMBOL_TRUE_SHORT, str_len == 0)) 
		(*token)->type = TOKEN_TYPE_TRUE;

	return end + 1;
}

char *lexer_token_make_special(char *str, Token **token)
{
	*token = lexer_token_make(str, 1, TOKEN_TYPE_SPECIAL);
	return str + 1;
}


void lexer_token_make_error(char *message, Token **token)
{
	unsigned long message_len = strlen(message);
	*token = lexer_token_make(message, message_len, TOKEN_TYPE_ERROR);
}


Lexer *lexer_tokenize(char *str) {
	Lexer *lexer = lexer_make();
	Token *token = NULL;

	while (str && *str) {
		/* if(!*str) return lexer; */

		switch (*str) {
		case ' ':
		case ';':
			token = NULL;
			str += 1;
			break;
		case '(':
		case ')':
		case '[':
		case ']':
			str = lexer_token_make_special(str, &token);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			str = lexer_token_make_number(str, &token);
			break;
		case '"':
			str = lexer_token_make_string(str, &token);
			break;
		default:
			str = lexer_token_make_symbol(str, &token);
			break;
		}
		if (token)
			lexer_token_push(lexer, token);
	}

	return lexer;
}

void lexer_tokens_print(Lexer *lexer)
{
	Token *token = lexer->tokens;
	while (token) {
		switch(token->type) {
		case TOKEN_TYPE_STRING:
			printf("String: ");
			break;
		case TOKEN_TYPE_SYMBOL:
			printf("Symbol: ");
			break;
		case TOKEN_TYPE_SPECIAL:
			printf("Special: ");
			break;
		case TOKEN_TYPE_NUMBER:
			printf("Number: ");
			break;
		case TOKEN_TYPE_TRUE:
			printf("True: ");
			break;
		case TOKEN_TYPE_FALSE:
			printf("False: ");
			break;
		case TOKEN_TYPE_NIL:
			printf("Nil: ");
			break;
		}
		printf("%s\n", token->value);
		token = token->next;
	}
}
