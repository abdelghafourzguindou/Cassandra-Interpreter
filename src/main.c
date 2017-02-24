#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "error.h"

#define SCANNING   0
#define PARSING    1

bool no_error;

void push_insert_data() {
    char data[500];
    int id;
    float salaire = 500;
    char c_Data[10];
    FILE *f = fopen("test/insert.cql", "w+");
    for(id = 0; id < 200; id++) {
        strcat(data, "insert into prof values (");
        memset(c_Data, '\0', 10);
        sprintf(c_Data, "%d", id);
        strcat(data, c_Data);
        strcat(data, ", 'cin");
        strcat(data, c_Data);
        strcat(data, "'"); 
        strcat(data, ", 'nom");
        strcat(data, c_Data);
        strcat(data, "'");        
        strcat(data, ", 'prenom");
        strcat(data, c_Data);
        strcat(data, "'"); 
        memset(c_Data, '\0', 10);
        sprintf(c_Data, "%f", salaire);
        salaire += 100;
        strcat(data, ", ");
        strcat(data, c_Data);
        strcat(data, ");");
        fprintf(f, "%s\n", data);
        memset(data, '\0', 500);
    }
    if(f) fclose(f);
}

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
    printf("help ?              print help posibility\n");
    printf("help                print all cassandra help\n");
    printf("help select         print select help\n");
    printf("help insert         print insert help\n");
    printf("help alter          print alter help\n");
    printf("help delete         print delete help\n");
    printf("help create         print create help\n");
    printf("help drop           print drop help\n");
    printf("help update         print update help\n");
    printf("help LDD            print LDD help\n");
    printf("help LMD            print LMD help\n");
    printf("desc                print description\n");
    printf("desc table_name     print table description\n");
    printf("execute             execute a cql script\n");
    printf("clear               screen clean\n");
}
/*
char[500] r_Request() {
    char request    [500];
    char commande   [500];
    char c = getc();
    while(c != ';') {
        if((int)c != 13) {
            printf("dddddddddd\n");
        }
        else printf("eeeeeeeeeeeee\n");
    }

    return request;
}
*/
void makeCassandra() 
{
    char chemin     [40];
    char chemin_in  [40];
    char origine    [100];
    char commande   [100];
    char request    [500];
    char nameTable  [100];
    int lenght;

    system("clear");

    while(true) {
        
        no_error = true;
        
        memset(chemin,   '\0', 40);
        memset(chemin_in,'\0', 40);
        memset(origine,  '\0', 100);
        memset(commande, '\0', 100);
        memset(request,  '\0', 500);
        memset(nameTable,'\0', 100);
        
        printf("%sCassandra > %s", RED, NC);
        gets(request);
        //request = r_Request();
        toMajuscule(request);

        //CLEAR
        if(!strcmp(request,"CLEAR"))
        {   
            system("clear");
        }       
        //HELP
        else if (strncmp(request,"HELP",4) == 0)
        {    

            if(strcmp(request+5,"?") == 0) printAllPossibily();
            else if(strlen(request) > 6)
            {
                strcpy(chemin,request+5);
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
        // DESCRIBE
         else if(! strncmp(request,"DESC",4))
         {   
            if (strlen(request) == 4)
            {
                if(get_State_Current_Keyspace())
                {
                printf("Current keyspace table list :\n");
                sprintf(commande,"ls %s | tr -s \" \" ",get_current_keyspace_path());
                }
                else
                {
                printf("Keyspaces list :\n");
                sprintf(commande,"ls CassandraDB/Keyspaces | tr -s \" \" ");
                }
                system(commande);
            }
            else
            {
                strcpy(nameTable,request+5);
                print_column(load_table_columns(nameTable));

            }
        } 
        else if(strncmp(request,"EXECUTE",7) == 0)
        {
          strcpy(chemin_in,request+8);
          lenght = strlen(chemin_in) - strlen(strchr(request+9,')'));
          strncpy(chemin,chemin_in,lenght);
          toMiniscule(chemin);
          //puts(chemin);
          file = fopen(chemin, "r");
          if(file == NULL) puts("No such Path like this");
        }
        else
        { 
            file = fopen("test/request.cql", "w+");
            fprintf(file, "%s\n\n", request);
            fclose(file);
            file = fopen("test/request.cql", "r");
        }

        if(file) { 
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
            file = NULL;
        }
    }
}

int main()
{

    //system("rm -r /home/zguindouos/Github/cassandra/CassandraDB/Keyspaces/*");
    //getchar();
    
    //push_insert_data();
    makeCassandra();
    
    /*file = fopen("test/test.cql", "r");
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




