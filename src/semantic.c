#include "semantic.h"

column_t            *Out               = NULL;
char                *COLUMN_CONDITION  = NULL;
char                *CONDITION         = NULL;
code_token_t        op;
bool                EXIST_CONDITION    = false;
bool                IS_COUNT           = false;
where_condition_t   *headConditions    = NULL;
token_t_list        *Head_liste_IN     = NULL;
user_t              *current_user      = NULL;
grant_t             *current_grant     = NULL;


// **************************************  FOR CREATE TABLE     ************************************** //
// Ici On doit chercher si la table existe selon KEYSPACE
// flag à éliminer c est juste pour tester si j appelle depuis create ou Insert   flag = 0 -> CREATE  flag = 1 -> INSERT

column_t* is_exist_table(char* nomTable , int flag)
{
    bool exist = true;
    FILE *f = fopen(nomTable, "r");
    if(f == NULL) exist = false;
    //if(fopen(nomTable, "r") == NULL) exist = false;
    fclose(f);
    if ( flag == 0)
    {
        //return  exist == false ? NULL : create_column(nomTable, STRING_TOKEN);
        return  exist == false ? NULL : create_column("exist", NULL_TOKEN);
    }
    else
    {
        // Ici Il faut retourner column_t* qui contient la structure de la table
        return  exist == false ? NULL : load_current_table_columns();// fonction de return
    }
}

bool is_exist_keyspace(const char* keyspaceName) {
    bool response = false;
    //mkdir(get_path(), 0777);
    //if(errno == EEXIST) {
    //printf("-->%s\n", get_current_keyspace_path());
    //FILE *f = fopen(keyspaceName, "r");
    if(fopen(keyspaceName, "r") != NULL) {
        set_semantic_state(false);
        response = true;
    }
    //fclose(f);
    return response;
}

// La 1ére chose lors de detection d une create table , C est creer la structure
table_t* create_table(const char * table_name)
{
    table_t* newTable = (table_t*) malloc(sizeof(table_t));
    newTable->column_list       = NULL;
    newTable->primary_key_list  = NULL;

    strcpy(newTable->table_name, table_name);
    return newTable;
}

// Lors du passage de syntaxique , On doi creer la colonne , s'il n est pas déja déclaré
column_t* create_column(const char *column_name, code_token_t column_type)
{
    column_t* newColumn = (column_t*) malloc(sizeof(column_t));
    strcpy(newColumn->column_name, column_name);
    newColumn->column_type = column_type;
    newColumn->next = NULL;

    return newColumn;
}

column_t* create_column_token(token_t token)
{
    column_t* newColumn = (column_t*) malloc(sizeof(column_t));
    strcpy(newColumn->column_name, token.value);
    newColumn->column_type = token.code;
    newColumn->next = NULL;

    return newColumn;
}

// Ajouter la colonne avec les autres qui seront affecter a la table courante
column_t* add_column(column_t* element , column_t* head)
{
    //print_column(element);
    //getchar();
    column_t* p  = head;
    if(p == NULL) head = element;
    else {
        while(p->next != NULL) p = p->next;
        p->next = element;
    }
    return head;
}

// A L'adition de chaque colonne , il faut verifiez s il est déclarée si oui il faut ajouter son type
bool search_column_CREATE(const char* name, column_t* head)
{
    if(head != NULL)
    {
        column_t* cursor = head;

        while(cursor != NULL) {
            if( strcmp(cursor->column_name, name) == 0 )
            {
                return true;
            }
            cursor = cursor->next;
        }
    }
    return false;
}

// l affichage de la table avec leurs colonnes
void print_table(table_t* tab)
{
    printf("----------------------\ntab name : %s\n----------------------\n", tab->table_name);
    print_column(tab->column_list);
    printf("\n");

}

// Affichage de la liste des colonnes d'une tables
void print_column(column_t * col) {
    if(col == NULL) printf("NULL COL");
    column_t* cursor = col;
    while(cursor != NULL) {
        printf("%s\t\t-- %s\n", cursor->column_name, token_code[cursor->column_type]);
        cursor = cursor->next;
    }
}


// **************************************  FOR INSERT INTO     ************************************** //


// A L'adition de chaque colonne , il faut verifiez s il est déclarée si oui il faut ajouter son type
code_token_t search_column_INSERT(const char* name , column_t* head)
{
    if(head != NULL)
    {
        column_t* cursor = head;

        while(cursor != NULL) {
            if( strcmp(cursor->column_name, name) == 0 )
            {
                return cursor->column_type;
            }
            cursor = cursor->next;
        }
    }
    return END_TOKEN;
}


int get_size_column(column_t* head)
{
    int size = 0 ;
    column_t* p = head ;
    while(p!= NULL)
    {
        size++;
        p = p->next;
    }
    return size;
}

int get_size_values(token_t_list* head)
{
    int size = 0 ;
    token_t_list* p = head ;
    while(p!= NULL)
    {
        size++;
        p = p->next;
    }
    return size;
}



