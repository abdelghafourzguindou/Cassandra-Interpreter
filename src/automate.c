#include "automate.h"

void inisializing(int stat_number)
{
    int i,j;
    transation_table    = (int**) malloc(stat_number*sizeof(int*));
    for(i = 0; i < stat_number; i++) transation_table[i] = (int*)  malloc(MAX_CARACTER*sizeof(int));
    for(i = 0; i < stat_number; i++) for(j = 0;  j <  MAX_CARACTER; j++) transation_table[i][j] = -1; //initialisation
}

bool is_table_function_name(char* str)
{
    char* nvStr = clone(str);
    char* pch;
    pch = strtok(nvStr,".");
    if(is_identifier(pch))
    {
        pch = strtok (NULL, ".");
        if(is_identifier(pch))
        {
            if(!strtok(NULL, ".")) return true;
        }
    }
    return false;
}

bool is_identifier(char* str)
{
    if(strlen(str) > 2 && (str[0] == '"' && str[strlen(str)-1] == '"')) return true;

    inisializing(2);
    int i;
    for(i = '0'; i <= '9'; i++) transation_table[1][i] = 1;
    for(i = 'a'; i <= 'z'; i++)
    {
        transation_table[0][i] = 1;
        transation_table[1][i] = 1;
    }
    for(i = 'A'; i <= 'Z'; i++)
    {
        transation_table[0][i] = 1;
        transation_table[1][i] = 1;
    }
    transation_table[1]['_'] = 1;

    i = 0;
    int e = 0;
    while(e != -1 && i < strlen(str))
    {
        e = transation_table[e][(int)(str[i])];
        i++;
    }
    if(e == -1) return false;
    return true;
}

bool is_string(char* str)
{
    if((str[0] == '\'' && str[strlen(str)-1] == '\'' && strlen(str) > 2)
           || (str[0] == '$' && str[1] == '$' && str[strlen(str)-2] == '$'
           && str[strlen(str)-1] == '$' && strlen(str) > 4))
        return true;
    return false;
}

bool is_integer(char* str)
{
    inisializing(2);
    int i;
    for(i = 48; i <= 57; i++)
    {
        transation_table[0][i] = 1;
        transation_table[1][i] = 1;
    } //pour 0...9
    transation_table[0][(int)'-'] = 0; //pour -

    i = 0;
    int e = 0;
    while(e != -1 && i < strlen(str))
    {
        e = transation_table[e][(int)(str[i])];
        i++;
    }
    if(e == -1 || strlen(str) == 0) return false;
    return true;
}

bool is_float(char* str)
{
    inisializing(6);
    int i;

    for(i = 48; i <= 57 ; i++)
    {
        transation_table[0][i] = 1;
        transation_table[1][i] = 1;
        transation_table[2][i] = 2;
        transation_table[3][i] = 5;
        transation_table[4][i] = 5;
        transation_table[5][i] = 5;
    }
    transation_table[0][(int)'-'] = 0;
    transation_table[1][(int)'.'] = 2;
    transation_table[2][(int)'e'] = 3;
    transation_table[2][(int)'E'] = 3;
    transation_table[3][(int)'+'] = 4;
    transation_table[3][(int)'-'] = 4;

    i = 0;
    int e = 0;
    while(e != -1 && i < strlen(str))
    {
        e = transation_table[e][(int)(str[i])];
        i++;
    }
    if(e == 1 || e == 2 || e == 5) return true;
    return false;
}

bool is_uuid(char* str)
{
    if(strlen(str) != 36) return false;
    int i = 0;
    bool is_valide = true;
    while(is_valide && i < 36)
    {
        is_valide = is_hex(str[i]);
        i++;
        if(i == 8 || i == 13 || i == 18 || i == 23)
        {
            if(str[i] == '-') i++;
            else return false;
        }
    }
    if(is_valide && i == 36) return true;
    return false;
}

bool is_boolean(char* str)
{
    if(!strcmp(str, "FALSE") || !strcmp(str, "TRUE")) return true;
    return false;
}

bool is_hex(char str)
{
    int z = (int)str;
    if((z >= '0' && z <= '9') || (z >= 'A' && z <= 'F') || (z >= 'a' && z <= 'f')) return true;
    return false;
}

bool is_blob(char* str)
{
    int i;
    if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        i = 2;
        bool is_valide = true;
        while(is_valide && i < strlen(str))
        {
            is_valide = is_hex(str[i]);
            i++;
        }
        if(i == strlen(str) && is_valide) return true;
    }
    return false;
}

char* clone(char* str) {

    char * nvStr = (char*)malloc(strlen(str));
    int i = 0 ;
    while( i < strlen(str))
    {   nvStr[i] = str[i];
        i++;
    }
    return nvStr;
}
