/*
 * File Name    : error.h
 * Description  : Error reporting helpers for Project-3 semantic analysis
 */

#ifndef __ERROR_H__
#define __ERROR_H__

void error_preamble(const char *filename, int line);
void error_undeclared(const char *filename, int line);
void error_redeclaration(const char *filename, int line);
/* ... other helpers as required ... */

#endif /* __ERROR_H__ */
