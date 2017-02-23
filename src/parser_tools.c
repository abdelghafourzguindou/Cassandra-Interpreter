#include "parser_tools.h"

Indice_courant_value = 1;
code_token_t op ;

column_t            *Out;
char                *COLUMN_CONDITION;
char                *CONDITION;
bool                EXIST_CONDITION;
bool                IS_COUNT;
where_condition_t   *headConditions;
token_t_list*       Head_liste_IN;
user_t              *current_user;
grant_t             *current_grant;

void _test_symbol(code_token_t code)
{
    if(current_token.code == code  || (code == IDENTIFIER_TOKEN  &&  current_token.code >= 62 ) );
    else
    {
        //expected_words(1,code);
       //
        print_error(PARSER_ERROR, current_token.value);
    }
    get_lex();
}

void isTableName()
{
    if(current_token.code == IDENTIFIER_TOKEN || current_token.code == TABLE_FUNCTION_NAME_TOKEN)
    {
        set_current_table_path(current_token.value);
        // Les traitements Semantiques pour create table
        if( CURRENT_PARENT_INST == CREATE_TOKEN && CURRENT_CHILD_INST == TABLE_TOKEN )
        {
        // the function  serch if there is any table in the current Keyspace with the same name
            if(is_exist_table(get_current_table_path(), 0))
            {
                print_error(TABLE_IS_ALREADY_EXISTS, current_token.value);
            }
            else
            {
                current_table = create_table(current_token.value);
            }
        }
        //Cntrole de DROP TABLE
        else if( (CURRENT_PARENT_INST == DROP_TOKEN && CURRENT_CHILD_INST == TABLE_TOKEN) || (CURRENT_PARENT_INST == TRUNCATE_TOKEN ) )
        {
            if(!is_exist_table(get_current_table_path(), 0))
            {
                print_error(TABLE_IS_NOT_EXISTS, current_token.value);
            }
        }
        //Cntrole de Alter table
        else if( CURRENT_PARENT_INST == ALTER_TOKEN && CURRENT_CHILD_INST == TABLE_TOKEN )
        {
            if(!is_exist_table(get_current_table_path(), 0))
            {
                print_error(TABLE_IS_NOT_EXISTS, current_token.value);
            }
            else current_table = create_table(current_token.value);
        }
        else if( CURRENT_PARENT_INST == SELECT_TOKEN || CURRENT_PARENT_INST ==UPDATE_TOKEN || CURRENT_PARENT_INST ==DELETE_TOKEN)
        {
            if(!is_exist_table(get_current_table_path(), 0))
            {
                print_error(TABLE_IS_NOT_EXISTS, current_token.value);
            }
            else current_table = create_table(current_token.value);
        }
        else
        // Les regles Semantique pour les autres Cas , D abord On test par INSERT INTO
        {
            if ( CURRENT_PARENT_INST == INSERT_TOKEN || //insert controle
                 CURRENT_PARENT_INST == UPDATE_TOKEN    //update controle
                 )
            {
               // Ici on cherche l existance de la table , s'il existe on doit retourner une liste chainé de type column_t
                Head_Column_Of_Table = is_exist_table(get_current_table_path(),1);

                //print_column(Head_Column_Of_Table);
                //exit(-1);
                if( Head_Column_Of_Table == NULL)
                    print_error(TABLE_IS_NOT_EXISTS, current_token.value);
            }
        }
    }

    else
        print_error(PARSER_ERROR, current_token.value);
    get_lex();
}


void _alter_table_instruction()
{
    char* col_name;
    switch (current_token.code)
    {
        case ALTER_TOKEN : ALTER_TABLE_ORDER = ALTER_TOKEN;
        get_lex();
        //controle
        last_token.code = current_token.code ;

        _test_symbol(IDENTIFIER_TOKEN);
        //controle
        col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
        strcpy(col_name, last_token.value);

        _test_symbol(TYPE_TOKEN);
        _cql_type();
        //conntrole
        if(search_column_indice(col_name,load_current_table_columns()) < 0)
            print_error(COLUMN_IS_NOT_EXISTS , col_name);

        if(! search_column_CREATE(col_name , current_table->column_list)  )
        {
         current_table->column_list = add_column(create_column(col_name, current_column_type) , current_table->column_list);
     }
     else {
        print_error(COLUMN_IS_ALREADY_ALTERED, col_name);

    }

    break;

    case ADD_TOKEN : ALTER_TABLE_ORDER = ADD_TOKEN;
    get_lex();
        //controle
    last_token.code = current_token.code ;

    _test_symbol(IDENTIFIER_TOKEN);
        //controle
    col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
    strcpy(col_name, last_token.value);

    _cql_type();
        //conntrole

    if(search_column_indice(col_name,load_current_table_columns()) >= 0)
        print_error(COLUMN_IS_ALREADY_EXISTS , col_name);

    if(! search_column_CREATE(col_name , current_table->column_list)  ) {
        current_table->column_list = add_column(create_column(col_name, current_column_type) , current_table->column_list);
    }
    else {
        print_error(COLUMN_IS_ALREADY_ADDED, col_name);

    }

        // Il faut faire getlex()
    while(current_token.code == VIR_TOKEN)
    {
        get_lex();
            //controle
        last_token.code = current_token.code ;

        _test_symbol(IDENTIFIER_TOKEN);
            //controle
        col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
        strcpy(col_name, last_token.value);

        _cql_type();
            //conntrole
        if(search_column_indice(col_name,load_current_table_columns()) >= 0)
            print_error(COLUMN_IS_ALREADY_EXISTS , col_name);

        if(! search_column_CREATE(col_name , current_table->column_list)  ) {
            current_table->column_list = add_column(create_column(col_name, current_column_type) , current_table->column_list);
        }
        else {
            print_error(COLUMN_IS_ALREADY_ADDED, col_name);
            
        }

            // Il faut faire getlex()

    }
    break;

    case DROP_TOKEN : ALTER_TABLE_ORDER = DROP_TOKEN;
    get_lex();
        //controle
    last_token.code = current_token.code ;

    _test_symbol(IDENTIFIER_TOKEN);
        //controle
    col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
    strcpy(col_name, last_token.value);
        //conntrole
    if(search_column_indice(col_name,load_current_table_columns()) < 0)
        print_error(COLUMN_IS_NOT_EXISTS , col_name);

    if(! search_column_CREATE(col_name , current_table->column_list)  ) {
        current_table->column_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->column_list);
    }
    else {
        print_error(COLUMN_IS_ALREADY_EXISTS, col_name);

    }

    while(current_token.code == IDENTIFIER_TOKEN) {
            //controle
        last_token.code = current_token.code ;
        get_lex();
            //controle
        col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
        strcpy(col_name, last_token.value);
            //conntrole
        if(! search_column_CREATE(col_name , current_table->column_list)  ) {
            current_table->column_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->column_list);
        }
        else {
            print_error(COLUMN_IS_ALREADY_EXISTS, col_name);

        }
    }
    break;

    case WITH_TOKEN : ALTER_TABLE_ORDER = WITH_TOKEN;
    get_lex();
    _options();
    break;

    default:  print_error(PARSER_ERROR, current_token.value); break;
}
}

