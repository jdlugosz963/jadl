#ifndef _EVAL
#define _EVAL

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "memory.h"


// HashMap *repl_env = NULL;

LISP_OBJECT *eval_apply(LISP_OBJECT *lisp_list, HashMap *env);
LISP_OBJECT *eval_prepere_args(LISP_OBJECT *lisp_list, HashMap *env);
LISP_OBJECT *eval(LISP_OBJECT *lisp_obj, HashMap *env);

#endif
