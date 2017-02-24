#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "error.h"

#define SCANNING   0
#define PARSING    1

bool no_error;

void toMajuscule(char* nom)
{
    int i = 0;
    while(nom[i])
    {
        nom[i] = toupper(nom[i]);
        i++;
    }
}

void toMiniscule(char* nom)
{
    int i = 0;
    while(nom[i]) {
        nom[i] = tolower(nom[i]);
        i++;
    }
}

void printAllPossibily()
{
    char *srt[100] = {"select", "insert","alter","delete","drop","update","LDD","LMD"};
    int i;
    for ( i=0 ; i < 8 ; i++)
        printf("%s -- ",srt[i]);
    printf("\n");
}

void makeCassandra() 
{
    char chemin     [40];
    char chemin_in  [40];
    char origine    [100];
    char requeste   [500];
    char commande   [100];
    int lenght;

    system("clear");

    while(true) {
        
        no_error = true;
        
        memset(chemin,   '\0', 40);
        memset(chemin_in,'\0', 40);
        memset(requeste, '\0', 100);
        memset(requeste, '\0', 500);
        memset(commande, '\0', 100);
        
        printf("%sCassandra > %s", RED, NC);
        gets(requeste);
        toMajuscule(requeste);

        //CLEAR
        if(!strcmp(requeste,"CLEAR"))
        {   
            system("clear");
        }       
        //HELP
        else if (strncmp(requeste,"HELP",4) == 0)
        {    

            if(strcmp(requeste+5,"?") == 0) printAllPossibily();
            else if(strlen(requeste) > 6)
            {
                strcpy(chemin,requeste+5);
                //printf("Chemin : %s\n",chemin );
                system("clear");
                sprintf(commande,"more help/%s.txt",chemin);
                system(commande);
            } 
            else 
            {    
                system("clear");
                system("more help/help.txt");
            }
        } 
        else if(strncmp(requeste,"EXECUTE",7) == 0)
        {
          strcpy(chemin_in,requeste+8);
          lenght = strlen(chemin_in) - strlen(strchr(requeste+9,')'));
          strncpy(chemin,chemin_in,lenght);
          toMiniscule(chemin);
          //puts(chemin);
          file = fopen(chemin, "r");
          if(file == NULL) puts("No such Path like this");
        }
        else
        { 
            file = fopen("test/requesteTest.sql", "w+");
            fprintf(file, "%s\n\n", requeste);
            fclose(file);
            file = fopen("test/requesteTest.sql", "r");
        }

        if(file != NULL) {        
            if(SCANNING) {
                read_char(file);
                while(current_token.code != END_TOKEN)
                {
                    get_lex();
                    print_current_token();
                    //getchar();
                }
            }
            if(PARSING) _parsing();
            fclose(file);
        }
    }
}

int main()
{

    //system("rm -r /home/zguindouos/Github/cassandra/CassandraDB/Keyspaces/*");
    //getchar();
    
    makeCassandra();
    
    /*file = fopen("test/test.sql", "r");
    if(file == NULL) {
        printf("error : file error");
    }
    else {        
        if(SCANNING) {
            read_char(file);
            while(current_token.code != END_TOKEN)
            {
                get_lex();
                print_current_token();
                //getchar();
            }
        }
        if(PARSING) _parsing();
        fclose(file);
    }*/

    return 0;
}




