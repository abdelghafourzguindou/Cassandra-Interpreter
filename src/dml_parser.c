#include "dml_parser.h"


void _use_statement() {
    _test_symbol(USE_TOKEN);
    _test_symbol(IDENTIFIER_TOKEN);

    strcpy(current_keyspace_name, last_token.value);
}

void _select_statement(void) {
    _test_symbol(SELECT_TOKEN);
    if(current_token.code==JSON_TOKEN || current_token.code==DISTINCT_TOKEN) get_lex();
    _select_clause();

    _test_symbol(FROM_TOKEN);
    isTableName();
    //get_lex();
    switch(current_token.code) {
        case WHERE_TOKEN: get_lex(); _where_clause(); break;
        case GROUP_TOKEN: get_lex(); _test_symbol(BY_TOKEN); _group_by_clause(); break;
        case ORDER_TOKEN: get_lex(); _test_symbol(BY_TOKEN); _order_by_clause(); break;
        case PER_TOKEN  : get_lex(); _test_symbol(PARTITION_TOKEN); _test_symbol(LIMIT_TOKEN); _option_type(); break;
        case LIMIT_TOKEN: get_lex(); _option_type(); break;
        case ALLOW_TOKEN: get_lex(); _test_symbol(FILTERING_TOKEN); break;
        default : /*printf("error : _select_statement");*/ break;
    }

    // Control Semantique
    Head_Column_Of_Table = load_current_table_columns();
    column_t *p = Out;
    while(p != NULL) {
        if(search_column_indice(p->column_name,Head_Column_Of_Table) < 0)
            print_error(COLUMN_IS_NOT_EXISTS , p->column_name);
        p = p->next;
    }
}

void _insert_statement(void){
    _test_symbol(INSERT_TOKEN);
    _test_symbol(INTO_TOKEN);
    isTableName();
    //Ici le control semantique
    _insert_statement_aux();
    _if_not_exists();
    _option_using();
    //print_column(Head_Column_Of_Table);
}

void _delete_statement(void) {
    _test_symbol(DELETE_TOKEN);
    _option_selection();
    _test_symbol(FROM_TOKEN);
    isTableName();
    _option_using();
    _test_symbol(WHERE_TOKEN);
    //Controle pour l'interpretation
    if(last_token.code == WHERE_TOKEN) EXIST_CONDITION = true;

    _relation();
    while(current_token.code==AND_TOKEN){
        get_lex();
        _relation();
    }
    _if_exist();

}

void _batch_statement(void){

    _test_symbol(BEGIN_TOKEN);
    _option_batch();
    _test_symbol(BATCH_TOKEN);
    _option_using();
    _modification_statement();
    while(current_token.code==PV_TOKEN){
        get_lex();
        if(current_token.code==INSERT_TOKEN
            || current_token.code==UPDATE_TOKEN
            || current_token.code==DELETE_TOKEN) _modification_statement();
         else;
 }

 _test_symbol(APPLY_TOKEN);
 _test_symbol(BATCH_TOKEN);
}

void _update_statement(void){
    _test_symbol(UPDATE_TOKEN);
    //if(current_token.code==TABLE_FUNCTION_NAME_TOKEN || current_token.code==IDENTIFIER_TOKEN) get_lex();
    //else print_error(PARSER_ERROR, current_token.value);
    isTableName();
    if(current_token.code==USING_TOKEN){
        _option_using();
    }
    _test_symbol(SET_TOKEN);
    _assignment();

    while(current_token.code==VIR_TOKEN){
        get_lex();
        _assignment();
    }

    _test_symbol(WHERE_TOKEN);
    //Controle de l'existance de where
    if(last_token.code == WHERE_TOKEN) EXIST_CONDITION = true;
    
    _relation();
    while(current_token.code==AND_TOKEN){
        get_lex();
        _relation();
    }
    _if_exist();

}
