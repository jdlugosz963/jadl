#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "types.h"
#include "parser.h"
#include "eval.h"


LISP_OBJECT *Fadd(LISP_OBJECT *l)
{
	long long natural=0;

	List *list = l->value.list;
	LISP_OBJECT *a = NULL;
	while(list)
	{
		a = list->ptr;
		natural += *a->value.number_natural;
		list = list->next;
	}
	LISP_OBJECT *c = lisp_object_make_number_natural(natural);
	return c;
}

LISP_OBJECT *Fminus(LISP_OBJECT *l)
{
	LISP_OBJECT *a = l->value.list->ptr;
	LISP_OBJECT *b = l->value.list->next->ptr;
	LISP_OBJECT *c = lisp_object_make_number_natural(
		*a->value.number_natural - *b->value.number_natural);
	return c;
}

LISP_OBJECT *Fmul(LISP_OBJECT *l)
{
	LISP_OBJECT *a = l->value.list->ptr;
	LISP_OBJECT *b = l->value.list->next->ptr;
	LISP_OBJECT *c = lisp_object_make_number_natural(
		*a->value.number_natural * *b->value.number_natural);
	return c;
}

LISP_OBJECT *Fdiv(LISP_OBJECT *l)
{
	LISP_OBJECT *a = l->value.list->ptr;
	LISP_OBJECT *b = l->value.list->next->ptr;
	LISP_OBJECT *c = lisp_object_make_number_natural(
		*a->value.number_natural / *b->value.number_natural);
	return c;
}

LISP_OBJECT *Fplusone(LISP_OBJECT *l)
{
	LISP_OBJECT *a = l->value.list->ptr;
	LISP_OBJECT *c = lisp_object_make_number_natural(
		*a->value.number_natural+1);
	return c;
}


LISP_OBJECT *READ(char *input)
{
        return parser_parse_str(input);
}

LISP_OBJECT *EVAL(LISP_OBJECT *input, HashMap *env)
{
	return eval(input, env);
}

void PRINT(LISP_OBJECT *input)
{
	lisp_object_print(input, 0);
	lisp_object_free(input);
}

void rep(char *input, HashMap *env)
{
	PRINT(EVAL(READ(input), env));
}


int main()
{
	HashMap *env = NULL;
	env = hash_map_push(env, "+", lisp_object_make_func(&Fadd));
	env = hash_map_push(env, "-", lisp_object_make_func(&Fminus));
	env = hash_map_push(env, "*", lisp_object_make_func(&Fmul));
	env = hash_map_push(env, "/", lisp_object_make_func(&Fdiv));
	env = hash_map_push(env, "++", lisp_object_make_func(&Fplusone));

	char *input1 = NULL;
	char *input = NULL;

	while(1)
	{
		input = readline("JADL> ");
		input1 = malloc(sizeof(char) * strlen(input) + 1);
		strcpy(input1, input);
		input1[strlen(input)] = '\0';

		free(input);

		rep(input1, env);
		free(input1);
	}
}
