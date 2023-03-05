#include "eval.h"
#include "types.h"


LISP_OBJECT *eval_apply(LISP_OBJECT *lisp_list, HashMap *env)
{
	if(lisp_list == NULL || lisp_list->type != LISP_TYPE_LIST)
		return lisp_list;

	LISP_OBJECT *symbol = lisp_list->value.list->ptr;
	List *args = lisp_list->value.list->next;

	LISP_OBJECT *func = hash_map_find(env, symbol->value.symbol)->value;

	if(func->type != LISP_TYPE_FUNC)
		return lisp_list;

	List *tmp = lisp_list->value.list;
	lisp_list->value.list = args;
	LISP_OBJECT *ret = func->value.func(lisp_list);
	lisp_list->value.list = tmp;
	return ret;
}

LISP_OBJECT *eval_prepere_args(LISP_OBJECT *lisp_list, HashMap *env)
{
	if(lisp_list == NULL || lisp_list->type != LISP_TYPE_LIST)
		return lisp_list;

	LISP_OBJECT *args = lisp_object_make_list();
	List **args_list = &args->value.list;

	List *list = lisp_list->value.list;
	LISP_OBJECT *obj = NULL;

	while(list)
	{
		obj = list->ptr;
		if(obj->type==LISP_TYPE_LIST)
			obj = eval(obj, env);

		*args_list = list_push(*args_list, obj);
		list = list->next;
	}

	*args_list = list_reverse(*args_list);

	return eval_apply(args, env);
}


LISP_OBJECT *eval(LISP_OBJECT *lisp_obj, HashMap *env)
{
	if(lisp_obj == NULL)
		return NULL;

	if(lisp_obj->type == LISP_TYPE_LIST)
		return eval_prepere_args(lisp_obj, env);

	return lisp_obj;
}
