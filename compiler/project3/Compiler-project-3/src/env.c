#include "env.h"
#include "type.h"
#include "symtab.h"
#include <stdlib.h>

char *current_filename = NULL;

void init_environment(void)
{
    /* init types */
    init_builtin_types();

    /* ensure global scope exists */
    enter_scope();
}
