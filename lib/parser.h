#ifndef PARSER_H
#define PARSER_H

#include "ddl_parser.h"
#include "dml_parser.h"
#include "dcl_parser.h"
#include "error.h"
#include "interpreter.h"

int NBR_INST_TRUE ;

code_token_t CURRENT_PARENT_INST ;
code_token_t CURRENT_CHILD_INST  ;

void _create_statement(void);
void _alter_statement(void);
void _drop_statement(void);
void _grant_statement(void);
void _cassandra_program(void);
void _parsing(void);




#endif // PARSER_H
