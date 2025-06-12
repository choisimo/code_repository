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
     // Ensure current_filename is not NULL
     // extern char *current_filename; // Available via env.h
     // extern int get_lineno(); // Available via subc.h
     if (current_filename != NULL) {
         fprintf(stderr, "%s:%d: error: ", current_filename, get_lineno());
     } else {
         fprintf(stderr, "unknown_file:%d: error: ", get_lineno());
     }
 }
 
 void error_undeclared(void)
 {
     error_preamble();
     fprintf(stderr, "use of undeclared identifier\n");
 }
 
 void error_redeclaration(void)
 {
     error_preamble();
     fprintf(stderr, "redeclaration\n");
 }
 