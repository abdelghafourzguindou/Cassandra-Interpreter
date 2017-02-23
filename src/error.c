#include "error.h"

static bool SEMANTIC_STATE  = true ;
static bool SYNTAXIC_STATE  = true ;
static bool is_type_error    = false;
bool no_error = true;

void print_error(error_code_t err, char *flage) {
    if(no_error) {
    set_semantic_state(false);
    set_syntaxic_state(false);

    //printf("\n%serror%s ligne %s%d%s : %s%s%s ", RED, KGRN, NC, ligne_number, KWHT, BLUE, flage, NC);
    printf("\n%serror%s ligne %s%d%s : ", RED, KGRN, NC, ligne_number, KWHT);

    switch(err) 
    {
        case SCANNER_ERROR               : printf("code = 2000 [Invalid query] %s%s%s undefined symbol\n",                      BLUE, flage, RED);                          break;
        case PARSER_ERROR                : printf("code = 2001 [Invalid query] %s%s%s unexpected here\n",                       BLUE, flage, RED);                          break;
        case KEYSPACE_IS_NOT_EXISTS      : printf("code = 2002 [Invalid query] %s%s%s keyspace is not exists\n",                BLUE, flage, RED);                          break;
        case TABLE_IS_NOT_EXISTS         : printf("code = 2003 [Invalid query] %s%s%s table is not exists\n",                   BLUE, flage, RED);                          break;
        case COLUMN_IS_NOT_EXISTS        : printf("code = 2004 [Invalid query] %s%s%s column is not exists\n",                  BLUE, flage, RED);                          break;
        case FUNCTION_IS_NOT_EXISTS      : printf("code = 2005 [Invalid query] %s%s%s function is not exists\n",                BLUE, flage, RED);                          break;
        case KEYSPACE_IS_ALREADY_EXISTS  : printf("code = 2006 [Invalid query] %s%s%s keyspace is already exists\n",            BLUE, flage, RED);                          break;
        case TABLE_IS_ALREADY_EXISTS     : printf("code = 2007 [Invalid query] %s%s%s table is already exists\n",               BLUE, flage, RED);                          break;
        case COLUMN_IS_ALREADY_EXISTS    : printf("code = 2008 [Invalid query] %s%s%s column is already exists\n",              BLUE, flage, RED);                          break;
        case FUNCTION_IS_ALREADY_EXISTS  : printf("code = 2009 [Invalid query] %s%s%s function is already exists\n",            BLUE, flage, RED);                          break;
        case INCOMPATIBLE_TYPE           : printf("code = 2010 [Invalid query] %s%s%s incompatible type ",                      BLUE, flage, RED); is_type_error = true;    break;
        case OVERTAKING_OF_ARGUMENTS     : printf("code = 2011 [Invalid query] %s%s%s overtaking of arguments\n",               BLUE, flage, RED);                          break;
        case INSUFFICIENT_ARGUMENTS      : printf("code = 2012 [Invalid query] %s%s%s insufficient arguments\n",                BLUE, flage, RED);                          break;
        case ROW_IS_ALREADY_EXISTS       : printf("code = 2013 [Invalid query] %s%s%s row is already exists in DB\n",           BLUE, flage, RED);                          break;
        case UNUSED_KEYSPACE             : printf("code = 2014 [Invalid query] %s%s%s unused keyspace\n",                       BLUE, flage, RED);                          break;
        case COLUMN_IS_ALREADY_ADDED     : printf("code = 2015 [Invalid query] %s%s%s column is already added\n",               BLUE, flage, RED);                          break;
        case COLUMN_IS_ALREADY_ALTERED   : printf("code = 2016 [Invalid query] %s%s%s column is already altered\n",             BLUE, flage, RED);                          break;
        case INCOMPATIBLE_COMPARAISON    : printf("code = 2017 [Invalid query] %s%s%s incompatible comparaison\n",              BLUE, flage, RED);                          break;
        case ALTER_DENIED                : printf("code = 2018 [Invalid query] %s%s%s alter is denied because data is found\n", BLUE, flage, RED);                          break;
        case USER_ALREADY_EXISTS         : printf("code = 2019 [Invalid query] %s%s%s user is already exists\n",                BLUE, flage, RED);                          break;
        case USER_IS_NOT_EXISTS          : printf("code = 2020 [Invalid query] %s%s%s user is not exists\n",                    BLUE, flage, RED);                          break;
        default                          : break;
    }
    }
    //exit(0);
    printf("%s", NC);
    no_error = false;
}

print_error_type(token_t t1, token_t t2) {
    if(is_type_error) {
        printf("%s%s%s has a %s", BLUE, t1.value, NC, YELLOW);
        print_type(t1.code);
        printf("%s type but %s%s %shas a %s", NC, BLUE, t2.value, NC, YELLOW);
        print_type(t2.code);
        printf("%s type\n", NC);
    }
}

bool get_semantic_state() {
    return SEMANTIC_STATE;
}

void set_semantic_state(bool ETAT) {
    SEMANTIC_STATE = ETAT;
}

bool get_syntaxic_state() {
    return SYNTAXIC_STATE;
}

void set_syntaxic_state(bool ETAT) {
    SYNTAXIC_STATE = ETAT ;
}

void expected_words(int num, ...)
{
    va_list valist;
    int i, indice_tocken ;
    //initialize valist for num number of arguments
    va_start(valist,num);
    //access all the arguments assigned to valist
    printf("Error Ligne %d Column %d  --->   Erreur Syntaxique : %s Inattendu Ici !\n", ligne_number,column_number,current_token.value);
    printf("Les Symboles attendu Ici Sont  :  \n");
    for (i = 0; i < num; i++)
    {
        print_token(va_arg(valist, int));
        //Traitement ici des erreurs
    }
    printf("\n");
    //clean memory reserved for valist
    va_end(valist);
}

void reset_states() {
    set_semantic_state(true);
    set_syntaxic_state(true);
    CURRENT_PARENT_INST     = NULL_TOKEN;
    CURRENT_CHILD_INST      = NULL_TOKEN;
    Indice_courant_value    = 1;
}