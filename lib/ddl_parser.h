#ifndef DDL_PARSER_H
#define DDL_PARSER_H

#include "parser_tools.h"

void _create_keyspace_statement();
void _create_table_statement();
void _create_index_statement();
void _create_materialized_statement();
void _create_role_statement();
void _create_user_statement();
void _create_type_statement();
void _create_trigger_statement();
void _create_function_statement();
void _create_aggregate_statement();

void _using_statement();

void _alter_keyspace_statement();
void _alter_table_statement();
void _alter_role_statement();
void _alter_user_statement();
void _alter_type_statement();

void _drop_keyspace_statement();
void _drop_table_statement();
void _drop_index_statement();
void _drop_role_statement();
void _drop_user_statement();
void _drop_function_statement();

void _drop_aggregate_statement();
void _drop_type_statement();
void _drop_trigger_statement();
void _drop_materialized_statement();

void _truncate_statement();

#endif // DDL_PARSER_H