void print_values(token_t_list* head)
{
    token_t_list* p = head ;
    if ( p == NULL )
        printf("\n La liste est vide !\n");
    while(p!= NULL)
    {
        printf("\n%s\t\t\t", p->token_v.value);
        print_token(p->token_v.code);
        p = p->next;
    }
}

// INTO (age,nom,prenom) VALUES ( .... )      ça permet de relever le type d un indice , si l'indice dépasse le nombre des colonne , la fonction return -1
token_t  extract_type_token(column_t* head, int indice)
{

   int i=1 ;
   column_t* p = head;
   token_t t;
   while( p!=NULL && i<indice)
   {
       i++;
       p = p->next;
   }

   if(p == NULL)
   {
       t.code = NULL_TOKEN ;
       return t;
   }
   else
   {
      t.code = p->column_type;
      strcpy(t.value,p->column_name);
      return t;
  }


}
// To add to the list of values
token_t_list* add_value(token_t_list* element ,token_t_list* head)
{
    token_t_list* p  = head;
    if(p == NULL) head = element;
    else {
        while(p->next != NULL) p = p->next;
        p->next = element;
    }
    return head;
}

token_t_list* create_value(token_t token)
{
  token_t_list* newvalue = (token_t_list*) malloc(sizeof(token_t_list));
  newvalue->token_v.code = token.code ;
  strcpy(newvalue->token_v.value , token.value);
  newvalue->next = NULL;

  return newvalue;
}

/// A la fin de construire les colonnes et les valeurs il faut les ajouter à l instruction
INST_INSERT* create_insert_inst(column_t* col_table, column_t*  current_col, token_t* lis_values)
{
    INST_INSERT* insert = (INST_INSERT*) malloc(sizeof(INST_INSERT));
    insert->Column_Of_Table = col_table;
    insert->Column_Actuel = current_col;
    insert->liste_Values = lis_values;

    return insert;

}

/// Interpretation :   return -1 si on va pas trouver le nom dans la liste des colonnes
int search_column_indice(const char* name, column_t* head)
{
    int i = 1 ;
    if(head != NULL)
    {
        column_t* cursor = head;

        while(cursor != NULL) {
            if( strcmp(cursor->column_name, name) == 0 )
            {
                return i;
            }
            i++;
            cursor = cursor->next;
        }
    }
    return -1;
}

////// Interpretation : return la value à partir d'un indice de liste values INTO(5,.....)
char* extract_from_values(token_t_list* head , int indice)
{
    int i=1 ;
    token_t_list* p = head;
    while(i<indice)
    {
        p = p->next;
        i++;
    }
    return p->token_v.value;
}


//// ------------------------------------------------------COntrole Semantique pour le where ------------------------------------------------------ ////

where_condition_t* create_condition(token_t columnConditon , char* conditionVal , code_token_t op)
{
     //printf("%d --%s --- %s -- %d\n", columnConditon.code , columnConditon.value, conditionVal , op );

    where_condition_t* newCondition = (where_condition_t*) malloc(sizeof(where_condition_t));
    newCondition->column_condition = clone_token(columnConditon);
    strcpy(newCondition->condition, conditionVal);
    newCondition->op = op;
    newCondition->next = NULL;
    return newCondition;
}

// Ajouter la where_condition_t avec les autres qui seront les conditions
where_condition_t* add_condition(where_condition_t* element , where_condition_t* head)
{
 
    where_condition_t* p  = head;
    if(p == NULL) head = element;
    else {
        while(p->next != NULL) p = p->next;
        p->next = element;
    }
    return head;
    

}

// Affichage de la liste des colonnes d'une tables
void print_Condition(where_condition_t * col) {
    if(col == NULL) printf("NULL Condition");
    where_condition_t* cursor = col;
    while(cursor != NULL) {
        printf("%s\t\t-- TYPE :", cursor->column_condition.value);
        print_token(cursor->column_condition.code);
        print_token(cursor->op);
        printf(" %s\n",cursor->condition);
        cursor = cursor->next;
    }
}


void set_current_user(user_t user) {
    current_user = (user_t*) malloc(sizeof(user_t));
    strcpy(current_user->user_name,     user.user_name);
    strcpy(current_user->user_password, user.user_password);
    current_user->user_option = user.user_option;
}


void reset_parameter()
{
    Head_Column_Of_Table    = NULL;
    Head_Column_Actuel      = NULL;
    Head_liste_Values       = NULL;
    Instruction_Insert      = NULL;
    Out                     = NULL;
    headConditions          = NULL;
    ALTER_TABLE_ORDER       = NULL_TOKEN;
    op                      = NULL_TOKEN;
    free(CONDITION);
    free(COLUMN_CONDITION);
    IS_COUNT                = false;
    EXIST_CONDITION         = false;
    updating_data           = NULL;
    current_user            = NULL;
    current_grant           = NULL;


    set_current_table_path(NULL);
    //set_current_keyspace_path(NULL);
}
