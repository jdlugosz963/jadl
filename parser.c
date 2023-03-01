#include "parser.h"
#include "lexer.h"
#include "types.h"
#include <stdio.h>


LISP_OBJECT *parser_parse_str(char *str)
{
	Lexer *lexer = lexer_tokenize(str);
	lexer_tokens_reverse(lexer);

	Token *token = lexer->tokens;
	LISP_OBJECT *lisp_obj = NULL;
	lisp_obj = parser_parse_tokens(&token);

	lexer_free(lexer);

	return lisp_obj;
}

LISP_OBJECT *parser_parse_tokens(Token **token)
{
	LISP_OBJECT *lisp_obj = NULL;
	switch((*token)->type)
	{
	case TOKEN_TYPE_SPECIAL:
		lisp_obj = parser_make_list(token);
		break;
	case TOKEN_TYPE_NUMBER:
		lisp_obj = parser_make_number(token);
		break;
	case TOKEN_TYPE_STRING:
		lisp_obj = parser_make_string(token);
		break;
	case TOKEN_TYPE_SYMBOL:
		lisp_obj = parser_make_symbol(token);
		break;
	case TOKEN_TYPE_TRUE:
		lisp_obj = parser_make_true(token);
		break;
	case TOKEN_TYPE_FALSE:
		lisp_obj = parser_make_false(token);
		break;
	case TOKEN_TYPE_NIL:
		lisp_obj = parser_make_nil(token);
		break;
	case TOKEN_TYPE_ERROR:
		lisp_obj = lisp_object_make_error((*token)->value);
		*token = (*token)->next;
		break;
	default:
		*token = (*token)->next;
		lisp_obj = lisp_object_make_error("Something bad happenend!");
	}
	return lisp_obj;
}

LISP_OBJECT *parser_make_list(Token **token)
{
	LISP_OBJECT *lisp_list_obj = lisp_object_make_list();
	char close_delimiter = ( *(*token)->value == '(' ) ? ')' : ']';
	*token = (*token)->next;
	int is_close_delimiter = 1;

	while(*token)
	{
		if((*token)->type==TOKEN_TYPE_SPECIAL &&
		   *(*token)->value==close_delimiter)
		{
			is_close_delimiter=0;
			*token=(*token)->next;
			break;
		}

		lisp_object_list_push(lisp_list_obj,
				      (void *)parser_parse_tokens(token));

	}


	List *list = list_reverse(lisp_list_obj->value.list);
	lisp_list_obj->value.list = list;

	if(is_close_delimiter != 0)
	{
		LISP_OBJECT *err_msg = lisp_object_make_error("Unbalanced parenthesis!");
		lisp_object_list_push(lisp_list_obj, err_msg);
	}

        return lisp_list_obj;
}


LISP_OBJECT *parser_make_number(Token **token)
{
	LISP_OBJECT *lisp_obj = NULL;
	char *number_str = (*token)->value;
	union
	{
		long long natural;
		double floating;
		
	} number;

	if((*token)->is_decimal_point)
	{
		sscanf(number_str, "%lf", &number.floating);
		lisp_obj = lisp_object_make_number_float(number.floating);
	}
	else
	{
		sscanf(number_str, "%lld", &number.natural);
		lisp_obj = lisp_object_make_number_natural(number.natural);
	}


	*token=(*token)->next;
	return lisp_obj;
}

LISP_OBJECT *parser_make_string(Token **token)
{
	char *str = (*token)->value;
	LISP_OBJECT *lisp_obj = lisp_object_make_string(str);

	*token=(*token)->next;
	return lisp_obj;
}

LISP_OBJECT *parser_make_symbol(Token **token)
{
	char *str = (*token)->value;
	LISP_OBJECT *lisp_obj = lisp_object_make_symbol(str);

	*token=(*token)->next;
	return lisp_obj;
}

LISP_OBJECT *parser_make_true(Token **token)
{
	LISP_OBJECT *lisp_obj = lisp_object_make_true();

	*token=(*token)->next;
	return lisp_obj;
}

LISP_OBJECT *parser_make_false(Token **token)
{
	LISP_OBJECT *lisp_obj = lisp_object_make_false();

	*token=(*token)->next;
	return lisp_obj;
}


LISP_OBJECT *parser_make_nil(Token **token)
{
	LISP_OBJECT *lisp_obj = lisp_object_make_nil();

	*token=(*token)->next;
	return lisp_obj;
}

int main()
{
	char *test = "(display (string-append \"4*3+2=\" (number->string (+ (* 4 3) 1))))";


	LISP_OBJECT *obj = NULL;

	obj = parser_parse_str(test);
	lisp_object_print(obj, 0);
	lisp_object_free(obj);
}
