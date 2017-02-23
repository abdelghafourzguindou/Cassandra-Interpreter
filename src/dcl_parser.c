#include "dcl_parser.h"
//grant statements

void _grant_statement(){
    
    current_grant = (grant_t*) malloc(sizeof(grant_t));

    _test_symbol(GRANT_TOKEN);
    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN)
        _grant_role_statement();
    else _grant_permission_statement();
}

void _grant_role_statement(){

    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN) get_lex();
    else {print_error(PARSER_ERROR, current_token.value);get_lex();}

    _test_symbol(TO_TOKEN);

    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN)get_lex();
    else {print_error(PARSER_ERROR, current_token.value);get_lex();}
}

void _grant_permission_statement(){
    _permissions();
    _test_symbol(ON_TOKEN);
    _ressource();
    _test_symbol(TO_TOKEN);

    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN);
    else print_error(PARSER_ERROR, current_token.value);
    //Controle
    strcpy(current_grant->user_name, current_token.value);
    
    get_lex();
}

//reveoke statements

void _revoke_statement(){
    _test_symbol(REVOKE_TOKEN);
    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN)
        _revoke_role_statement();
    else _revoke_permission_statement();

}



void _revoke_role_statement(){
    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN) get_lex();
    else {print_error(PARSER_ERROR, current_token.value);get_lex();}

    _test_symbol(FROM_TOKEN);

    if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN)get_lex();
    else {print_error(PARSER_ERROR, current_token.value);get_lex();}



}

void _revoke_permission_statement(){
    _permissions();
    _test_symbol(ON_TOKEN);
    _ressource();
    _test_symbol(FROM_TOKEN);
    if(current_token.code==IDENTIFIER_TOKEN|| current_token.code==STRING_TOKEN)get_lex();
    else {print_error(PARSER_ERROR, current_token.value);get_lex();}

}


//list statements
void _list_statement(){
    _test_symbol(LIST_TOKEN);
    if(current_token.code==ROLES_TOKEN){
        _list_roles_statement();
    }
    else if(current_token.code==USERS_TOKEN){
        _list_users_statement();
    }
    else
        _list_permissions_statement();

}

void _list_roles_statement(){
    _test_symbol(ROLES_TOKEN);
    _of_role_statement();
    _no_recursive_statement();

}



void _list_users_statement(){
    _test_symbol(USERS_TOKEN);
}

void _list_permissions_statement(){
    _permissions();
    _on_ressource_statement();
    _of_role_name_statement();

}

