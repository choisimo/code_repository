/*
 * File Name    : error.c
 * Description  : Implementation of semantic error reporting helpers.
 */

#include "error.h"
#include <stdio.h>
#include "env.h"
#include "subc.h"

/* Common prefix for every semantic error. */
void error_preamble(void)
{
    printf("%s:%d: error: ", current_filename, get_lineno());
}

void error_undeclared(void)
{
    error_preamble();
    printf("use of undeclared identifier\n");
}

void error_redeclaration(void)
{
    error_preamble();
    printf("redeclaration\n");
}
