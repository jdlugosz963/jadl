#ifndef _TYPES
#define _TYPES

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"


typedef enum _LispType {
	LISP_TYPE_SYMBOL,
	LISP_TYPE_LIST,
	LISP_TYPE_STRING,
	LISP_TYPE_NUMBER,
	LISP_TYPE_NIL,
	LISP_TYPE_TRUE,
	LISP_TYPE_FALSE,
	LISP_TYPE_ERROR
} LispType;
	

typedef struct _List {
	void *ptr;
	struct _List *next;
} List;

List *list_make(void *ptr);
List *list_push(List *list, void *ptr);
List *list_reverse(List *list);

typedef struct _LispObject {
	LispType type;
	int is_decimal_point;

	union {
		char *string;
		char *symbol;
		char *error;
		long long *number_natural;
		double *number_float;
		struct _LispObject (*function)(struct _LispObject*);
		List *list;
		void *ptr;
	} value;
} LISP_OBJECT;

void lisp_object_free(LISP_OBJECT *lisp_obj);

void lisp_object_list_push(LISP_OBJECT *lisp_list_obj, void *ptr);

LISP_OBJECT *lisp_object_make();
LISP_OBJECT *lisp_object_make_list();
LISP_OBJECT *lisp_object_make_number_natural(long long number);
LISP_OBJECT *lisp_object_make_number_float(double number);
LISP_OBJECT *lisp_object_make_string(char *str);
LISP_OBJECT *lisp_object_make_symbol(char *str);
LISP_OBJECT *lisp_object_make_error(char *str);
LISP_OBJECT *lisp_object_make_nil();
LISP_OBJECT *lisp_object_make_true();
LISP_OBJECT *lisp_object_make_false();

int lisp_object_print(LISP_OBJECT *obj, int indent);

#endif
