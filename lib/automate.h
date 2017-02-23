#ifndef AUTOMATE_H
#define AUTOMATE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CARACTER 256

int** transation_table;

void inisializing(int);
char* clone(char*);

bool is_table_function_name(char*);
bool is_identifier(char*);
bool is_string(char*);
bool is_integer(char*);
bool is_float(char*);
bool is_uuid(char*);
bool is_boolean(char*);
bool is_hex(char);
bool is_blob(char*);

#endif // AUTOMATE_H
