#include "parser.h"

int NBR_INST_TRUE = 1 ;

void

_create_statement()
{
    _test_symbol(CREATE_TOKEN);
    CURRENT_CHILD_INST = current_token.code ;
    switch(current_token.code)
    {
        case KEYSPACE_TOKEN     : get_lex(); _create_keyspace_statement();         break;
        case TABLE_TOKEN        : get_lex(); _create_table_statement();            break;
        case COLUMNFAMILY_TOKEN : get_lex(); _create_table_statement();            break;
        case INDEX_TOKEN        : get_lex(); _create_index_statement();            break;
        case MATERIALIZED_TOKEN : get_lex(); _create_materialized_statement();     break;
        case ROLE_TOKEN         : get_lex(); _create_role_statement();             break;
        case USER_TOKEN         : get_lex(); _create_user_statement();             break;
        case TYPE_TOKEN         : get_lex(); _create_type_statement();             break;
        case TRIGGER_TOKEN      : get_lex(); _create_trigger_statement();          break;
        case FUNCTION_TOKEN     : get_lex(); _create_function_statement();         break;
        case AGGREGATE_TOKEN    : get_lex(); _create_aggregate_statement();        break;

        case OR_TOKEN           :
        get_lex();
        _test_symbol(REPLACE_TOKEN);
        switch(current_token.code)
        {
            case FUNCTION_TOKEN  : get_lex(); _create_function_statement();  break;
            case AGGREGATE_TOKEN : get_lex(); _create_aggregate_statement(); break;
            default              : print_error(PARSER_ERROR, current_token.value); get_lex();
        }
        break;

        case CUSTOM_TOKEN       :
        get_lex();
        _test_symbol(INDEX_TOKEN);
        _create_index_statement();
        break;
        default : print_error(PARSER_ERROR, current_token.value); get_lex();
    }
}

void _alter_statement()
{

    _test_symbol(ALTER_TOKEN);
    CURRENT_CHILD_INST = current_token.code;
    switch (current_token.code)
    {
        case KEYSPACE_TOKEN  :  get_lex();      _alter_keyspace_statement();    break;
        case TABLE_TOKEN     :  get_lex();      _alter_table_statement();       break;
        case ROLE_TOKEN      :  get_lex();      _alter_role_statement();        break;
        case USER_TOKEN      :  get_lex();      _alter_user_statement();        break;
        case TYPE_TOKEN      :  get_lex();      _alter_type_statement();        break;
        default:    print_error(PARSER_ERROR, current_token.value);  get_lex();          break;
    }
}

void _drop_statement()
{
    _test_symbol(DROP_TOKEN);
    CURRENT_CHILD_INST = current_token.code;
    switch (current_token.code) {
        case KEYSPACE_TOKEN  :  get_lex();      _drop_keyspace_statement();      break;
        case TABLE_TOKEN     :  get_lex();      _drop_table_statement();         break;
        case INDEX_TOKEN     :  get_lex();      _drop_index_statement();         break;
        case ROLE_TOKEN      :  get_lex();      _drop_role_statement();          break;
        case USER_TOKEN      :  get_lex();      _drop_user_statement();          break;
        case FUNCTION_TOKEN  :  get_lex();      _drop_function_statement();      break;
        case AGGREGATE_TOKEN :  get_lex();      _drop_aggregate_statement();     break;
        case TYPE_TOKEN      :  get_lex();      _drop_type_statement();          break;
        case TRIGGER_TOKEN   :  get_lex();      _drop_trigger_statement();       break;
        case MATERIALIZED_TOKEN:get_lex();      _drop_materialized_statement();  break;
        default:  print_error(PARSER_ERROR, current_token.value); break;
    }
}

void _cassandra_program()
{
    while(current_token.code != END_TOKEN)
    {
        //printf("\n\n------------------------------- INSTRUCTION NUM : %d ------------------------------- \n",NBR_INST_TRUE++);
        CURRENT_PARENT_INST = current_token.code ;
        switch(current_token.code)
        {
            case CREATE_TOKEN   : _create_statement();  break;
            case USE_TOKEN      : _use_statement();     break;
            case USING_TOKEN    : _using_statement();   break;
            case ALTER_TOKEN    : _alter_statement();   break;
            case DROP_TOKEN     : _drop_statement();    break;
            case INSERT_TOKEN   : _insert_statement();  break;
            case UPDATE_TOKEN   : _update_statement();  break;
            case SELECT_TOKEN   : _select_statement();  break;
            case BEGIN_TOKEN    : _batch_statement();   break;
            case DELETE_TOKEN   : _delete_statement();  break;
            case GRANT_TOKEN    : _grant_statement();   break;
            case REVOKE_TOKEN   : _revoke_statement();  break;
            case LIST_TOKEN     : _list_statement();    break;
            case TRUNCATE_TOKEN : _truncate_statement();break;
            default             :
            if(current_token.code == ERROR_TOKEN) print_error(SCANNER_ERROR, current_token.value);
            else print_error(PARSER_ERROR, current_token.value);
        }
        _test_symbol(PV_TOKEN);


        // AVANT DE FAIRE APPEL A L INTERPRETATION , JE DOIT VERIFIER SI LE SYNTAXIQUE ET SEMANTIQUE SONT TRUE
        if (get_semantic_state() && get_syntaxic_state())
        { 
            interpret();
            //printf("\t --- EST VRAI --\n");
        }
        // Renitialiser Le traitement Semantique et Syntaxique pour l'instruction précédente et faire des noveau etat pour l instruction prochaine
        reset_states();
        reset_parameter();
    }
}

void _parsing()
{
    //Demarage
    read_char(file);
    get_lex();

    _cassandra_program();

    /*if(current_token.code == END_TOKEN && no_error == true)
    {
        printf("\nend : 0 error\n");
    }
    else
    {
        printf("\nend with errors\n" );
    }*/
}