void _alter_type_modification()
{
    switch (current_token.code) {
        case ALTER_TOKEN :
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _test_symbol(TYPE_TOKEN);
        _cql_type();
        break;

        case ADD_TOKEN :
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _cql_type();
        break;

        case RENAME_TOKEN :
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _test_symbol(TO_TOKEN);
        _test_symbol(IDENTIFIER_TOKEN);
        while(current_token.code == IDENTIFIER_TOKEN)
        {
            get_lex();
            _test_symbol(TO_TOKEN);
            _test_symbol(IDENTIFIER_TOKEN);
        }
        break;
        default: print_error(PARSER_ERROR, current_token.value); break;
    }
}

void _role_options()
{
    _role_option();
    while (current_token.code == AND_TOKEN )
    {
        get_lex();
        _role_option();
    }
}

void _role_option()
{
    switch (current_token.code) {
        case PASSWORD_TOKEN  :     get_lex();    _test_symbol(EG_TOKEN);    _test_symbol(STRING_TOKEN);                          break;
        case LOGIN_TOKEN     :     get_lex();    _test_symbol(EG_TOKEN);    _test_symbol(BOOLEAN_VALUE_TOKEN);                   break;
        case SUPERUSER_TOKEN :     get_lex();    _test_symbol(EG_TOKEN);    _test_symbol(BOOLEAN_VALUE_TOKEN);                   break;
        case OPTIONS_TOKEN   :     get_lex();    _test_symbol(EG_TOKEN);    _test_symbol(ACOLADO_TOKEN);   _map_literal();       break;
        default:   print_error(PARSER_ERROR, current_token.value);

    }
}

void _if_exist()
{
    if ( current_token.code == IF_TOKEN )
    {
        get_lex();
        _test_symbol(EXISTS_TOKEN);

    }
}

void _arg_sin()
{
    if( current_token.code == END_TOKEN );
    _cql_type();
    //avancer avant dans cql_type
    while(current_token.code == VIR_TOKEN )
    {
        get_lex();
        _cql_type();
    }
}

void _exist_aux()
{
    if( current_token.code == TABLE_TOKEN || current_token.code == COLUMNFAMILY_TOKEN )
        get_lex();

}

void _cql_type()
{

    if(current_token.code==ASCII_TOKEN
        || current_token.code==BIGINT_TOKEN
        || current_token.code==BLOB_TOKEN
        || current_token.code==BOOLEAN_TOKEN
        || current_token.code==COUNTER_TOKEN
        || current_token.code==DATE_TOKEN
        || current_token.code==DECIMAL_TOKEN
        || current_token.code==DOUBLE_TOKEN
        || current_token.code==FLOAT_TOKEN
        || current_token.code==INET_TOKEN
        || current_token.code==INT_TOKEN
        || current_token.code==SMALLINT_TOKEN
        || current_token.code==TEXT_TOKEN
        || current_token.code==TIME_TOKEN
        || current_token.code==TIMESTAMP_TOKEN
        || current_token.code==TIMEUUID_TOKEN
        || current_token.code==TINYINT_TOKEN
        || current_token.code==UUID_TOKEN
        || current_token.code==VARCHAR_TOKEN
        || current_token.code==VARINT_TOKEN){
        current_column_type = current_token.code;
    get_lex();
}
else if(current_token.code==MAP_TOKEN){
 current_column_type = current_token.code;
 get_lex();
 _test_symbol(INF_TOKEN);
 _cql_type();
 _test_symbol(VIR_TOKEN);
 _cql_type();
 _test_symbol(SUP_TOKEN);
}
else if(current_token.code==SET_TOKEN || current_token.code==LIST_TOKEN){
    current_column_type = current_token.code;
    get_lex();
    _test_symbol(INF_TOKEN);
    _cql_type();
    _test_symbol(SUP_TOKEN);
}
else if(current_token.code==IDENTIFIER_TOKEN) {

    print_error(PARSER_ERROR, current_token.value);
    get_lex();
    if(current_token.code==P_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
    }
    else;
}
else if(current_token.code==TUPLE_TOKEN){
    current_column_type = current_token.code;
    get_lex();
    _test_symbol(INF_TOKEN);
    _cql_type();
    while(current_token.code==VIR_TOKEN){
        get_lex();
        _cql_type();
    }
    _test_symbol(SUP_TOKEN);
}
else if(current_token.code==STRING_TOKEN){
 current_column_type = current_token.code;
 get_lex();
}
else {
    print_error(PARSER_ERROR, current_token.value);
    get_lex();
}
}

void _column_defenition()
{
    char* col_name;
    bool isPrimaryKey = false;
    last_token.code = current_token.code ;
    //strcpy(last_token.value,current_token.value);
    _test_symbol(IDENTIFIER_TOKEN);

    //Contr
    col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
    strcpy(col_name, last_token.value);

    _cql_type();
    if(current_token.code == STATIC_TOKEN) {
        get_lex();
    }
    if(current_token.code == PRIMARY_TOKEN)
    {
        get_lex();
        if(current_token.code == KEY_TOKEN) {
            isPrimaryKey = true;
            get_lex();
        }
        else print_error(PARSER_ERROR, current_token.value);
    }

    // Ici on est sur que la colonne déja déclarée
    if(! search_column_CREATE(col_name , current_table->column_list)  ) {
     current_table->column_list = add_column(create_column(col_name, current_column_type) , current_table->column_list);
     if(isPrimaryKey) {
        current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
        isPrimaryKey = false;
    }
}
else {
    print_error(COLUMN_IS_ALREADY_EXISTS, col_name);

}

}

void _primary_key_ox_para()
{
    if(current_token.code == PRIMARY_TOKEN)
    {
        get_lex();
        if(current_token.code == KEY_TOKEN)
        {
            get_lex();
            _test_symbol(PO_TOKEN);
            _primary_key_def();
            _test_symbol(PF_TOKEN);
        }
        else print_error(PARSER_ERROR, current_token.value);
    }
}

void _primary_key_def()
{
    _partition_key();
    _clustring_columns();
}

