#include "env.h"
#include "type.h"
#include "symtab.h"
#include <stdlib.h>

char *current_filename = NULL;
int  building_struct_fields = 0; /* flag used by parser to detect struct field context */

void init_environment(void)
{
    /* init types */
    init_builtin_types();

    /* ensure global scope exists */
    enter_scope();
}
