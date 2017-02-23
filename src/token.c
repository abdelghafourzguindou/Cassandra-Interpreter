#include "token.h"

code_token_t is_Keyword(char* keyword)
{

    code_token_t token;

    if     (!strcmp(keyword, "ADD"))                token = ADD_TOKEN;
    else if(!strcmp(keyword, "ALLOW"))              token = ALLOW_TOKEN;
    else if(!strcmp(keyword, "ALTER"))              token = ALTER_TOKEN;
    else if(!strcmp(keyword, "AND"))                token = AND_TOKEN;
    else if(!strcmp(keyword, "ANY"))                token = ANY_TOKEN;
    else if(!strcmp(keyword, "APPLY"))              token = APPLY_TOKEN;
    else if(!strcmp(keyword, "ASC"))                token = ASC_TOKEN;
    else if(!strcmp(keyword, "AUTHORIZE"))          token = AUTHORIZE_TOKEN;
    else if(!strcmp(keyword, "BATCH"))              token = BATCH_TOKEN;
    else if(!strcmp(keyword, "BEGIN"))              token = BEGIN_TOKEN;
    else if(!strcmp(keyword, "BY"))                 token = BY_TOKEN;
    else if(!strcmp(keyword, "COLUMNFAMILY"))       token = COLUMNFAMILY_TOKEN;
    else if(!strcmp(keyword, "CREATE"))             token = CREATE_TOKEN;
    else if(!strcmp(keyword, "DELETE"))             token = DELETE_TOKEN;
    else if(!strcmp(keyword, "DESC"))               token = DESC_TOKEN;
    else if(!strcmp(keyword, "DESCRIBE"))           token = DESCRIBE_TOKEN;
    else if(!strcmp(keyword, "DROP"))               token = DROP_TOKEN;
    else if(!strcmp(keyword, "ENTRIES"))            token = ENTRIES_TOKEN;
    else if(!strcmp(keyword, "EXECUTE"))            token = EXECUTE_TOKEN;
    else if(!strcmp(keyword, "FROM"))               token = FROM_TOKEN;
    else if(!strcmp(keyword, "FULL"))               token = FULL_TOKEN;
    else if(!strcmp(keyword, "GRANT"))              token = GRANT_TOKEN;
    else if(!strcmp(keyword, "GROUP"))              token = GROUP_TOKEN;
    else if(!strcmp(keyword, "IF"))                 token = IF_TOKEN;
    else if(!strcmp(keyword, "IN"))                 token = IN_TOKEN;
    else if(!strcmp(keyword, "INDEX"))              token = INDEX_TOKEN;
    else if(!strcmp(keyword, "INFINITY"))           token = INFINITY_TOKEN;
    else if(!strcmp(keyword, "INSERT"))             token = INSERT_TOKEN;
    else if(!strcmp(keyword, "INTO"))               token = INTO_TOKEN;
    else if(!strcmp(keyword, "KEYSPACE"))           token = KEYSPACE_TOKEN;
    else if(!strcmp(keyword, "LIMIT"))              token = LIMIT_TOKEN;
    else if(!strcmp(keyword, "MODIFY"))             token = MODIFY_TOKEN;
    else if(!strcmp(keyword, "NAN"))                token = NAN_TOKEN;
    else if(!strcmp(keyword, "NORECURSIVE"))        token = NORECURSIVE_TOKEN;
    else if(!strcmp(keyword, "NOT"))                token = NOT_TOKEN;
    else if(!strcmp(keyword, "NULL"))               token = NULL_TOKEN;
    else if(!strcmp(keyword, "OF"))                 token = OF_TOKEN;
    else if(!strcmp(keyword, "ON"))                 token = ON_TOKEN;
    else if(!strcmp(keyword, "OR"))                 token = OR_TOKEN;
    else if(!strcmp(keyword, "ORDER"))              token = ORDER_TOKEN;
    else if(!strcmp(keyword, "PARTITION"))          token = PARTITION_TOKEN;
    else if(!strcmp(keyword, "PER"))                token = PER_TOKEN;
    else if(!strcmp(keyword, "PRIMARY"))            token = PRIMARY_TOKEN;
    else if(!strcmp(keyword, "RENAME"))             token = RENAME_TOKEN;
    else if(!strcmp(keyword, "REPLACE"))            token = REPLACE_TOKEN;
    else if(!strcmp(keyword, "REVOKE"))             token = REVOKE_TOKEN;
    else if(!strcmp(keyword, "SCHEMA"))             token = SCHEMA_TOKEN;
    else if(!strcmp(keyword, "SELECT"))             token = SELECT_TOKEN;
    else if(!strcmp(keyword, "SET"))                token = SET_TOKEN;
    else if(!strcmp(keyword, "TABLE"))              token = TABLE_TOKEN;
    else if(!strcmp(keyword, "TO"))                 token = TO_TOKEN;
    else if(!strcmp(keyword, "TOKEN"))              token = TOKEN_TOKEN;
    else if(!strcmp(keyword, "TRUNCATE"))           token = TRUNCATE_TOKEN;
    else if(!strcmp(keyword, "UNLOGGED"))           token = UNLOGGED_TOKEN;
    else if(!strcmp(keyword, "UPDATE"))             token = UPDATE_TOKEN;
    else if(!strcmp(keyword, "USE"))                token = USE_TOKEN;
    else if(!strcmp(keyword, "USING"))              token = USING_TOKEN;
    else if(!strcmp(keyword, "WHERE"))              token = WHERE_TOKEN;
    else if(!strcmp(keyword, "WITH"))               token = WITH_TOKEN;
    else if(!strcmp(keyword, "ASCII"))              token = ASCII_TOKEN;
    else if(!strcmp(keyword, "BIGINT"))             token = BIGINT_TOKEN;
    else if(!strcmp(keyword, "BLOB"))               token = BLOB_TOKEN;
    else if(!strcmp(keyword, "BOOLEAN"))            token = BOOLEAN_TOKEN;
    else if(!strcmp(keyword, "COUNTER"))            token = COUNTER_TOKEN;
    else if(!strcmp(keyword, "DATE"))               token = DATE_TOKEN;
    else if(!strcmp(keyword, "DECIMAL"))            token = DECIMAL_TOKEN;
    else if(!strcmp(keyword, "DOUBLE"))             token = DOUBLE_TOKEN;
    else if(!strcmp(keyword, "FLOAT"))              token = FLOAT_TOKEN;
    else if(!strcmp(keyword, "INET"))               token = INET_TOKEN;
    else if(!strcmp(keyword, "INT"))                token = INT_TOKEN;
    else if(!strcmp(keyword, "SMALLINT"))           token = SMALLINT_TOKEN;
    else if(!strcmp(keyword, "TEXT"))               token = TEXT_TOKEN;
    else if(!strcmp(keyword, "TIME"))               token = TIME_TOKEN;
    else if(!strcmp(keyword, "TYPE"))               token = TYPE_TOKEN;
    else if(!strcmp(keyword, "TIMESTAMP"))          token = TIMESTAMP_TOKEN;
    else if(!strcmp(keyword, "TIMEUUID"))           token = TIMEUUID_TOKEN;
    else if(!strcmp(keyword, "TINYINT"))            token = TINYINT_TOKEN;
    else if(!strcmp(keyword, "UUID"))               token = UUID_TOKEN;
    else if(!strcmp(keyword, "VARCHAR"))            token = VARCHAR_TOKEN;
    else if(!strcmp(keyword, "VARINT"))             token = VARINT_TOKEN;
    else if(!strcmp(keyword, "AVG"))                token = AVG_TOKEN;
    else if(!strcmp(keyword, "MAX"))                token = MAX_TOKEN;
    else if(!strcmp(keyword, "MIN"))                token = MIN_TOKEN;
    else if(!strcmp(keyword, "SUM"))                token = SUM_TOKEN;
    else if(!strcmp(keyword, "COUNT"))              token = COUNT_TOKEN ;
    else if(!strcmp(keyword, "ALL"))                token = ALL_TOKEN;
    else if(!strcmp(keyword, "AGGREGATE"))          token = AGGREGATE_TOKEN;
    else if(!strcmp(keyword, "AS"))                 token = AS_TOKEN;
    else if(!strcmp(keyword, "CALLED"))             token = CALLED_TOKEN;
    else if(!strcmp(keyword, "CLUSTERING"))         token = CLUSTERING_TOKEN;
    else if(!strcmp(keyword, "COMPACT"))            token = COMPACT_TOKEN;
    else if(!strcmp(keyword, "CONSISTENCY"))        token = CONSISTENCY_TOKEN;
    else if(!strcmp(keyword, "CONTAINS"))           token = CONTAINS_TOKEN;
    else if(!strcmp(keyword, "CUSTOM"))             token = CUSTOM_TOKEN;
    else if(!strcmp(keyword, "DISTINCT"))           token = DISTINCT_TOKEN;
    else if(!strcmp(keyword, "EXISTS"))             token = EXISTS_TOKEN;
    else if(!strcmp(keyword, "FILTERING"))          token = FILTERING_TOKEN;
    else if(!strcmp(keyword, "FINALFUNC"))          token = FINALFUNC_TOKEN;
    else if(!strcmp(keyword, "FROZEN"))             token = FROZEN_TOKEN;
    else if(!strcmp(keyword, "FUNCTION"))           token = FUNCTION_TOKEN;
    else if(!strcmp(keyword, "FUNCTIONS"))          token = FUNCTIONS_TOKEN;
    else if(!strcmp(keyword, "INITCOND"))           token = INITCOND_TOKEN;
    else if(!strcmp(keyword, "INPUT"))              token = INPUT_TOKEN;
    else if(!strcmp(keyword, "JSON"))               token = JSON_TOKEN;
    else if(!strcmp(keyword, "KEY"))                token = KEY_TOKEN;
    else if(!strcmp(keyword, "KEYS"))               token = KEYS_TOKEN;
    else if(!strcmp(keyword, "KEYSPACES"))          token = KEYSPACES_TOKEN;
    else if(!strcmp(keyword, "LANGUAGE"))           token = LANGUAGE_TOKEN;
    else if(!strcmp(keyword, "LEVEL"))              token = LEVEL_TOKEN;
    else if(!strcmp(keyword, "LIST"))               token = LIST_TOKEN;
    else if(!strcmp(keyword, "LOGIN"))              token = LOGIN_TOKEN;
    else if(!strcmp(keyword, "MAP"))                token = MAP_TOKEN;
    else if(!strcmp(keyword, "NOLOGIN"))            token = NOLOGIN_TOKEN;
    else if(!strcmp(keyword, "NOSUPERUSER"))        token = NOSUPERUSER_TOKEN;
    else if(!strcmp(keyword, "OPTIONS"))            token = OPTIONS_TOKEN;
    else if(!strcmp(keyword, "PASSWORD"))           token = PASSWORD_TOKEN;
    else if(!strcmp(keyword, "PERMISSION"))         token = PERMISSION_TOKEN;
    else if(!strcmp(keyword, "PERMISSIONS"))        token = PERMISSIONS_TOKEN;
    else if(!strcmp(keyword, "RETURNS"))            token = RETURNS_TOKEN;
    else if(!strcmp(keyword, "ROLE"))               token = ROLE_TOKEN;
    else if(!strcmp(keyword, "ROLES"))              token = ROLES_TOKEN;
    else if(!strcmp(keyword, "SFUNC"))              token = SFUNC_TOKEN;
    else if(!strcmp(keyword, "STATIC"))             token = STATIC_TOKEN;
    else if(!strcmp(keyword, "STORAGE"))            token = STORAGE_TOKEN;
    else if(!strcmp(keyword, "SUPERUSER"))          token = SUPERUSER_TOKEN;
    else if(!strcmp(keyword, "TTL"))                token = TTL_TOKEN;
    else if(!strcmp(keyword, "TRIGGER"))            token = TRIGGER_TOKEN;
    else if(!strcmp(keyword, "TUPLE"))              token = TUPLE_TOKEN;
    else if(!strcmp(keyword, "USER"))               token = USER_TOKEN;
    else if(!strcmp(keyword, "USERS"))              token = USERS_TOKEN;
    else if(!strcmp(keyword, "VALUES"))             token = VALUES_TOKEN;
    else if(!strcmp(keyword, "WRITETIME"))          token = WRITETIME_TOKEN;
    else if(!strcmp(keyword, "MATERIALIZED"))       token = MATERIALIZED_TOKEN;
    else if(!strcmp(keyword, "VIEW"))               token = VIEW_TOKEN;
    else if(!strcmp(keyword, "IS"))                 token = IS_TOKEN;
    else if(!strcmp(keyword, "MBEAN"))              token = MBEAN_TOKEN;
    else if(!strcmp(keyword, "MBEANS"))             token = MBEANS_TOKEN;
    else                                            token = ERROR_TOKEN;

    return token;
}

