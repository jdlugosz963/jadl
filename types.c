#include "types.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>


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

LISP_OBJECT *lisp_object_make_func(LISP_OBJECT *(*func)(LISP_OBJECT*))
{
	LISP_OBJECT *lisp_obj = lisp_object_make();
	lisp_obj->type = LISP_TYPE_FUNC;
	lisp_obj->value.func = func;
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

int lisp_object_cmp_types(LISP_OBJECT *a, LISP_OBJECT *b)
{
	if(a==NULL && b==NULL)
		return 1;
	else if(a==NULL || b==NULL)
		return 0;
	else if(a->type != b->type)
		return 0;
	else if(a->type == b->type)
		return 1;
	return 0;
}

int lisp_object_cmp(LISP_OBJECT *a, LISP_OBJECT *b)
{
	int is_same_type = lisp_object_cmp_types(a, b);
	if(!is_same_type)
		return 0;

	switch(a->type)
	{
	case LISP_TYPE_LIST:
		break;
	case LISP_TYPE_NUMBER:
		return lisp_object_cmp_numbers(a, b);
		break;
	case LISP_TYPE_STRING:
		lisp_object_cmp_string(a, b);
		break;
	case LISP_TYPE_SYMBOL:
		lisp_object_cmp_symbol(a, b);
		break;
	case LISP_TYPE_NIL:
	case LISP_TYPE_TRUE:
	case LISP_TYPE_FALSE:
	case LISP_TYPE_ERROR:
		return 1;
		break;
	}
}

int lisp_object_cmp_numbers(LISP_OBJECT *a, LISP_OBJECT *b) // TODO: make it works 
{
	int is_same_type= lisp_object_cmp_types(a, b);
	if(!is_same_type && a->type == LISP_TYPE_NUMBER)
		return 0;

	if(a->is_decimal_point && b->is_decimal_point)
		return *a->value.number_float == *b->value.number_float;
	else if(a->is_decimal_point && !b->is_decimal_point)
		return *a->value.number_float == *(double *)b->value.number_natural;
	else if(!a->is_decimal_point && b->is_decimal_point)
		return *(double *)a->value.number_natural == *b->value.number_float;
	else
		return *a->value.number_natural == *b->value.number_natural;
}

int lisp_object_cmp_symbol(LISP_OBJECT *a, LISP_OBJECT *b)
{
	int is_same_type= lisp_object_cmp_types(a, b);
	if(!is_same_type && a->type == LISP_TYPE_SYMBOL)
		return 0;

	return strcmp(a->value.symbol, b->value.symbol) == 0;
}

int lisp_object_cmp_string(LISP_OBJECT *a, LISP_OBJECT *b)
{
	int is_same_type= lisp_object_cmp_types(a, b);
	if(!is_same_type && a->type == LISP_TYPE_STRING)
		return 0;

	return strcmp(a->value.string, b->value.string) == 0;
}

HashMap *hash_map_make(char *key, LISP_OBJECT *value)
{
	return hash_map_push(NULL, key, value);
}

HashMap *hash_map_push(HashMap *hash_map, char *key, LISP_OBJECT *value)
{
	HashMap *is_exist = hash_map_find(hash_map, key);

	if(is_exist != NULL)
		return NULL;

	HashMap *new_hash_map = jadl_malloc(sizeof(HashMap));

	new_hash_map->key = jadl_malloc(sizeof(char) * strlen(key));;
	strcpy(new_hash_map->key, key);

	new_hash_map->value = value;
	new_hash_map->prev = NULL;

	if(hash_map != NULL)
		hash_map->prev = new_hash_map;
	new_hash_map->next = hash_map;

	return new_hash_map;
}

HashMap *hash_map_reverse(HashMap *hash_map)
{
	HashMap *current = hash_map;
	HashMap *next = NULL;
	HashMap *new_hash_map = NULL;
	while (current) {
		next = current->next;

		current->next = new_hash_map;
		new_hash_map = current;

		current = next;
	}

	return new_hash_map;
}


void hash_map_free(HashMap *hash_map)
{
	HashMap *next = NULL;

	while (hash_map)
	{
		next = hash_map->next;

		jadl_free(hash_map->key);
		lisp_object_free(hash_map->value);
		jadl_free(hash_map);

		hash_map = next;
	}
}

HashMap *hash_map_find(HashMap *hash_map, char* key)
{
	if (hash_map == NULL)
		return NULL;

	while (hash_map)
	{
		if(strcmp(hash_map->key, key) == 0)
			return hash_map;

		hash_map = hash_map->next;
	}

	return NULL;
}

HashMap *hash_map_delete(HashMap *hash_map, char* key)
{
	HashMap *hash_map_to_free = hash_map_find(hash_map, key);

	if(hash_map_to_free == NULL)
		return hash_map;

	HashMap *new_hash_map = NULL;

	if(hash_map_to_free->prev == NULL)
	{
		new_hash_map = hash_map_to_free->next;
		new_hash_map->prev = NULL;
	} else
	{
		new_hash_map = hash_map_to_free->prev;
		new_hash_map->next=hash_map_to_free->next;
		if(new_hash_map->next)
			new_hash_map->next->prev = new_hash_map;
	}

	HashMap *prev = new_hash_map->prev;
	while(prev)
	{
		new_hash_map = prev;
		prev = prev->prev;
	}

	hash_map_to_free->next = NULL;
	hash_map_free(hash_map_to_free);

	return new_hash_map;
}


void hash_map_print(HashMap *hash_map)
{
	while (hash_map)
	{
		printf("-------------\n");
		printf("HashMap->key = %s\n", hash_map->key);
		printf("HashMap->value: \n");
		lisp_object_print(hash_map->value, 0);

		hash_map = hash_map->next;
	}

	printf("-------------\n");
}

HashMap *hash_last(HashMap *hash_map)
{
	HashMap *next = NULL;
	while (hash_map)
	{
		next = hash_map->next;
		if(!next)
			return hash_map;
		hash_map = next;
	}
	return NULL;
}
