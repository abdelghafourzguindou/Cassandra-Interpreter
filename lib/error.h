#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include "parser.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define RED    "\033[1;31m"
#define GREEN  "\033[1;32m"
#define BLUE   "\033[1;34m"
#define YELLOW "\033[1;33m"
#define NC     "\033[1;0m"

static bool SEMANTIC_STATE;
static bool SYNTAXIC_STATE;
extern bool no_error;
static bool is_type_error;

typedef enum _error_code {
    //Pour controler les erreus lexicale
    SCANNER_ERROR,
    //Pour controler les erreurs syntaxique
    PARSER_ERROR,
    //Pour controler les erreurs semantique
    KEYSPACE_IS_NOT_EXISTS,
    TABLE_IS_NOT_EXISTS,
    COLUMN_IS_NOT_EXISTS,
    FUNCTION_IS_NOT_EXISTS,
    KEYSPACE_IS_ALREADY_EXISTS,
    TABLE_IS_ALREADY_EXISTS,
    COLUMN_IS_ALREADY_EXISTS,
    FUNCTION_IS_ALREADY_EXISTS,
    INCOMPATIBLE_TYPE,
    OVERTAKING_OF_ARGUMENTS,
    INSUFFICIENT_ARGUMENTS,
    ROW_IS_ALREADY_EXISTS,
    UNUSED_KEYSPACE,
    COLUMN_IS_ALREADY_ADDED,
    COLUMN_IS_ALREADY_ALTERED,
    INCOMPATIBLE_COMPARAISON,
    ALTER_DENIED,
    USER_ALREADY_EXISTS,
    USER_IS_NOT_EXISTS

} error_code_t;

////Pour les erreurs syntaxique
void expected_words(int, ...);

////Pour les erreurs semantique
void print_error(error_code_t, char*);
void print_error_type(token_t, token_t);
//Getters et setters pour l'etat de l'analyseur semantique
bool get_semantic_state();
void set_semantic_state(bool);
//Getters et setters pour l'etar de l'analyseur syntaxique
bool get_syntaxic_state();
void set_syntaxic_state(bool);

int get_errors_number();
void reset_states();

#endif // ERROR_H