char* token_code[SIZE_TOKEN] = {
    "ADD_TOKEN",
      "ALLOW_TOKEN",
      "ALTER_TOKEN",
      "AND_TOKEN",
      "ANY_TOKEN",
      "APPLY_TOKEN",
      "ASC_TOKEN",
      "AUTHORIZE_TOKEN",
      "BATCH_TOKEN",
      "BEGIN_TOKEN",
      "BY_TOKEN",
      "CAST_TOKEN",
      "COLUMNFAMILY_TOKEN",
      "CREATE_TOKEN",
      "DEFAULT_TOKEN",
      "DELETE_TOKEN",
      "DESC_TOKEN",
      "DESCRIBE_TOKEN",
      "DROP_TOKEN",
      "ENTRIES_TOKEN",
      "EXECUTE_TOKEN",
      "FROM_TOKEN",
      "FULL_TOKEN",
      "GRANT_TOKEN",
      "GROUP_TOKEN",
      "IF_TOKEN",
      "IN_TOKEN",
      "INDEX_TOKEN",
      "INFINITY_TOKEN",
      "INSERT_TOKEN",
      "INTO_TOKEN",
      "KEYSPACE_TOKEN",
      "LIMIT_TOKEN",
      "MODIFY_TOKEN",
      "NAN_TOKEN",
      "NORECURSIVE_TOKEN",
      "NOT_TOKEN",
      "NULL_TOKEN",
      "OF_TOKEN",
      "ON_TOKEN",
      "OR_TOKEN",
      "ORDER_TOKEN",
      "PARTITION_TOKEN",
      "PER_TOKEN",
      "PRIMARY_TOKEN",
      "RENAME_TOKEN",
      "REPLACE_TOKEN",
      "REVOKE_TOKEN",
      "SCHEMA_TOKEN",
      "SELECT_TOKEN",
      "SET_TOKEN",
      "TABLE_TOKEN",
      "TO_TOKEN",
      "TOKEN_TOKEN",
      "TRUNCATE_TOKEN",
      "UNLOGGED_TOKEN",
      "UNSET_TOKEN",
      "UPDATE_TOKEN",
      "USE_TOKEN",
      "USING_TOKEN",
      "WHERE_TOKEN",
      "WITH_TOKEN",
      //debut type
      "ASCII_TOKEN",
      "BIGINT_TOKEN",
      "BLOB_TOKEN",
      "BOOLEAN_TOKEN",
      "COUNTER_TOKEN",
      "DATE_TOKEN",
      "DECIMAL_TOKEN",
      "DOUBLE_TOKEN",
      "FLOAT_TOKEN",
      "INET_TOKEN",
      "INT_TOKEN",
      "SMALLINT_TOKEN",
      "TEXT_TOKEN",
      "TIME_TOKEN",
      "TIMESTAMP_TOKEN",
      "TIMEUUID_TOKEN",
      "TINYINT_TOKEN",
      "UUID_TOKEN",
      "VARCHAR_TOKEN",
      "VARINT_TOKEN",
      //debut valeur type
      "ASCII_VALUE_TOKEN",
      "BIGINT_VALUE_TOKEN",
      "BLOB_VALUE_TOKEN",
      "BOOLEAN_VALUE_TOKEN",
      "COUNTER_VALUE_TOKEN",
      "DATE_VALUE_TOKEN",
      "DECIMAL_VALUE_TOKEN",
      "DOUBLE_VALUE_TOKEN",
      "FLOAT_VALUE_TOKEN",
      "INET_VALUE_TOKEN",
      "INT_VALUE_TOKEN",
      "SMALLINT_VALUE_TOKEN",
      "TEXT_VALUE_TOKEN",
      "TIME_VALUE_TOKEN",
      "TIMESTAMP_VALUE_TOKEN",
      "TIMEUUID_VALUE_TOKEN",
      "TINYINT_VALUE_TOKEN",
      "UUID_VALUE_TOKEN",
      "VARCHAR_VALUE_TOKEN",
      "VARINT_VALUE_TOKEN",
      //debut agreggate
      "AVG_TOKEN",
      "MAX_TOKEN",
      "MIN_TOKEN",
      "SUM_TOKEN",
      "COUNT_TOKEN",
      //others
      "ALL_TOKEN",
      "AGGREGATE_TOKEN",
      "AS_TOKEN",
      "CALLED_TOKEN",
      "CLUSTERING_TOKEN",
      "COMPACT_TOKEN",
      "CONSISTENCY_TOKEN",
      "CONTAINS_TOKEN",
      "CUSTOM_TOKEN",
      "DISTINCT_TOKEN",
      "EXISTS_TOKEN",
      "FILTERING_TOKEN",
      "FINALFUNC_TOKEN",
      "FROZEN_TOKEN",
      "FUNCTION_TOKEN",
      "FUNCTIONS_TOKEN",
      "INITCOND_TOKEN",
      "INPUT_TOKEN",
      "JSON_TOKEN",
      "KEY_TOKEN",
      "KEYS_TOKEN",
      "KEYSPACES_TOKEN",
      "LANGUAGE_TOKEN",
      "LEVEL_TOKEN",
      "LIST_TOKEN",
      "LOGIN_TOKEN",
      "MAP_TOKEN",
      "NOLOGIN_TOKEN",
      "NOSUPERUSER_TOKEN",
      "OPTIONS_TOKEN",
      "PASSWORD_TOKEN",
      "PERMISSION_TOKEN",
      "PERMISSIONS_TOKEN",
      "RETURNS_TOKEN",
      "ROLE_TOKEN",
      "ROLES_TOKEN",
      "SFUNC_TOKEN",
      "STATIC_TOKEN",
      "STORAGE_TOKEN",
      "SUPERUSER_TOKEN",
      "TTL_TOKEN",
      "TRIGGER_TOKEN",
      "TUPLE_TOKEN",
      "TYPE_TOKEN",
      "USER_TOKEN",
      "USERS_TOKEN",
      "VALUES_TOKEN",
      "WRITETIME_TOKEN",
      "MATERIALIZED_TOKEN",
      "VIEW_TOKEN",
      "IS_TOKEN",
      //debut symboles
      "PV_TOKEN",
      "P_TOKEN",
      "PLUS_TOKEN",
      "MOINS_TOKEN",
      "ETOILE_TOKEN",
      "DIV_TOKEN",
      "VIR_TOKEN",
      "EG_TOKEN",
      "INF_TOKEN",
      "INFEG_TOKEN",
      "SUP_TOKEN",
      "SUPEG_TOKEN",
      "DIFF_TOKEN",
      "PO_TOKEN",
      "PF_TOKEN",
      "ACOLADO_TOKEN",
      "ACOLADF_TOKEN",
      "DEUXP_TOKEN",
      "PI_TOKEN",
      "CROCHO_TOKEN",
      "CROCHF_TOKEN",
      //Debut automates
      "STRING_TOKEN",
      "IDENTIFIER_TOKEN",
      "TABLE_FUNCTION_NAME_TOKEN",
      //pour le controle
      "END_TOKEN",
      "EMPTY_TOKEN",
      "ERROR_TOKEN",
      "MBEAN_TOKEN",
      "MBEANS_TOKEN"
};

