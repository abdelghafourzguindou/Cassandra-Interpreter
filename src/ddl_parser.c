#include "ddl_parser.h"


void _create_keyspace_statement()
{
  _test_symbol(IDENTIFIER_TOKEN);

  strcpy(current_keyspace_name, last_token.value);

  _if_not_exists();
  _test_symbol(WITH_TOKEN);
  _options();
}

void _create_table_statement()
{
  _if_not_exists();
  isTableName();

  _test_symbol(PO_TOKEN);
  _column_defenition();
  while(current_token.code == VIR_TOKEN)
  {
    get_lex();
    if(current_token.code == IDENTIFIER_TOKEN) _column_defenition();
  }
  _primary_key_ox_para();
  _test_symbol(PF_TOKEN);
  if(current_token.code == WITH_TOKEN)
  {
    get_lex();
    _table_options();
  }
    //print_table(current_table);
}


void _create_index_statement()
{
  _if_not_exists();
  if(current_token.code == IDENTIFIER_TOKEN) get_lex();
  _test_symbol(ON_TOKEN);
  isTableName();
  _test_symbol(PO_TOKEN);
  _index_identifier();
  _test_symbol(PF_TOKEN);
  if(current_token.code == USING_TOKEN) _using_statement();
}

void _create_materialized_statement()
{
  _test_symbol(VIEW_TOKEN);
  _if_not_exists();
  _test_symbol(IDENTIFIER_TOKEN);
  _test_symbol(AS_TOKEN);
  _select_statement();
  _test_symbol(PRIMARY_TOKEN);
  _test_symbol(KEY_TOKEN);
  _test_symbol(PO_TOKEN);
  _primary_key_def();
  _test_symbol(WITH_TOKEN);
  _table_options();
}

void _create_role_statement()
{
  _if_not_exists();
  _test_symbol(IDENTIFIER_TOKEN);
  if(current_token.code == WITH_TOKEN)
  {
   get_lex();
   _role_options();
 }
}

void _create_user_statement()
{
  user_t user;
  strcpy(user.user_name     , "");
  strcpy(user.user_password , "");
  user.user_option = NULL_TOKEN;
  
  _if_not_exists();
  _test_symbol(IDENTIFIER_TOKEN);
  //Controle
  strcpy(user.user_name, last_token.value);
  if(current_token.code == WITH_TOKEN)
  {
    get_lex();
    _test_symbol(PASSWORD_TOKEN);
    _test_symbol(STRING_TOKEN);
    strcpy(user.user_password, last_token.value);
  }

  if( current_token.code == SUPERUSER_TOKEN || current_token.code == NOSUPERUSER_TOKEN) {
    user.user_option = current_token.code;
    get_lex();
  }
  set_current_user(user);
}

void _create_type_statement()
{
 _if_not_exists();
 isTableName();
 _test_symbol(PO_TOKEN);
 _test_symbol(IDENTIFIER_TOKEN);
 _cql_type();
 while(current_token.code == VIR_TOKEN )
 {
   get_lex();
   _test_symbol(IDENTIFIER_TOKEN);
   _cql_type();
 }
 _test_symbol(PF_TOKEN);

}

void _create_trigger_statement()
{

  _if_not_exists();
  _test_symbol(IDENTIFIER_TOKEN);
  _test_symbol(ON_TOKEN);
  isTableName();
  _test_symbol(USING_TOKEN);
  _test_symbol(STRING_TOKEN);
}

void _create_function_statement()
{
  _if_not_exists();
  isTableName();
  _test_symbol(PO_TOKEN);
  _arg_dec();
  _test_symbol(PF_TOKEN);

  // Called_ox ::= CALLED | RETURNS NULL | Epsilon
  if(current_token.code == CALLED_TOKEN)
    get_lex();
  else if ( current_token.code == RETURNS_TOKEN )
  {
    get_lex();
    _test_symbol(NULL_TOKEN);
  }

  _test_symbol(ON_TOKEN);
  _test_symbol(NULL_TOKEN);
  _test_symbol(INPUT_TOKEN);
  _test_symbol(RETURNS_TOKEN);
  _cql_type();
  _test_symbol(LANGUAGE_TOKEN);
  _test_symbol(IDENTIFIER_TOKEN);
  _test_symbol(AS_TOKEN);
  _test_symbol(STRING_TOKEN);

}

void _create_aggregate_statement() {}

void _using_statement()
{
  _test_symbol(USING_TOKEN);
  _test_symbol(STRING_TOKEN);
  if(current_token.code == WITH_TOKEN)
    _index_option();
}

void _alter_keyspace_statement()
{

  _test_symbol(IDENTIFIER_TOKEN);
  _test_symbol(WITH_TOKEN);
  _options();
}

void _alter_table_statement()
{
  isTableName();
  _alter_table_instruction();

}

void _alter_role_statement()
{
  _test_symbol(IDENTIFIER_TOKEN);
  _test_symbol(WITH_TOKEN);
  _role_options();

}

void _alter_user_statement()
{
  user_t user;
  strcpy(user.user_name     , "");
  strcpy(user.user_password , "");
  user.user_option = NULL_TOKEN;

  _test_symbol(IDENTIFIER_TOKEN);
  strcpy(user.user_name     , last_token.value);

  switch (current_token.code) {

    case WITH_TOKEN :
    get_lex();
    _test_symbol(PASSWORD_TOKEN);
    _test_symbol(STRING_TOKEN);

    strcpy(user.user_password , last_token.value);
    break;
    case SUPERUSER_TOKEN   : user.user_option = SUPERUSER_TOKEN;   get_lex(); break;
    case NOSUPERUSER_TOKEN : user.user_option = NOSUPERUSER_TOKEN; get_lex(); break;
    default : print_error(PARSER_ERROR, current_token.value);                 break;
  }
  set_current_user(user);
}

void _alter_type_statement()
{
  isTableName();
  _alter_type_modification();
}

void _drop_keyspace_statement()
{
  _if_exist();
  //controle
  strcpy(last_token.value, current_token.value);
  _test_symbol(IDENTIFIER_TOKEN);
  //controle
  set_current_keyspace_path(last_token.value);
}

void _drop_table_statement()
{

  _if_exist();
  isTableName();

}

void _drop_index_statement()
{
  _if_exist();
  _test_symbol(IDENTIFIER_TOKEN);
}

void _drop_role_statement()
{
  _if_exist();
  _test_symbol(IDENTIFIER_TOKEN);
}

void _drop_materialized_statement()
{
  _test_symbol(VIEW_TOKEN);
  _if_exist();
  _test_symbol(IDENTIFIER_TOKEN);
}

void _drop_user_statement()
{  
  user_t user;
  strcpy(user.user_name     , "");
  strcpy(user.user_password , "");
  user.user_option = NULL_TOKEN;
  _if_exist();
  _test_symbol(IDENTIFIER_TOKEN);
  strcpy(user.user_name     ,last_token.value);
  set_current_user(user);
}

void _drop_function_statement()
{
  _if_exist();
  isTableName();
  _arg_sin();
}

void _drop_aggregate_statement()
{
  _drop_function_statement();
}

void _drop_type_statement()
{
  _drop_table_statement();
}

void _drop_trigger_statement()
{
  _if_exist();
  isTableName();
  _test_symbol(ON_TOKEN);
  isTableName();
}

void _truncate_statement()
{
  _test_symbol(TRUNCATE_TOKEN);
  _exist_aux();
  isTableName();
}
