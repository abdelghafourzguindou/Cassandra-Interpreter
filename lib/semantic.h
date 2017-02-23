#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <errno.h>

#include "token.h"
#include "scanner.h"

#define NAME_SIZE 25

// ************************************** CFG FOR CREATE TABLE     ************************************** //
typedef struct _column {
    char column_name[NAME_SIZE];
    code_token_t column_type;
    struct _column* next;
} column_t;


typedef struct _table {
    char table_name[NAME_SIZE];
    column_t* column_list;
    column_t* primary_key_list;
} table_t;


 // the function  serch if there is any table in the current Keyspace with the same name
column_t* is_exist_table(char* ,int );

bool is_exist_keyspace(const char*);

char current_keyspace_name[NAME_SIZE];
table_t* current_table;
code_token_t current_column_type;

column_t* create_column(const char*, code_token_t);
column_t* create_column_token(token_t );
table_t* create_table(const char * table_name);

// add the 1ére parameter in the last rang of 2éme parameter
column_t* add_column(column_t*, column_t*);
void remove_column(const char*);
// Return NULL IF char* dosnt exist or column pointer if exist
bool search_column_CREATE(const char* , column_t*);

//Debug
void print_keyspaces();
void print_table(table_t*);
void print_column(column_t*);


// ************************************** CFG FOR INSERT INTO     ************************************** //

// CFG pour stocker les valeurs d'une INSERT

typedef struct _token_t_list
{
    token_t token_v;
    struct _token_t_list *next;
} token_t_list;

typedef struct _INST_INSERT
{
    column_t*  Column_Of_Table;
    column_t*  Column_Actuel;
    token_t_list*  liste_Values;
} INST_INSERT;

// we have to fill Head_Column_Of_Table from Json and Head_Column_Actuel from the present
column_t*       Head_Column_Of_Table;
column_t*       Head_Column_Actuel;
token_t_list*   Head_liste_Values;

INST_INSERT* Instruction_Insert;

code_token_t search_column_INSERT(const char* , column_t*);

INST_INSERT* create_insert_inst(column_t* , column_t*  , token_t*) ;

// this function allow as to discover how many column exist and compare them with the number of the arguments

int get_size_column(column_t*);
int get_size_values(token_t_list*);


//Pour ajouter à la liste des valeurs
token_t_list* add_value(token_t_list* ,token_t_list*);

token_t_list* create_value(token_t);

void print_values(token_t_list*);   

// INTO (age,nom,prenom) VALUES ( .... )      ça permet de relever le type d un indice
token_t extract_type_token(column_t* , int );
// Interpretation :   return -1 si on va pas trouver le nom dans la liste des colonnes
int search_column_indice(const char* , column_t* );
// Interpretation : return la value à partir d'un indice de liste values INTO(5,.....)
char* extract_from_values(token_t_list*  , int );

/*
**CFG pour insert et update pour les données a interpreter
**-------------------------------------------------------------------
*/
typedef struct _data_insert
{
    char key[50];
    char value[50];
    struct _data_insert *next;
} data_insert_t;
//-------------------------------------------------------------------

code_token_t ALTER_TABLE_ORDER; //Variable pour controler ALTER TABLE


//Variable de controle pour select, update,
typedef struct _where_condition {
    token_t column_condition; //Column de condition
    char condition[100];        //Condition sur column de condition
    code_token_t op;            //Operateur de condition
    struct _where_condition *next;
} where_condition_t;


token_t cloningToking; //Variable pour controler le type des colonnes de select

extern column_t      *Out;                //Tableu pour stocker les colonne a selectionner
extern char          *COLUMN_CONDITION;   //Column de condition
extern char          *CONDITION;          //condition de where
extern bool          EXIST_CONDITION;     //pour controler l'existance de condition
extern bool          IS_COUNT;            //pour verifier si une select count qui declonche
extern data_insert_t *updating_data;      //pour stocker les colones consernant l'update et aussi sa neuvelle valeurs
extern where_condition_t *headConditions;
extern code_token_t op ;
extern token_t_list*   Head_liste_IN;   // pour controler IN (.....)
where_condition_t* create_condition(token_t, char* ,code_token_t);
// Ajouter la where_condition_t avec les autres qui seront les conditions
where_condition_t* add_condition(where_condition_t*, where_condition_t*);
// Affichage de la liste des colonnes d'une tables
void print_Condition(where_condition_t *);


//Create user statement
typedef struct _user
{
    char user_name      [NAME_SIZE];
    char user_password  [NAME_SIZE];
    code_token_t user_option;
} user_t;
extern user_t *current_user;

void set_current_user(user_t);

//Grant
typedef struct _grant
{
    char user_name  [NAME_SIZE];
    code_token_t permission_name;
    struct _resource {
        code_token_t r_key;
        union r_value {
            code_token_t v_Code;
            char v_Others[NAME_SIZE];
        } r_value;
    } resource;
} grant_t;
grant_t *current_grant;


void reset_parameter();

#endif // SEMANTIC_H
