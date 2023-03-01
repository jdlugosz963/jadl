#include "types.h"
#include "memory.h"
#include <stdio.h>


List *list_make(void *ptr)
{
	return list_push(NULL, ptr);
}

List *list_push(List *list, void *ptr)
{
	List *new_head = (List*)malloc(sizeof(List));
	new_head->next=list;
	new_head->ptr=ptr;
	return new_head;
}


List *list_reverse(List *list)
{
	List *current = list;
	List *next = NULL;
	List *new_list = NULL;
	while (current) {
		next = current->next;

		current->next = new_list;
		new_list = current;

		current = next;
	}
	return new_list;
}

void list_free(List *list)
{
	List *next = NULL;
	while (list)
	{
		next = list->next;

		jadl_free(list->ptr); 
		jadl_free(list);

		list = next;
	}
	
}

void lisp_object_list_push(LISP_OBJECT *lisp_list_obj, void *ptr)
{
	List *new_head = list_push(lisp_list_obj->value.list, ptr);
	lisp_list_obj->value.list = new_head;
}

LISP_OBJECT *lisp_object_make()
{
	LISP_OBJECT *lisp_obj = jadl_malloc(sizeof(LISP_OBJECT));
	lisp_obj->is_decimal_point = 0;
	return lisp_obj;
}

void lisp_object_list_free(LISP_OBJECT *lisp_obj_list)
{
	if(lisp_obj_list->type != LISP_TYPE_LIST) 
		return;
	
	List *list = lisp_obj_list->value.list;
	List *next = NULL;

	while (list)
	{
		next = list->next;

		LISP_OBJECT *lisp_obj = list->ptr;

		lisp_object_free(lisp_obj); 

		jadl_free(list);
		list = next;
	}
	jadl_free(lisp_obj_list);
}

void lisp_object_free(LISP_OBJECT *lisp_obj)
{
	if (lisp_obj->type == LISP_TYPE_LIST)
		lisp_object_list_free(lisp_obj);
	else
	{
		if (lisp_obj->type != LISP_TYPE_FALSE &&
		    lisp_obj->type != LISP_TYPE_TRUE &&
		    lisp_obj->type != LISP_TYPE_NIL)
		{
			jadl_free(lisp_obj->value.ptr);
		}
		jadl_free(lisp_obj);
	}
}

LISP_OBJECT *lisp_object_make_list()
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_LIST;
	lisp_obj->value.ptr = NULL;
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_number_natural(long long number)
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_NUMBER;
	lisp_obj->value.number_natural = jadl_malloc(sizeof(long long));
	*lisp_obj->value.number_natural = number;
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_number_float(double number)
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_NUMBER;
	lisp_obj->value.number_float = jadl_malloc(sizeof(double));
	*lisp_obj->value.number_float = number;
	lisp_obj->is_decimal_point = 1;
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_string(char *str)
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_STRING;
	lisp_obj->value.string = jadl_malloc(sizeof(char)*strlen(str));
	strcpy(lisp_obj->value.string, str);
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_symbol(char *str)
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_SYMBOL;
	lisp_obj->value.symbol = jadl_malloc(sizeof(char)*strlen(str));
	strcpy(lisp_obj->value.symbol, str);
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_error(char *str)
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_ERROR;
	lisp_obj->value.error = jadl_malloc(sizeof(char)*strlen(str));
	strcpy(lisp_obj->value.error, str);
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_nil()
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_NIL;
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_true()
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_TRUE;
	return lisp_obj;
}

LISP_OBJECT *lisp_object_make_false()
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_FALSE;
	return lisp_obj;
}

int lisp_object_print(LISP_OBJECT *obj, int indent)
{
	int count = 0;
	for(int i=0; i<indent; i++) { printf("    "); }

	switch(obj->type)
	{
	case LISP_TYPE_LIST:
		printf("    │\n");
		break;
	default:
		printf("└─ ");
		break;
	}

	switch(obj->type)
	{
	case LISP_TYPE_LIST:
		List *lst = obj->value.list;
		int temp = 0;
		while (lst)
		{
			count ++;
			temp = lisp_object_print((LISP_OBJECT *)lst->ptr,
					  indent+1);
			count += temp;
			lst = lst->next;
		}
		break;
	case LISP_TYPE_STRING:
		printf("STRING: %s\n", obj->value.string);
		break;
	case LISP_TYPE_ERROR:
		printf("ERROR: %s\n", obj->value.error);
		break;
	case LISP_TYPE_SYMBOL:
		printf("SYMBOL: %s\n", obj->value.symbol);
		break;
	case LISP_TYPE_NUMBER:
		if(obj->is_decimal_point)
			printf("NUMBER: %lf\n", *obj->value.number_float);
		else
			printf("NUMBER: %lld\n", *obj->value.number_natural);
		break;
	case LISP_TYPE_TRUE:
		printf("TRUE\n");
		break;
	case LISP_TYPE_FALSE:
		printf("FALSE\n");
		break;
	case LISP_TYPE_NIL:
		printf("NIL\n");
		break;
	}
	return count;
}
