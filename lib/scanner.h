
#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "token.h"
#include "automate.h"
//#include "error.h"

FILE* file;
char current_char;
int ligne_number;
int column_number;

typedef struct _token {
    char         value[255];
    code_token_t code;
} token_t;

token_t current_token;
token_t last_token;

token_t clone_token(token_t);

bool isSpecial(char);
bool isEmpty(char);
bool isComment(char);

void append(char*, char);
void read_char(FILE*);
void read_word();
void read_number();
void read_special_symbol();
void read_comment();
void read_quoted();
void isOthers();
void scanning();
void get_lex();
void setCurrent_token_NULL();
void print_current_token();
void print_value_token(token_t);


#endif /* SCANNER_H_ */