void _partition_key()
{
    char *col_name = NULL;
    switch(current_token.code)
    {
        case IDENTIFIER_TOKEN :
    /*------------------------------------------------------
    ** Ici on est sur que la colonne déja déclarée
    */
        col_name = (char*) malloc(strlen(current_token.value)*sizeof(char));
        strcpy(col_name, current_token.value);
        if(search_column_CREATE(col_name , current_table->column_list)  ) {
            current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
            free(col_name);
        }
        else {
            print_error(COLUMN_IS_NOT_EXISTS, col_name);

        }
    //-------------------------------------------------------
        get_lex(); break;
        case PO_TOKEN         : get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        /*------------------------------------------------------
        ** Ici on est sur que la colonne déja déclarée
        */
        col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
        strcpy(col_name, last_token.value);
        if(search_column_CREATE(col_name , current_table->column_list)  ) {
            current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
            free(col_name);
        }
        else {
            print_error(COLUMN_IS_NOT_EXISTS, col_name);
            
        }
        //-------------------------------------------------------
        while(current_token.code == VIR_TOKEN)
        {
            get_lex();
            _test_symbol(IDENTIFIER_TOKEN);
            /*------------------------------------------------------
            ** Ici on est sur que la colonne déja déclarée
            */
            col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
            strcpy(col_name, last_token.value);
            if(search_column_CREATE(col_name , current_table->column_list)  ) {
                current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
                free(col_name);
            }
            else {
                print_error(COLUMN_IS_NOT_EXISTS, col_name);
                
            }
            //-------------------------------------------------------
        }
        _test_symbol(PF_TOKEN);
        break;
        default : print_error(PARSER_ERROR, current_token.value);
    }
}

void _clustring_columns()
{
    char *col_name = NULL;
    if(current_token.code == PF_TOKEN);
    else
    {
        _test_symbol(VIR_TOKEN);
        _test_symbol(IDENTIFIER_TOKEN);
        /*------------------------------------------------------
        ** Ici on est sur que la colonne déja déclarée
        */
        col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
        strcpy(col_name, last_token.value);
        if(search_column_CREATE(col_name , current_table->column_list)  ) {
            current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
            free(col_name);
        }
        else {
            print_error(COLUMN_IS_NOT_EXISTS, col_name);
            
        }
        //-------------------------------------------------------
        while(current_token.code == VIR_TOKEN)
        {
            get_lex();
            _test_symbol(IDENTIFIER_TOKEN);
            /*------------------------------------------------------
            ** Ici on est sur que la colonne déja déclarée
            */
            col_name = (char*) malloc(strlen(last_token.value)*sizeof(char));
            strcpy(col_name, last_token.value);
            if(search_column_CREATE(col_name , current_table->column_list)  ) {
                current_table->primary_key_list = add_column(create_column(col_name, NULL_TOKEN) , current_table->primary_key_list);
                free(col_name);
            }
            else {
                print_error(COLUMN_IS_NOT_EXISTS, col_name);
                
            }
            //-------------------------------------------------------
        }
    }
}

void _table_options()
{
    switch(current_token.code)
    {
        case COMPACT_TOKEN      : get_lex();
        _test_symbol(STORAGE_TOKEN);
        _table_options_ox();
        break;
        case CLUSTERING_TOKEN   : get_lex();
        _test_symbol(ORDER_TOKEN);
        _test_symbol(BY_TOKEN);
        _test_symbol(PO_TOKEN);
        _clustering_oredr();
        _test_symbol(PF_TOKEN);
        _table_options_ox();
        break;
        case IDENTIFIER_TOKEN   : _options(); break;
        default : print_error(PARSER_ERROR, current_token.value);
    }
}

void _table_options_ox()
{
    if(current_token.code == COMPACT_TOKEN
        || current_token.code == CLUSTERING_TOKEN
        || current_token.code == IDENTIFIER_TOKEN) _table_options();
}

void _clustering_oredr()
{
    _test_symbol(IDENTIFIER_TOKEN);
    _asc_desc();
    while(current_token.code == VIR_TOKEN)
    {
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _asc_desc();
    }
}

void _asc_desc()
{
    switch(current_token.code)
    {
        case ASC_TOKEN  : get_lex(); break;
        case DESC_TOKEN : get_lex(); break;
        default : print_error(PARSER_ERROR, current_token.value); get_lex();
    }
}

void _index_identifier()
{

    if(current_token.code == IDENTIFIER_TOKEN)
        get_lex();
    else if ( current_token.code == KEYS_TOKEN || current_token.code == VALUES_TOKEN || current_token.code == ENTRIES_TOKEN || current_token.code == FULL_TOKEN )
    {
        get_lex();
        _test_symbol(PO_TOKEN);
        _test_symbol(IDENTIFIER_TOKEN);
        _test_symbol(PF_TOKEN);
    }
    else
        print_error(PARSER_ERROR, current_token.value);
}

void _index_option()
{
    _test_symbol(WITH_TOKEN);
    _test_symbol(OPTIONS_TOKEN);
    _test_symbol(EG_TOKEN);
    _test_symbol(ACOLADO_TOKEN);
    _map_literal();
}

void _arg_dec()
{
    _test_symbol(IDENTIFIER_TOKEN);
    _cql_type();
    while( current_token.code == VIR_TOKEN )
    {
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _cql_type();
    }
}

void _options()
{
    _option();
    while(current_token.code == AND_TOKEN)
    {
        get_lex();
        _option();
    }
}

void _option()
{
    _test_symbol(IDENTIFIER_TOKEN);
    _test_symbol(EG_TOKEN);
    switch(current_token.code)
    {
    //Soit IDF
        case IDENTIFIER_TOKEN   : get_lex(); break;
        //Soit map_literal
        case ACOLADO_TOKEN      : get_lex(); _map_literal(); break;
        //Soit const
        default                 : _constant();
    }
}

