#ifndef PARSER_TOOLS_H
#define PARSER_TOOLS_H

#include "scanner.h"
#include "error.h"
#include "interpreter.h"
#include "semantic.h"

int Indice_courant_value;

static token_t update_token;

void _test_symbol(code_token_t);
void isTableName(void);
void _index_option(void);
void _index_identifier(void);
void _options(void);
void _option(void);
void _constant(void);
void _map_literal(void);
void _term(void);
void _aux_1(void);
void _aux_2(void);
void _aux_3(void);
void _aux_4(void);
void _aux_1_1(void);
void _aux_1_2(void);
void _opt_data(void);
void _if_not_exists(void);
void _cql_type(void);
void _column_defenition(void);
void _primary_key_ox_para(void);
void _primary_key_def(void);
void _partition_key(void);
void _clustring_columns(void);
void _table_options(void);
void _clustering_oredr(void);
void _table_options_ox(void);
void _asc_desc(void);
void _alter_table_instruction(void);
void _alter_type_modification(void);
void _role_options(void);
void _role_option(void);
void _if_exist(void);
void _arg_sin(void);
void _exist_aux(void);
void _arg_dec(void);
void _select_clause(void);
void _selector(void);
void _selector_aux(void);
void _where_clause(void);
void _group_clause(void);
void _order_by_clause(void);
void _option_type(void);
void _relation(void);
void _operator(void);

void _option_using(void);
void _update_parametre(void);
void _assignment(void);
void _assignment_aux(void);
void _option_type_aux(void);
void _update_parametre_aux(void);
void _aff_column_name(void);
void _option_list_literal(void);
void _option_if(void);
void _option_if_aux(void);
void _condition(void);
void _tuple_literal(void);
void _insert_statement_aux(void);
void _option_default(void);
void _default_aux(void);
void _option_selection(void);
void _option_batch(void);
void _modification_statement(void);


void _permissions(void);
void _ressource(void);
void _permission(void);
void _all_statement(void);
void _all_functions_statement(void);
void _function_statement(void);
void _of_role_statement(void);
void _no_recursive_statement(void);
void _on_ressource_statement(void);
void _of_role_name_statement(void);


#endif // PARSER_TOOLS_H
