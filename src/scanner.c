#include "scanner.h"

int ligne_number  = 1;
int column_number = 1;

void append(char* s, char c)
{
    int len  = strlen(s);
    s[len]   = c;
    s[len+1] = '\0';
}

void read_char(FILE* file)
{
    current_char = toupper(fgetc(file));
    column_number++;
    //printf("current --> %c\n", current_char);
}


//IDF.IDF "IDF"."IDF"  "IDF".IDF  IDF."IDF"

void read_quoted()
{
    append(current_token.value, current_char);
    read_char(file);
    while(current_char != '\"' && current_char != EOF) { append(current_token.value, current_char);  read_char(file); }
    if(current_char == EOF) current_token.code = ERROR_TOKEN;
    else
    {
        append(current_token.value, current_char);
        read_char(file);
        if(current_char == '.')
        {
            append(current_token.value, current_char);
            read_char(file);
            read_word();
        }
        if(is_table_function_name(current_token.value)) current_token.code = TABLE_FUNCTION_NAME_TOKEN ;
        else if(is_identifier(current_token.value))     current_token.code = IDENTIFIER_TOKEN;
        else                                            current_token.code = ERROR_TOKEN;
    }
}

void read_word()
{

    if(current_char == '\"') read_quoted();
    else if(current_char == '\'' || current_char == '$') read_string();
    else
    {
        while(isalnum(current_char) || current_char == '_' || current_char == '.')
        {
            append(current_token.value, current_char);
            read_char(file);
        }

        if(current_char == '\"') read_quoted();

        code_token_t unlex = is_Keyword(current_token.value);
        if(unlex != ERROR_TOKEN)                            current_token.code = unlex;
        else if(is_boolean(current_token.value))            current_token.code = BOOLEAN_VALUE_TOKEN;
        else if(is_identifier(current_token.value))         current_token.code = IDENTIFIER_TOKEN;
        else if(is_table_function_name(current_token.value))current_token.code = TABLE_FUNCTION_NAME_TOKEN;
        else                                                current_token.code = ERROR_TOKEN;
    }
}

void read_number()
{
    while(isalnum(current_char) || current_char == '.')
    {
        append(current_token.value, current_char);
        read_char(file);
    }

    if     (is_integer(current_token.value)) current_token.code = INT_VALUE_TOKEN;
    else if(is_float(current_token.value))   current_token.code = FLOAT_VALUE_TOKEN;
    else                                     current_token.code = ERROR_TOKEN;
}

void read_special_symbol()
{
    append(current_token.value, current_char);
    switch(current_char)
    {
    case '+': current_token.code = PLUS_TOKEN;      read_char(file);         break;
    case '-': current_token.code = MOINS_TOKEN;     read_char(file);         break;
    case '*': current_token.code = ETOILE_TOKEN;    read_char(file);         break;
    case ',': current_token.code = VIR_TOKEN;       read_char(file);         break;
    case ';': current_token.code = PV_TOKEN;        read_char(file);         break;
    case '/': current_token.code = DIV_TOKEN;       read_char(file);         break;
    case '(': current_token.code = PO_TOKEN;        read_char(file);         break;
    case ')': current_token.code = PF_TOKEN;        read_char(file);         break;
    case '=': current_token.code = EG_TOKEN;        read_char(file);         break;
    case '{': current_token.code = ACOLADO_TOKEN;   read_char(file);         break;
    case '}': current_token.code = ACOLADF_TOKEN;   read_char(file);         break;
    case ':': current_token.code = DEUXP_TOKEN;     read_char(file);         break;
    case '.': current_token.code = P_TOKEN;         read_char(file);         break;
    case '[': current_token.code = CROCHO_TOKEN;    read_char(file);         break;
    case ']': current_token.code = CROCHF_TOKEN;    read_char(file);         break;
    case '?': current_token.code = PI_TOKEN;        read_char(file);         break;
    case '<': read_char(file);
        switch(current_char)
        {
        case '=': append(current_token.value, current_char); current_token.code = INFEG_TOKEN; read_char(file); break;
        default : current_token.code = INF_TOKEN;   break;
        }
        break;
    case '>': read_char(file);
        switch(current_char){
        case '=': append(current_token.value, current_char); current_token.code = SUPEG_TOKEN; read_char(file); break;
        default : current_token.code = SUP_TOKEN;   break;
        }
        break;
    case '!': read_char(file);
        switch(current_char)
        {
        case '=': append(current_token.value, current_char); current_token.code = DIFF_TOKEN; read_char(file);  break;
        default : current_token.code = ERROR_TOKEN; break;
        }
        break;
    default : current_token.code = ERROR_TOKEN;     break;
    }
}

