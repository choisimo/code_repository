/*
 * File Name    : env.h
 * Description  : Global environment initialisation for Project 3 semantic analysis.
 */

#ifndef __ENV_H__
#define __ENV_H__

/* Expose current source filename for error reporting */
extern char *current_filename;

/* Prepare built-in types and create global scope.  Must be called before
 * parsing begins (ideally from main()). */
void init_environment(void);

#endif /* __ENV_H__ */
