#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <jansson.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"
#include "parser.h"
#include "semantic.h"

#define KEYSPACE_PATH_SIZE 	150
#define TABLE_PATH_SIZE 	255

static const char	*directory;
static char 		current_keyspace_path[KEYSPACE_PATH_SIZE];
static char 		current_table_path[TABLE_PATH_SIZE];
static bool 		keyspace_is_used;

column_t* load_current_table_columns();
column_t* load_current_table_primary_keys();
column_t* load_table_columns(char*);


void set_current_keyspace_path(const char*);
char* get_current_keyspace_path();
void set_current_table_path(const char*);
char* get_current_table_path();
bool get_State_Current_Keyspace();
void set_State_Current_Keyspace(bool);

void interpret();
void interpret_use();
void interpret_create();
void interpret_alter();
void interpret_drop();
void interpret_insert();
void interpret_update();
void interpret_delete();
void interpret_select();
void interpret_truncute();
void interpret_grant();

void interpret_create_keyspace();
void interpret_create_table();
void interpret_create_user();

void interpret_drop_table();
void interpret_drop_user();

void interpret_alter_table();
void interpret_alter_user();

data_insert_t* create_all_data_insert();
data_insert_t* add_data_insert(data_insert_t*  , data_insert_t* );
data_insert_t*  create_data_insert(char * key, char * value);

json_t *select_rows_where(json_t*);

#endif // INTERPRETER_H