void read_comment()
{
    char s = current_char;
    read_char(file);

    if(s == '-')
    {
        if(current_char == '-') while(current_char != '\n') read_char(file);
        else
        {
            append(current_token.value, s);
            current_token.code = MOINS_TOKEN;
        }
    }

    if(s == '/')
    {

        if(current_char == '/') while(current_char != '\n') read_char(file);
        else if(current_char == '*')
        {
            int fin_comment = 0;
            char previous_char;

            previous_char = current_char;
            read_char(file);
            while( fin_comment == 0 && current_char != EOF)
            {
                previous_char = current_char ;
                read_char(file);
                if(previous_char == '*' && current_char == '/')
                {
                    fin_comment = 1;
                    read_char(file);
                }
            }
            if( fin_comment == 0) printf("error : incomplet comment");
        }
        else
        {
            append(current_token.value, s);
            current_token.code = DIV_TOKEN;
        }
    }
}

bool isComment(char s)
{
    switch(s)
    {
    case '-' : return true; break;
    case '/' : return true; break;
    default  : return false;break;
    }
}

bool isEmpty(char s)
{
    if(s == '\n') ligne_number++;
    if(s == ' ' || s == '\n' || s == '\t') return true;
    return false;
}

bool isSpecial(char s)
{
    if(s == ':' ||
            s == '=' ||
            s == '<' ||
            s == '>' ||
            s == '+' ||
            s == '-' ||
            s == '/' ||
            s == '*' ||
            s == ',' ||
            s == ';' ||
            s == '.' ||
            s == '(' ||
            s == ')' ||
            s == '{' ||
            s == '}' ||
            s == '[' ||
            s == ']' ||
            s == '?' ||
            s == '!') return true;
    return false;
}

void setCurrent_token_NULL()
{
    current_token.value[0] = '\0';
    current_token.code     = EMPTY_TOKEN;
}

void read_empty()
{
    do{ read_char(file); } while(isEmpty(current_char));
}

void read_string()
{
    if(current_char == '\'')
    {
        do {
            append(current_token.value, current_char);
            read_char(file);
        } while(current_char != '\'' && current_char != EOF);
        if(current_char == EOF) current_token.code = ERROR_TOKEN;
        else
        {
            append(current_token.value, current_char);
            current_token.code = STRING_TOKEN;
            read_char(file);
        }
    }
    if(current_char == '$')
    {
        append(current_token.value, current_char);
        read_char(file);
        if(current_char == '$')
        {
            do {
                append(current_token.value, current_char);
                read_char(file);
                if(current_char == '$')
                {
                    append(current_token.value, current_char);
                    read_char(file);
                }
            } while(current_char != '$' && current_char != EOF);
            if(current_char == EOF) current_token.code = ERROR_TOKEN;
            else
            {
                append(current_token.value, current_char);
                current_token.code = STRING_TOKEN;
                read_char(file);
            }
        }
        else
        {
            append(current_token.value, current_char);
            isOther();
            current_token.code = ERROR_TOKEN;
        }
    }
}

void print_current_token()
{
    printf("%s\t\t\t", current_token.value);
    print_token(current_token.code);
    printf("\n");
}

void print__value_token(token_t tocken)
{
    printf("%s\t\t\t", tocken.value);
    print_token(tocken.code);
    printf("\n");
}

token_t clone_token(token_t curent)
{
    token_t last ;
    strcpy(last.value,curent.value);
    last.code = curent.code;
    return last ;
}

void isOther()
{
    do {
        append(current_token.value, current_char);
        read_char(file);
    } while( !isSpecial(current_char) && !isEmpty(current_char) && current_char != EOF ) ;

}

void scanning()
{
    setCurrent_token_NULL();

    if      (isEmpty  (current_char))           read_empty();
    else if (isComment(current_char))           read_comment();
    else if (isalpha  (current_char) ||
             current_char == '\"'    ||
             current_char == '\''    ||
             current_char == '$')               read_word();
    else if (isdigit  (current_char))           read_number();
    else if (isSpecial(current_char))           read_special_symbol();
    else if (current_char == EOF)               current_token.code = END_TOKEN;
    else
    {
        isOther();
        current_token.code     = ERROR_TOKEN;
    }
}

void get_lex()
{
    last_token.value[0] = '\0';
    last_token.code    = EMPTY_TOKEN;
    strcpy(last_token.value, current_token.value);
    last_token.code = current_token.code;
    do {
        scanning();
    } while(current_token.code == EMPTY_TOKEN);
}