void print_token(code_token_t num)
{
    printf("%s\t", token_code[num]);
}

void print_type(code_token_t keyword) {
  if     (keyword == ASCII_TOKEN    || keyword == ASCII_VALUE_TOKEN)     printf("ascii");
  else if(keyword == BIGINT_TOKEN   || keyword == BIGINT_VALUE_TOKEN)    printf("bigint");
  else if(keyword == BLOB_TOKEN     || keyword == BLOB_VALUE_TOKEN)      printf("blob");
  else if(keyword == BOOLEAN_TOKEN  || keyword == BOOLEAN_VALUE_TOKEN)   printf("boolean");
  else if(keyword == COUNTER_TOKEN  || keyword == COUNTER_VALUE_TOKEN)   printf("counter");
  else if(keyword == DATE_TOKEN     || keyword == DATE_VALUE_TOKEN)      printf("date");
  else if(keyword == DECIMAL_TOKEN  || keyword == DECIMAL_VALUE_TOKEN)   printf("decimal");
  else if(keyword == DOUBLE_TOKEN   || keyword == DOUBLE_VALUE_TOKEN)    printf("double");
  else if(keyword == FLOAT_TOKEN    || keyword == FLOAT_VALUE_TOKEN)     printf("float");
  else if(keyword == INET_TOKEN     || keyword == INET_VALUE_TOKEN)      printf("inet");
  else if(keyword == INT_TOKEN      || keyword == INT_VALUE_TOKEN)       printf("int");
  else if(keyword == SMALLINT_TOKEN || keyword == SMALLINT_VALUE_TOKEN)  printf("smallint");
  else if(keyword == TEXT_TOKEN     || keyword == TEXT_VALUE_TOKEN)      printf("text");
  else if(keyword == TIME_TOKEN     || keyword == TIME_VALUE_TOKEN)      printf("time");
  else if(keyword == TIMESTAMP_TOKEN|| keyword == TIMESTAMP_VALUE_TOKEN) printf("timestamp");
  else if(keyword == TIMEUUID_TOKEN || keyword == TIMEUUID_VALUE_TOKEN)  printf("timeuuid");
  else if(keyword == TINYINT_TOKEN  || keyword == TINYINT_VALUE_TOKEN)   printf("tinyint");
  else if(keyword == UUID_TOKEN     || keyword == UUID_VALUE_TOKEN)      printf("uuid");
  else if(keyword == VARCHAR_TOKEN  || 
    keyword == VARCHAR_VALUE_TOKEN || 
    STRING_TOKEN || 
    TEXT_TOKEN || 
    TEXT_VALUE_TOKEN)   printf("varchar");
  else if(keyword == VARINT_TOKEN   || keyword == VARINT_VALUE_TOKEN)    printf("varint");
  else if(keyword == TYPE_TOKEN)                                         printf("type");
  else;
}