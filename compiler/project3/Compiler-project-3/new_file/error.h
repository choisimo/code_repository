/*
 * File Name    : error.h
 * Description  : Error reporting helpers for Project-3 semantic analysis
 */

 #ifndef __ERROR_H__
 #define __ERROR_H__
 
 void error_preamble(void);
 void error_undeclared(void);
 void error_redeclaration(void);
 /* ... other helpers as required ... */
 // Add other error functions that are present in error.c for completeness,
 // assuming they might be used elsewhere or planned for future use.
 // For now, these are the ones explicitly mentioned or modified.
 // We need to ensure this list is comprehensive if other C files call these.
 // Based on error.c, these are the defined functions.
 // If more are added to error.c, they should be declared here too.
 
 #endif /* __ERROR_H__ */
 