void _constant()
{
    // Le traitement du cas IN
    if( op == IN_TOKEN)
    {
        if ( ( cloningToking.code == TEXT_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
           ( cloningToking.code == VARCHAR_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
           ( cloningToking.code == INT_TOKEN && current_token.code == INT_VALUE_TOKEN ) ||
           ( cloningToking.code == FLOAT_TOKEN && current_token.code == FLOAT_VALUE_TOKEN) ||
           ( cloningToking.code == BOOLEAN_TOKEN && current_token.code == BOOLEAN_VALUE_TOKEN) ||
           ( cloningToking.code == UUID_TOKEN && current_token.code == UUID_VALUE_TOKEN )
           )
        {
            Head_liste_IN = add_value( create_value(current_token) , Head_liste_IN);
        }
        else
        {
            // Incompatible Type
            print_error(INCOMPATIBLE_TYPE, "");
            print_error_type(cloningToking, current_token);
        }

    }
    else 
    { 

        if(CURRENT_PARENT_INST == INSERT_TOKEN) {
        //controle semantic : le type de colonne doit etre INT et le contenu de type INT_VALUES...)
            token_t Colonne =  extract_type_token(Head_Column_Actuel ,Indice_courant_value);
            Indice_courant_value++;

            if (Colonne.code == NULL_TOKEN)
                print_error(OVERTAKING_OF_ARGUMENTS, current_token.value);

            if ( ( Colonne.code == TEXT_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
               ( Colonne.code == VARCHAR_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
               ( Colonne.code == INT_TOKEN && current_token.code == INT_VALUE_TOKEN ) ||
               ( Colonne.code == FLOAT_TOKEN && current_token.code == FLOAT_VALUE_TOKEN) ||
               ( Colonne.code == BOOLEAN_TOKEN && current_token.code == BOOLEAN_VALUE_TOKEN) ||
               ( Colonne.code == UUID_TOKEN && current_token.code == UUID_VALUE_TOKEN ) ||
               ( current_token.code  == NULL_TOKEN )
               )
            {
                Head_liste_Values = add_value( create_value(current_token) , Head_liste_Values);
            }
            else
            {
            // Incompatible Type
                if(Colonne.code != NULL_TOKEN) {
                    print_error(INCOMPATIBLE_TYPE, "");
                    print_error_type(Colonne, current_token);
                }
            }
        }
    // Erreur Semantique de de Select
        else if (CURRENT_PARENT_INST == SELECT_TOKEN ||  CURRENT_PARENT_INST == UPDATE_TOKEN || CURRENT_PARENT_INST == DELETE_TOKEN  )
        {
        // Creation de la condition where    
            if (EXIST_CONDITION ){
                cloningToking.code  = search_column_INSERT(cloningToking.value,load_current_table_columns());
                headConditions = add_condition( create_condition(cloningToking , current_token.value , op ) , headConditions );
            }
            if(CURRENT_PARENT_INST == SELECT_TOKEN)
            {
                code_token_t code = search_column_INSERT(cloningToking.value,load_current_table_columns());
                // where age > 18   true where age = "issam" false because age is typed Int or Float
                if( !((code == TEXT_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
                   ( code == VARCHAR_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
                   ( code == INT_TOKEN && current_token.code == INT_VALUE_TOKEN ) ||
                   ( code == FLOAT_TOKEN && current_token.code == FLOAT_VALUE_TOKEN) ||
                   ( code == BOOLEAN_TOKEN && current_token.code == BOOLEAN_VALUE_TOKEN) ||
                   ( code == UUID_TOKEN && current_token.code == UUID_VALUE_TOKEN )
                   )
                 )
                {
                    print_error(INCOMPATIBLE_TYPE, "");
                    cloningToking.code = code;
                    print_error_type(cloningToking, current_token);
                }
            }
            else if(CURRENT_PARENT_INST == UPDATE_TOKEN || CURRENT_PARENT_INST == DELETE_TOKEN )
            {
                if(search_column_indice(update_token.value,load_current_table_columns()) < 0)
                    print_error(COLUMN_IS_NOT_EXISTS , update_token.value);

                code_token_t code = search_column_INSERT(update_token.value ,load_current_table_columns());
                // where age > 18   true where age = "issam" false because age is typed Int or Float
                //print_current_token();
                if( !((code == TEXT_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
                   ( code == VARCHAR_TOKEN && (current_token.code == TEXT_VALUE_TOKEN || current_token.code  == STRING_TOKEN ) )  ||
                   ( code == INT_TOKEN && current_token.code == INT_VALUE_TOKEN ) ||
                   ( code == FLOAT_TOKEN && current_token.code == FLOAT_VALUE_TOKEN) ||
                   ( code == BOOLEAN_TOKEN && current_token.code == BOOLEAN_VALUE_TOKEN) ||
                   ( code == UUID_TOKEN && current_token.code == UUID_VALUE_TOKEN )
                   )
                 )
                {
                    print_error(INCOMPATIBLE_TYPE, "");
                    cloningToking.code = code;
                    print_error_type(cloningToking, current_token);
                }
            }
        }

        switch(current_token.code)
        {
            case STRING_TOKEN       : get_lex(); break;
            case INT_VALUE_TOKEN    : get_lex(); break;
            case FLOAT_VALUE_TOKEN  : get_lex(); break;
            case BOOLEAN_VALUE_TOKEN: get_lex(); break;
            case UUID_VALUE_TOKEN   : get_lex(); break;
            case BLOB_VALUE_TOKEN   : get_lex(); break;
            case NULL_TOKEN         : get_lex(); break;
            default                 : print_error(PARSER_ERROR, current_token.value);
        }
    }
}

void _map_literal()
{
    _term();

    _test_symbol(DEUXP_TOKEN);
    _term();
    while(current_token.code == VIR_TOKEN)
    {
        get_lex();
        _term();
        while(current_token.code == DEUXP_TOKEN)
        {
            get_lex();
            _term();
        }
    }
    _test_symbol(ACOLADF_TOKEN);
}

void _term()
{
    switch(current_token.code)
    {
        case ACOLADO_TOKEN      : get_lex(); _aux_1();  _test_symbol(ACOLADF_TOKEN); break;
        case CROCHO_TOKEN       : get_lex(); _aux_2();  _test_symbol(CROCHF_TOKEN);  break;
        case PO_TOKEN           : get_lex(); _aux_3();                               break;
        //IDFprint_current_token();
        case IDENTIFIER_TOKEN   :
        //Controle semantic
        CONDITION = (char*) malloc(strlen(current_token.value) * sizeof(char));
        strcpy(CONDITION, current_token.value);

        get_lex();
        _test_symbol(PO_TOKEN);
        _aux_4();
        _test_symbol(PF_TOKEN);
        break;
        //
        case PI_TOKEN           : get_lex(); break;
        case DEUXP_TOKEN        : get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        break;
        //Const
        default                 :
        if(op != IN_TOKEN)
            _constant();
        //Controle pour la val de condition
        CONDITION = (char*) malloc(strlen(last_token.value) * sizeof(char));
        strcpy(CONDITION, last_token.value);

        //Controle for les new val de update
        if(CURRENT_PARENT_INST == UPDATE_TOKEN && !EXIST_CONDITION) {
            updating_data = add_data_insert(create_data_insert(update_token.value, last_token.value), updating_data);
        }

    }
}

void _opt_data()
{
    switch (current_token.code)
    {
        case ACOLADO_TOKEN  : get_lex(); _aux_1(); _test_symbol(ACOLADF_TOKEN);    break;
        case CROCHO_TOKEN   : get_lex(); _aux_2(); _test_symbol(CROCHF_TOKEN);     break;
        case PO_TOKEN       : get_lex(); _aux_3();                                 break;
        case PI_TOKEN       : get_lex();                                           break;
        case DEUXP_TOKEN    : get_lex(); _test_symbol(IDENTIFIER_TOKEN);           break;
        default             : _constant();                                         break;
    }
}

void _aux_1()
{

    if(current_token.code == ACOLADF_TOKEN);
    else {
        if(current_token.code == IDENTIFIER_TOKEN)
        {
            get_lex();
            _aux_1_2();

        }
        else {
            _opt_data();
            _aux_1_1();
        }
    }
}

void _aux_2()
{
    if(current_token.code == CROCHF_TOKEN);
    else {
        _term();
        while(current_token.code == VIR_TOKEN) {
            get_lex();
            _term();
        }
        _test_symbol(PF_TOKEN);
    }
}

void _aux_3()
{
    if(current_token.code == ACOLADO_TOKEN
        || current_token.code == CROCHO_TOKEN
        || current_token.code == PO_TOKEN
        || current_token.code == IDENTIFIER_TOKEN
        || current_token.code == PI_TOKEN
        || current_token.code == DEUXP_TOKEN
        || current_token.code == STRING_TOKEN
        || current_token.code == INT_VALUE_TOKEN
        || current_token.code == FLOAT_VALUE_TOKEN
        || current_token.code == BOOLEAN_VALUE_TOKEN
        || current_token.code == UUID_VALUE_TOKEN
        || current_token.code == BLOB_VALUE_TOKEN
        || current_token.code == NULL_TOKEN)
    {
        _term();
        while(current_token.code == VIR_TOKEN) {
            get_lex();
            _term();
        }
        _test_symbol(PF_TOKEN);
    }
    else {
        _cql_type();
        _test_symbol(PF_TOKEN);
        _term();
    }
}

void _aux_4()
{
    if(current_token.code == PF_TOKEN);
    else {
        _term();
        while(current_token.code == VIR_TOKEN) {
            get_lex();
            _term();
        }
    }
}

void _aux_1_1()
{
    if(current_token.code == DEUXP_TOKEN) {
        get_lex();
        _term();
        while(current_token.code == VIR_TOKEN)
        {
            get_lex();
            _term();
            _test_symbol(DEUXP_TOKEN);
            _term();
        }
    }
    else if(current_token.code == VIR_TOKEN) {
        while(current_token.code == VIR_TOKEN) {
            get_lex();
            _term();
        }
    }
    else {
        print_error(PARSER_ERROR, current_token.value);
        get_lex();
    }
}

void _aux_1_2()
{
    switch(current_token.code)
    {
        case DEUXP_TOKEN    : get_lex();
        _term();
        while(current_token.code == VIR_TOKEN)
        {
            get_lex();
            _test_symbol(IDENTIFIER_TOKEN);
            _test_symbol(DEUXP_TOKEN);
            _term();
        }

        break;
        case PO_TOKEN       : get_lex();
        _aux_4();
        _test_symbol(PF_TOKEN);
        break;
        default : print_error(PARSER_ERROR, current_token.value);

    }
}

void _if_not_exists()
{
    if(current_token.code == IF_TOKEN)
    {
        get_lex();
        if(current_token.code == NOT_TOKEN)
        {
            get_lex();
        }
        _test_symbol(EXISTS_TOKEN);

    }
}

void _tuple_literal(void){
    _test_symbol(PO_TOKEN);
    // Ici Ou il y a les test semantique
    _term();

    while(current_token.code==VIR_TOKEN){
        get_lex();
        _term();
    }
    _test_symbol(PF_TOKEN);

    if ( get_size_column(Head_Column_Actuel) > get_size_values(Head_liste_Values) )
        print_error(INSUFFICIENT_ARGUMENTS, "");
}

void _insert_statement_aux(void) {
    if(current_token.code==PO_TOKEN){

      //Head_Column_Actuel = NULL;

        get_lex();
        // Ici c'est VALUES (IDF,IDF,IDF,IDF,IDF)
        //last_token = clone_token(current_token);
        last_token.value[0] = '\0';
        strcpy(last_token.value, current_token.value);
        last_token.code = current_token.code;
        _test_symbol(IDENTIFIER_TOKEN);

        // Je doit chercher si la colonne existe déja dans la structure        // Il faut faire le Cast parce que il se peut qu elle return un NULL
        last_token.code = search_column_INSERT(last_token.value , Head_Column_Of_Table);

        // Si le code est END_TOKEN  alors il n existe pas dans la table
        if(last_token.code == END_TOKEN)
            print_error(COLUMN_IS_NOT_EXISTS, last_token.value );
        else {
                //  Ici , on est sur que le colonne existe : Ajouter avec la liste des colonnes courantes
            Head_Column_Actuel = add_column( create_column_token(last_token) , Head_Column_Actuel );
        }

        while(current_token.code==VIR_TOKEN)
        {
            get_lex();
            // Ici c'est VALUES (IDF,IDF,IDF,IDF,IDF)
            last_token.value[0] = '\0';
            last_token = clone_token(current_token);

            _test_symbol(IDENTIFIER_TOKEN);

            /// Avant il faut rechercher si jamais il y a une redandonce (age,age)
            if ( search_column_CREATE(last_token.value , Head_Column_Actuel) )
                print_error(COLUMN_IS_ALREADY_EXISTS, last_token.value);
            else
            {

            // Je doit chercher si la colonne existe déja dans la structure        // Il faut faire le Cast parce que il se peut qu elle return un NULL
                last_token.code =  search_column_INSERT(last_token.value , Head_Column_Of_Table);
            // Si le code est NULL alors il n existe pas dans la table
                if(last_token.code == END_TOKEN)
                    print_error(COLUMN_IS_NOT_EXISTS, last_token.value );

            //  Ici , on est sur que le colonne existe : Ajouter avec la liste des colonnes courantes
                Head_Column_Actuel = add_column( create_column_token(last_token) , Head_Column_Actuel );

            }
        }
        _test_symbol(PF_TOKEN);

        // Verifier Que ça marche
        //print_column(Head_Column_Actuel);
        _test_symbol(VALUES_TOKEN);
        // Dans c tuple Inserer le control semantique de VALUES
        _tuple_literal();
         //print_values(Head_liste_Values);
    }

    else if(current_token.code==VALUES_TOKEN){
        // Ici on sait qu il y avait INSERT INTO TABLENAME VALES(....)   Donc les colonnes actuelles sont les mêmes des colonnes de la table
        Head_Column_Actuel = Head_Column_Of_Table ;
        get_lex();
        _tuple_literal();
    }

    else if(current_token.code == JSON_TOKEN){
        get_lex();
        _test_symbol(STRING_TOKEN);
        _option_default();
    }
    else print_error(PARSER_ERROR, current_token.value);

}

void _option_default(void){
    if(current_token.code==DEFAULT_TOKEN){
        get_lex();
        _default_aux();
    }
    else ;//get_lex();
}

void _default_aux(void){
    if(current_token.code==NULL_TOKEN || current_token.code==UNSET_TOKEN){
        get_lex();
    }
    else print_error(PARSER_ERROR, current_token.value); get_lex();
}

void _option_selection(void){
    if(current_token.code==IDENTIFIER_TOKEN){
        get_lex();
        if(current_token.code==CROCHO_TOKEN){
            get_lex();
            _term();
        }
        else if(current_token.code==P_TOKEN){
            get_lex();
            _test_symbol(IDENTIFIER_TOKEN);
        }
        else;
    }
    else;
}


void _modification_statement(void){

    if(current_token.code==INSERT_TOKEN) _insert_statement();
    else if(current_token.code==UPDATE_TOKEN) _update_statement();
    else if(current_token.code==DELETE_TOKEN) _delete_statement();
    else {

        get_lex();
    }

}

void _option_batch(void){
    if(current_token.code==UNLOGGED_TOKEN) get_lex();
    else if(current_token.code==COUNTER_TOKEN) get_lex();
    else;
}

void _select_clause(void) {
    _selector();
    //get_lex();
    if(current_token.code==AS_TOKEN) _test_symbol(IDENTIFIER_TOKEN);
    while(current_token.code == VIR_TOKEN){
        get_lex();
        _selector();
        if(current_token.code==AS_TOKEN) _test_symbol(IDENTIFIER_TOKEN);
    }
}


void _selector(void){
    switch(current_token.code){
        case ETOILE_TOKEN:
        get_lex();
        break;
        case IDENTIFIER_TOKEN:
        //Symontic Controle
        //Stockage des colonnes a selectionnée
        if(!search_column_CREATE(current_token.value, Out))
            Out = add_column(create_column_token(current_token), Out);
        else
            print_error(COLUMN_IS_ALREADY_EXISTS, current_token.value);

        get_lex();
        _selector_aux();
        break;
        case TABLE_FUNCTION_NAME_TOKEN || TOKEN_TOKEN:
        get_lex();
        _test_symbol(PO_TOKEN);
        _option_selector();
        _test_symbol(PF_TOKEN);
        break;
        case CAST_TOKEN:
        get_lex();
        _test_symbol(PO_TOKEN);
        _selector();
        _test_symbol(AS_TOKEN);
        _cql_type();
        _test_symbol(PF_TOKEN);
        break;
        case COUNT_TOKEN:
        get_lex();
        _test_symbol(PO_TOKEN);
        _test_symbol(ETOILE_TOKEN);
        _test_symbol(PF_TOKEN);
        //Symontic controle
        IS_COUNT = true; //cette select est pour un count
        break;
        case ACOLADO_TOKEN: 
        get_lex();
        _aux_1();
        _test_symbol(ACOLADF_TOKEN);
        break;
        case CROCHO_TOKEN:
        get_lex();
        _aux_2();
        _test_symbol(CROCHF_TOKEN);
        break;
        case PO_TOKEN:
        get_lex();
        _aux_3();
        break;
        case PI_TOKEN:
        get_lex();
        break;
        case DEUXP_TOKEN:
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        break;
        case STRING_TOKEN       : _constant(); break;
        case INT_VALUE_TOKEN    : _constant(); break;
        case FLOAT_VALUE_TOKEN  : _constant(); break;
        case BOOLEAN_VALUE_TOKEN: _constant(); break;
        case UUID_VALUE_TOKEN   : _constant(); break;
        case BLOB_VALUE_TOKEN   : _constant(); break;
        case NULL_TOKEN         : _constant(); break;
        default : print_error(PARSER_ERROR, current_token.value);
    }
}

void _selector_aux(void){
    if(current_token.code == PO_TOKEN) {
        get_lex();
        _aux();
    }
    else;
}

void _option_selector(void){

    _selector();
    while(current_token.code==VIR_TOKEN){
        get_lex();
        _selector();
    }
}

void _aux(void){
    ;
}

void _where_clause(void)
{
    //print_current_token();
    EXIST_CONDITION  = true; //Controle semantic
    _relation();
    while(current_token.code==AND_TOKEN)
    {
        get_lex();
        _relation();
    }

    switch(current_token.code){
        case GROUP_TOKEN: get_lex(); _test_symbol(BY_TOKEN); _group_by_clause(); break;
        case ORDER_TOKEN: get_lex(); _test_symbol(BY_TOKEN); _order_by_clause(); break;
        case PER_TOKEN  : get_lex(); _test_symbol(PARTITION_TOKEN); _test_symbol(LIMIT_TOKEN); _option_type(); break;
        case LIMIT_TOKEN: get_lex(); _option_type(); break;
        case ALLOW_TOKEN: get_lex(); _test_symbol(FILTERING_TOKEN); break;
    default : /*printf("error : _where_clause\n");*/ break;
    }
}

void _group_by_clause(void){

   if(search_column_indice(current_token.value,load_current_table_columns()) < 0)
    print_error(COLUMN_IS_NOT_EXISTS , current_token.value);

_test_symbol(IDENTIFIER_TOKEN);
while(current_token.code==VIR_TOKEN)
{
    get_lex();
    if(search_column_indice(current_token.value,load_current_table_columns()) < 0)
        print_error(COLUMN_IS_NOT_EXISTS , current_token.value);
    _test_symbol(IDENTIFIER_TOKEN);
}
switch(current_token.code){
    case ORDER_TOKEN: get_lex();_test_symbol(BY_TOKEN); _order_by_clause(); break;
    case PER_TOKEN  : get_lex();_test_symbol(PARTITION_TOKEN); _test_symbol(LIMIT_TOKEN); _option_type(); break;
    case LIMIT_TOKEN: get_lex(); _option_type(); break;
    case ALLOW_TOKEN: _test_symbol(FILTERING_TOKEN); break;
    default : printf("error : _group_by_clause\n"); break;
}
}

void _order_by_clause(void){
    if(current_token.code==ASC_TOKEN || current_token.code==DESC_TOKEN) get_lex();
    else;
    if(search_column_indice(current_token.value,load_current_table_columns()) < 0)
        print_error(COLUMN_IS_NOT_EXISTS , current_token.value);
    _test_symbol(IDENTIFIER_TOKEN);
    while(current_token.code==VIR_TOKEN){
        get_lex();
        if(search_column_indice(current_token.value,load_current_table_columns()) < 0)
            print_error(COLUMN_IS_NOT_EXISTS , current_token.value);
        _test_symbol(IDENTIFIER_TOKEN);
        if(current_token.code==ASC_TOKEN || current_token.code==DESC_TOKEN);
        get_lex();
    }
    switch(current_token.code){
        case PER_TOKEN  : get_lex();_test_symbol(PARTITION_TOKEN); _test_symbol(LIMIT_TOKEN); _option_type(); break;
        case LIMIT_TOKEN: get_lex(); _option_type(); break;
        case ALLOW_TOKEN: _test_symbol(FILTERING_TOKEN); break;
        default : printf("error : _order_by_clause"); break;
    }
}

void _option_type(void){
    if(current_token.code==INT_TOKEN){
        get_lex();
        switch(current_token.code){
            case LIMIT_TOKEN: get_lex(); _option_type(); break;
            case ALLOW_TOKEN: get_lex(); _test_symbol(FILTERING_TOKEN); break;
            default : printf("error : _option_type\n"); break;
        }
    }
    else if(current_token.code==PI_TOKEN || current_token.code==DEUXP_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        switch(current_token.code){
            case LIMIT_TOKEN: get_lex(); _option_type(); break;
            case ALLOW_TOKEN: get_lex();_test_symbol(FILTERING_TOKEN); break;
            default : printf("error : _option_type\n"); break;
        }
    }
    else {
        //check this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        print_error(PARSER_ERROR, current_token.value);
        get_lex();
    }
}

void _relation(void){
    if(current_token.code==IDENTIFIER_TOKEN){
        cloningToking = clone_token(current_token);
        //Controle semontic , this means that we can't find the column
        if ( CURRENT_PARENT_INST == SELECT_TOKEN )
            cloningToking = clone_token(current_token);
        if ( CURRENT_PARENT_INST == UPDATE_TOKEN || CURRENT_PARENT_INST ==  DELETE_TOKEN )
            update_token =  clone_token(current_token);
        COLUMN_CONDITION = (char*) malloc(strlen(current_token.value) * sizeof(char));
        strcpy(COLUMN_CONDITION, current_token.value);
        get_lex();
        _operator();
        _term();

    }
    else if(current_token.code==PO_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        while(current_token.code==VIR_TOKEN){
            get_lex();
            _test_symbol(IDENTIFIER_TOKEN);
        }
        _test_symbol(PF_TOKEN);
        /* _operator();
       _tuple_literal();*/
    }
    else if(current_token.code==TOKEN_TOKEN){
        get_lex();
        _test_symbol(PO_TOKEN);
        _aux_4();
        while (current_token.code==VIR_TOKEN) {
            get_lex();
            _term();
        }
        _test_symbol(PF_TOKEN);

        _operator();

        _term();
    }
    else print_error(PARSER_ERROR, current_token.value);
}

void _operator(void){
    if(current_token.code==EG_TOKEN
        || current_token.code==INF_TOKEN
        || current_token.code==INFEG_TOKEN
        || current_token.code==SUP_TOKEN
        || current_token.code==SUPEG_TOKEN
        || current_token.code==DIFF_TOKEN
        ){


        cloningToking.code  = search_column_INSERT(cloningToking.value,load_current_table_columns());

    if( (current_token.code==INF_TOKEN || current_token.code==INFEG_TOKEN || current_token.code==SUP_TOKEN  || current_token.code==SUPEG_TOKEN ) 
     &&
     ( cloningToking.code==VARCHAR_TOKEN || cloningToking.code==TEXT_TOKEN ||  cloningToking.code==STRING_TOKEN ) )
    {
        print_error(INCOMPATIBLE_COMPARAISON,cloningToking.value);
    }
    else
    {
        op = current_token.code;
        get_lex();
    }
}
else if (current_token.code==IN_TOKEN)
{
    cloningToking.code  = search_column_INSERT(cloningToking.value,load_current_table_columns());
    op = current_token.code ;
    get_lex();
    _test_symbol(PO_TOKEN);
    _constant();
    get_lex();

    while(current_token.code == VIR_TOKEN)
    {
       get_lex();
       _constant();
       get_lex();
   }
   _test_symbol(PF_TOKEN);
}
else if(current_token.code==CONTAINS_TOKEN){
    get_lex();
    _test_symbol(KEY_TOKEN);
}
    else print_error(PARSER_ERROR, current_token.value); //get_lex();

}

void _option_using(void){
    if(current_token.code==USING_TOKEN){

        get_lex();
        _update_parametre();
        while(current_token.code==VIR_TOKEN){
            get_lex();
            _update_parametre();
        }
    }
    else;
}

void _update_parametre(void){

    _update_parametre_aux();
    _option_type_aux();
}

void _update_parametre_aux(void){
    if(current_token.code==TIMESTAMP_TOKEN || current_token.code==TTL_TOKEN){
        get_lex();
    }
    else print_error(PARSER_ERROR, current_token.value); //get_lex();

}

void _option_type_aux(void){

    if(current_token.code==INT_VALUE_TOKEN){

        get_lex();
    }
    else if(current_token.code==PI_TOKEN || current_token.code==DEUXP_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
    }
    else print_error(PARSER_ERROR, current_token.value); //get_lex();

}

void _assignment(void){
    _test_symbol(IDENTIFIER_TOKEN);
    //Controle
    if (CURRENT_PARENT_INST == SELECT_TOKEN)
     cloningToking  = clone_token(last_token);
 if ( CURRENT_PARENT_INST == UPDATE_TOKEN || CURRENT_PARENT_INST == DELETE_TOKEN )
    update_token  = clone_token(last_token);


_assignment_aux();
}

void _assignment_aux(void){
    if(current_token.code==CROCHO_TOKEN){
        get_lex();
        _term();
        _test_symbol(CROCHF_TOKEN);
        _test_symbol(EG_TOKEN);
        _term();
    }

    // to do check field_name
    else if(current_token.code==P_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _test_symbol(EG_TOKEN);
        _term();
    }
    else if(current_token.code==EG_TOKEN){
        get_lex();
        //_aff_column_name(); //À revoir par rapport à la grammaire
        _term();
    }
    else print_error(PARSER_ERROR, current_token.value); //get_lex();


}

void _aff_column_name(void){
    if(current_token.code==IDENTIFIER_TOKEN){
        get_lex();
        if(current_token.code==PLUS_TOKEN || current_token.code==MOINS_TOKEN);
        else print_error(PARSER_ERROR, current_token.value);
        get_lex();
        _term();
    }
    else if(current_token.code==CROCHO_TOKEN){
        get_lex();
        if(current_token.code!=CROCHF_TOKEN){
            _option_list_literal();
        }
        _test_symbol(CROCHF_TOKEN);
        _test_symbol(PLUS_TOKEN);
        _test_symbol(IDENTIFIER_TOKEN);
    }

    else _term();




}

void _option_list_literal(void){
    _term();
    while(current_token.code==VIR_TOKEN){
        get_lex();
        _term();
    }
}

void _option_if(void){
    if(current_token.code==IF_TOKEN){
        get_lex();
        _option_if_aux();
    }
    else;
}

void _option_if_aux(void){
    if(current_token.code==EXISTS_TOKEN) {
        get_lex();
    }
    else if(current_token.code==IDENTIFIER_TOKEN){
        _condition();
        while(current_token.code==AND_TOKEN){
            get_lex();
            _condition();
        }
    }
    else print_error(PARSER_ERROR, current_token.value);
}

void _condition(void){
    _test_symbol(IDENTIFIER_TOKEN);
    if(current_token.code==CROCHO_TOKEN){
        get_lex();
        _term();
        _test_symbol(CROCHF_TOKEN);
        _operator();
        _term();
    }
    else if(current_token.code==P_TOKEN){
        get_lex();
        _test_symbol(IDENTIFIER_TOKEN);
        _operator();
        _term();
    }
    else {
        _operator();
        _term();
    }

}

//grant options

void _permissions() {
    if(current_token.code==ALL_TOKEN){
        current_grant->permission_name = ALL_TOKEN;
        get_lex();
        if(current_token.code==PERMISSIONS_TOKEN){
            get_lex();
        }
        else
            {get_lex();}
    }
    else {
        _permission();
        if(current_token.code==PERMISSION_TOKEN)
            get_lex();
    }
}

void _ressource(){
    switch(current_token.code){
        case ALL_TOKEN :
            current_grant->resource.r_key   = ALL_TOKEN;
            get_lex(); 
            _all_statement(); 
        break;
        case KEYSPACE_TOKEN :
            get_lex(); 
            _test_symbol(IDENTIFIER_TOKEN);
            current_grant->resource.r_key   = KEYSPACE_TOKEN;
            strcpy(current_grant->resource.r_value.v_Others, last_token.value);
        break;
        case TABLE_TOKEN :
            get_lex();
            if(current_token.code==IDENTIFIER_TOKEN || current_token.code==TABLE_FUNCTION_NAME_TOKEN) get_lex();
            else{
                print_error(PARSER_ERROR, current_token.value);
                get_lex();
            }
            current_grant->resource.r_key   = TABLE_TOKEN;
            strcpy(current_grant->resource.r_value.v_Others, last_token.value);
        break;
        case IDENTIFIER_TOKEN :             
            get_lex();
        break;
        case TABLE_FUNCTION_NAME_TOKEN :    
            get_lex();
        break;
        case ROLE_TOKEN :
            get_lex();
            if(current_token.code==IDENTIFIER_TOKEN|| current_token.code==STRING_TOKEN) get_lex(); 
        break;
        case FUNCTION_TOKEN :    
            get_lex();
            _test_symbol(TABLE_FUNCTION_NAME_TOKEN);
            _test_symbol(PO_TOKEN);
            _function_statement();
            _test_symbol(PF_TOKEN); 
        break;
        case MBEAN_TOKEN :
            _test_symbol(STRING_TOKEN);
        break;
        case MBEANS_TOKEN :
            _test_symbol(STRING_TOKEN);
        break;
        default : 
            printf("\nressource_ERROR\n");
            get_lex(); 
        break;


    }
}
void _permission(){
    switch(current_token.code){
        case CREATE_TOKEN   : current_grant->permission_name = CREATE_TOKEN;     get_lex(); break;
        case ALTER_TOKEN    : current_grant->permission_name = ALTER_TOKEN;      get_lex();break;
        case DROP_TOKEN     : current_grant->permission_name = DROP_TOKEN;       get_lex();break;
        case SELECT_TOKEN   : current_grant->permission_name = SELECT_TOKEN;     get_lex();break;
        case MODIFY_TOKEN   : current_grant->permission_name = MODIFY_TOKEN;     get_lex();break;
        case AUTHORIZE_TOKEN: current_grant->permission_name = AUTHORIZE_TOKEN;  get_lex();break;
        case DESCRIBE_TOKEN : current_grant->permission_name = DESCRIBE_TOKEN;   get_lex();break;
        case EXECUTE_TOKEN  : current_grant->permission_name = EXECUTE_TOKEN;    get_lex();break;
        default             : printf("\npermission_ERROR\n");get_lex(); break;
    }
}

void _all_statement(){
    switch(current_token.code){
        case KEYSPACES_TOKEN : current_grant->resource.r_value.v_Code = KEYSPACES_TOKEN; get_lex();break;
        case ROLES_TOKEN     : get_lex();break;
        case FUNCTIONS_TOKEN : get_lex(); _all_functions_statement();break;
        case MBEANS_TOKEN    : get_lex();break;
        default              : printf("\nall statement_ERROR\n");get_lex(); break;

    }
}

void _all_functions_statement(){
    if(current_token.code==IN_TOKEN){
        get_lex();
        _test_symbol(KEYSPACE_TOKEN);
        _test_symbol(IDENTIFIER_TOKEN);
    }
    else{
        get_lex();
    }
}


void _function_statement(){

    if(current_token.code!=PF_TOKEN){
        _cql_type();
        while(current_token.code==VIR_TOKEN){
            get_lex();
            _cql_type();
        }
    }
    else get_lex();
}

//list options

void _of_role_statement(){
    if(current_token.code==OF_TOKEN){
        get_lex();
        if(current_token.code==IDENTIFIER_TOKEN|| current_token.code==STRING_TOKEN){get_lex();}
        else {print_error(PARSER_ERROR, current_token.value);get_lex();}


    }

}

void _no_recursive_statement(){
    if(current_token.code==NORECURSIVE_TOKEN)
        get_lex();
}

void _on_ressource_statement(){
    if(current_token.code==ON_TOKEN){
        get_lex();
        _ressource();
    }

}


void _of_role_name_statement(){
    if(current_token.code==OF_TOKEN){
        get_lex();
        if(current_token.code==IDENTIFIER_TOKEN || current_token.code==STRING_TOKEN)get_lex();
        else {print_error(PARSER_ERROR, current_token.value);get_lex();}
        _no_recursive_statement();


    }
}
