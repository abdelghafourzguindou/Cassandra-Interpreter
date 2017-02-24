#include "interpreter.h"

static bool         keyspace_is_used = false;
static const char*  directory = "CassandraDB";

column_t          *Out;
char              *COLUMN_CONDITION;
char              *CONDITION;
bool              EXIST_CONDITION;
bool              IS_COUNT;
data_insert_t     *updating_data;
where_condition_t *headConditions;
code_token_t      op;
token_t_list      *Head_liste_IN;
user_t            *current_user; 
grant_t           *current_grant;


void set_current_keyspace_path(const char* r) {
  //current_keyspace_path = (char*) malloc((strlen(directory) + strlen(r))*sizeof(char));
  //if(!strlen(r)) current_keyspace_path[0] = '\0';
  //else {
  strcat(current_keyspace_path, directory);
  strcat(current_keyspace_path, "/Keyspaces");
  strcat(current_keyspace_path, "/");
  strcat(current_keyspace_path, r);
  //}
}

char *get_current_keyspace_path() {
  return current_keyspace_path;
}

void set_current_table_path(const char *tab_name) {
  //current_table_path = (char*) malloc ((strlen(current_keyspace_path) + strlen(tab_name)) * sizeof(char));
  if(tab_name == NULL) current_table_path[0] = '\0';
  else {
    strcat(current_table_path, current_keyspace_path);
    strcat(current_table_path, "/");
    strcat(current_table_path, tab_name);
  }
}

char *get_current_table_path() {
  return current_table_path;
}

column_t* load_current_table_columns() {
  column_t *col_list = NULL;
  //FILE *table_file = fopen(get_current_table_path(), "r");
  //char* data = read_json_file(table_file);//"{\"description\": {\"EMPID\": 72,\"DEPTID\": 72,\"FIRST_NAME\": 80,\"LAST_NAME\": 80},\"data\": {}}";

  json_error_t error;
  json_t *root;
  json_t *element;

  int value;
  const char *key;

  //root = json_loads(data, 0, &error);
  //printf("ici -> %s\n", get_current_table_path());
  root = json_load_file(get_current_table_path(), 0, &error);
  //root = json_loadf(table_file,  0, &error);
  //root = json_loads(read_json_file(table_file), 0, &error);

  /*if(!root) {
    fprintf(stderr, "error : root\n");
    fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
    //exit(1);
  }*/

  json_t *desc = json_object_get(root, "description");
  /*if(!json_is_object(desc)) {
    printf("error loading description");
  }*/

  json_t *columns = json_object_get(desc, "columns");
  /*if(!json_is_object(columns)) {
    printf("error loading columns");
  }*/

  json_object_foreach(columns, key, element) {
    value = json_integer_value(element);
    column_t *newCol = create_column(key, value);
    //print_column(newCol);
    col_list = add_column(newCol, col_list);
    //print_column(col_list);
    //printf("%s : %d\n", key, value);
  }
  //print_column(col_list);
  //fclose(table_file);
  json_decref(columns);
  json_decref(desc);
  json_decref(root);
  return col_list;
}

column_t* load_current_table_primary_keys() {
  column_t *col_list = NULL;

  json_error_t error;
  json_t *root;
  json_t *element;
  const char *value;
  size_t index;

  root = json_load_file(get_current_table_path(), 0, &error);

  if(!root) {
    fprintf(stderr, "error : root\n");
    fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
    //exit(1);
  }

  json_t *desc = json_object_get(root, "description");
  if(!json_is_object(desc)) {
    printf("error loading description");
  }

  json_t *primaryKeys = json_object_get(desc, "primaryKeys");
  if(!json_is_array(primaryKeys)) {
    printf("error loading primary keys\n");
  }

  json_array_foreach(primaryKeys, index, element) {
    value = json_string_value(element);
    col_list = add_column(create_column(value, NULL_TOKEN), col_list);
  }

  json_decref(primaryKeys);
  json_decref(desc);
  json_decref(root);
  return col_list;
}

data_insert_t*  create_data_insert(char * key, char * value) {
  data_insert_t* newData = (data_insert_t*) malloc(sizeof(data_insert_t));
  strcpy(newData->key, key);
  strcpy(newData->value, value);
  newData->next = NULL;

  return newData;
}

data_insert_t* add_data_insert(data_insert_t* element , data_insert_t* head) {

  data_insert_t* p  = head;
  if(p == NULL) head = element;
  else {
    while(p->next != NULL) p = p->next;
    p->next = element;
  }
  return head;
}

char *read_json_file(FILE* fp) {
  char string[100];
  char *p = (char*) malloc(sizeof(char));
  while(fgets(string, 100, fp)) {
    p = (char*) realloc(p, (strlen(p) + strlen(string)) * sizeof(char));
    strcat(p, string);
  }
  return p;
}

// Cette fonction , on la donne la structure de la table et la colonne actuelle
data_insert_t* create_all_data_insert() {
  data_insert_t* inserting_data = NULL;
  int indice;

  if(Head_Column_Of_Table != NULL) {
    column_t* cursor = Head_Column_Of_Table;

    while(cursor != NULL) {
      indice = search_column_indice(cursor->column_name , Head_Column_Actuel);
      if( indice < 0) // n existe pas , initialiser avec NULL
      {
        inserting_data = add_data_insert(create_data_insert(cursor->column_name,"NULL") , inserting_data);
      }
      else
      {
        inserting_data = add_data_insert(create_data_insert(cursor->column_name, /* On cherche la valeur*/ extract_from_values(Head_liste_Values,indice)) , inserting_data);
      }
      cursor = cursor->next;
    }
  }
  return inserting_data;
}

void interpret()
{
  switch(CURRENT_PARENT_INST)
  {
    case USE_TOKEN        :  interpret_use();        break;
    case CREATE_TOKEN     :  interpret_create();     break;
    case ALTER_TOKEN      :  interpret_alter();      break;
    case DROP_TOKEN       :  interpret_drop();       break;
    case INSERT_TOKEN     :  interpret_insert();     break;
    case UPDATE_TOKEN     :  interpret_update();     break;
    case DELETE_TOKEN     :  interpret_delete();     break;
    case SELECT_TOKEN     :  interpret_select();     break;
    case TRUNCATE_TOKEN   :  interpret_truncute();   break;
    case GRANT_TOKEN      :  interpret_grant();      break;
    default : /*printf("interpret\n");*/ break;
  }
}

void interpret_create()
{
  switch(CURRENT_CHILD_INST)
  {
    case KEYSPACE_TOKEN     :     interpret_create_keyspace();    break;
    case TABLE_TOKEN        :     interpret_create_table();       break;
    case USER_TOKEN         :     interpret_create_user();        break;
    default : printf("interpret_create\n"); break;
  }
}

void interpret_create_keyspace() {

  set_current_keyspace_path(current_keyspace_name);

  if(!is_exist_keyspace(get_current_keyspace_path()))
  {
    mkdir(get_current_keyspace_path(), 0777);
    printf("Keyspace %s is created\n", current_keyspace_name);
  }
  else print_error(KEYSPACE_IS_ALREADY_EXISTS, current_keyspace_name);

  //free(current_keyspace_path);
  current_keyspace_path[0] = '\0';
}

void interpret_use() {
  //free(current_keyspace_path);
  current_keyspace_path[0] = '\0';
  set_current_keyspace_path(current_keyspace_name);
  if(is_exist_keyspace(get_current_keyspace_path()))
  {
    keyspace_is_used = true;
    printf("Keyspace %s is used\n", current_keyspace_name);
  }
  else {
    print_error(KEYSPACE_IS_NOT_EXISTS, current_keyspace_name);
    //free(current_keyspace_path);
    current_keyspace_path[0] = '\0';
  }
}

void interpret_create_table() {

  if(keyspace_is_used) {

      //printf("---> %s\n", get_current_table_path());

    if(!is_exist_table(get_current_table_path(), 0)) {

      json_t *root        = json_object();
      json_t *desc        = json_object();
      json_t *columns     = json_object();
      json_t *primaryKeys = json_array();

        //json_object_set_new(root, current_table->table_name, "table");
        json_object_set_new(root, "description", desc);         //head of table file
        json_object_set_new(desc, "columns", columns);          //head of table columns
        json_object_set_new(desc, "primaryKeys", primaryKeys);  //head of primary key table
        json_object_set_new(root, "data", json_object());       //head of data table

        do {
          json_t *newCol = json_object();
          json_object_set_new(newCol, current_table->column_list->column_name, json_integer(current_table->column_list->column_type));
          json_object_update(columns, newCol);
          printf("column %s is created\n", current_table->column_list->column_name);
          current_table->column_list = current_table->column_list->next;
        } while(current_table->column_list != NULL);

        //for storing primary key in a table
        while(current_table->primary_key_list != NULL) {
          json_array_append(primaryKeys, json_string(current_table->primary_key_list->column_name));
          current_table->primary_key_list = current_table->primary_key_list->next;
      } //while(current_table->primary_key_list != NULL);

      printf("table %s is created\n", current_table->table_name);

      FILE* new_table_file = fopen(get_current_table_path(), "w+");
      json_dumpf(root, new_table_file, JSON_INDENT(3));

      fclose(new_table_file);
      current_table_path[0] = '\0';
        //free(current_table_path);
      free(current_table->column_list);
      free(current_table);

      json_decref(columns);
      json_decref(primaryKeys);
      json_decref(desc);
      json_decref(root);
    }
  }
  else {
    print_error(UNUSED_KEYSPACE, "");
  }
}

void interpret_insert() {
  if(keyspace_is_used) {

    json_error_t error;
    json_t *root;
    json_t *data;
    json_t *element;

    const char *key;

    root = json_load_file(get_current_table_path(), 0, &error);
    data = json_object_get(root, "data");

    data_insert_t *inserting_data = create_all_data_insert();
      /*
      **controle semontique par primary key pour insert
      **---------------------------------------------------------------------------------------
      */
    column_t *current_table_primary_keys = load_current_table_primary_keys();
    bool col_already_exist = false;

    int primary_key_number  = get_size_column(current_table_primary_keys);
    int primary_key_counter = 0;

    data_insert_t *test_row = NULL;
    while(current_table_primary_keys) {
      data_insert_t *verify_insert = inserting_data;
      bool isPrimaryKey = false;
      while(verify_insert && !isPrimaryKey) {
        if(!strcmp(verify_insert->key, current_table_primary_keys->column_name)) {
          isPrimaryKey = true;
          test_row = add_data_insert(create_data_insert(verify_insert->key, verify_insert->value), test_row);
        }
        verify_insert = verify_insert->next;
      }
      current_table_primary_keys = current_table_primary_keys->next;
    }
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_t *e;
      const char *k;
      const char *v;
      data_insert_t *row = test_row;
      json_object_foreach(data_element, k , e) {
        if(row) {
          if(!strcmp(k, row->key)) {
            v = json_string_value(e);
            if(!strcmp(v, row->value)) primary_key_counter++;
            row = row->next;
          }
        }
      }
      if(primary_key_counter == primary_key_number) {
        col_already_exist = true;
        break;
      }
      else primary_key_counter = 0;
    }
      //---------------------------------------------------------------------------------------
      //si la colone n'exist pas
    if(!col_already_exist) {

      json_t *newRow = json_object();
      int row_numbers = (int) json_object_size(data);
      char row_key[15] = "Row";
      char c[12];
      sprintf(c, "%d", row_numbers);
      strcat(row_key, c);
      json_object_set_new(data, row_key, newRow);

      do {
        json_t *newCol = json_object();
        json_object_set_new(newCol, inserting_data->key, json_string(inserting_data->value));
        json_object_update(newRow, newCol);
        inserting_data = inserting_data->next;
      } while(inserting_data != NULL);

      FILE* table_file = fopen(get_current_table_path(), "w+");
      json_dumpf(root, table_file, JSON_INDENT(3));

      fclose(table_file);
      printf("row is inserted\n");
    }
      //sinon
    else {
      print_error(ROW_IS_ALREADY_EXISTS, "");
    }

    json_decref(data);
    json_decref(root);
    free(inserting_data);
  }
  else {
    print_error(UNUSED_KEYSPACE, "");
  }
  current_table_path[0] = '\0';
}

void interpret_alter() {
  switch(CURRENT_CHILD_INST) {
    case TABLE_TOKEN : interpret_alter_table(); break;
    case USER_TOKEN  : interpret_alter_user();  break;
    default : printf("interpret_alter\n"); break;
  }
}

void interpret_alter_table() {
  if(keyspace_is_used) {
    json_error_t error;
    json_t *root;
    json_t *desc;
    json_t *columns;
    json_t *element;
    json_t *data;
    json_t *e;
    const char *key;
    const char *k;

    root = json_load_file(get_current_table_path(), 0, &error);
    if(!root) {
      fprintf(stderr, "error : root\n");
      fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
    //exit(1);
    }

    desc = json_object_get(root, "description");
    if(!json_is_object(desc)) {
      printf("error loading description");
    }

    columns = json_object_get(desc, "columns");
    if(!json_is_object(columns)) {
      printf("error loading columns");
    }

    data = json_object_get(root, "data");
    if(!json_is_object(data)) {
      printf("error loading data");
    }

    switch(ALTER_TABLE_ORDER) {
      case ALTER_TOKEN :
      ;
      //Controle de si la colonne a alterer est exist dans data
      json_t *col;
      bool isPossible;
      json_object_foreach(data, key, element) {
        json_t *obj = json_object_get(data, key);
        col = json_object_get(obj, current_table->column_list->column_name);
        if(!strcmp(json_string_value(col), "NULL")) {
          isPossible = true;
        }
        else { 
          isPossible = false;
          break;
        }
      }
      //----------------------------
      if(!isPossible) print_error(ALTER_DENIED, current_table->column_list->column_name);
      else {
        json_t *newCol = json_object();
        json_object_set_new(newCol, current_table->column_list->column_name, json_integer(current_table->column_list->column_type));
        json_object_update_existing(columns, newCol);
        printf("column %s is altered\n", current_table->column_list->column_name);
        printf("table %s is altered\n", current_table->table_name);
      }
      break;
      case ADD_TOKEN :
      do {
        json_t *newCol = json_object();
        json_object_set_new(newCol, current_table->column_list->column_name, json_integer(current_table->column_list->column_type));
        json_object_update(columns, newCol);
        printf("new description %s\n", current_table->column_list->column_name);

        //ici pour ajouter les data de cette collone NULL
        json_object_foreach(data, key, element) {
          json_t *data_element  = json_object_get(data, key);
          json_t *new_data      = json_object();
          json_object_set_new(new_data, current_table->column_list->column_name, json_string("NULL"));
          json_object_update(data_element, new_data);
        }

        current_table->column_list = current_table->column_list->next;
      } while(current_table->column_list != NULL);
      printf("table %s is altered\n", current_table->table_name);
      break;
      case DROP_TOKEN :
      do {
        json_object_del(columns, current_table->column_list->column_name);
        printf("column %s is DROPED\n", current_table->column_list->column_name);

        //ici pour suprimer les data de cette colone
        json_object_foreach(data, key, element) {
          json_t *data_element = json_object_get(data, key);
          json_object_foreach(data_element, k , e) {
            if(!strcmp(k, current_table->column_list->column_name)) {
              json_object_del(data_element, k);
            }
          }
        }

        current_table->column_list = current_table->column_list->next;
      } while(current_table->column_list != NULL);
      printf("table %s is altered\n", current_table->table_name);
      break;
      default : printf("interpret_alter_table\n"); break;
    }

    FILE* table_file = fopen(get_current_table_path(), "w+");
    json_dumpf(root, table_file, JSON_INDENT(3));
    json_decref(columns);
    json_decref(desc);
    json_decref(root);
    fclose(table_file);
    current_table_path[0] = '\0';
  }
  else {
    print_error(UNUSED_KEYSPACE, "");
  }
}

void interpret_drop() {
  //si drop keyspace pas besoine de use keyspace
  if(CURRENT_CHILD_INST == KEYSPACE_TOKEN) {
    if(is_exist_keyspace(get_current_keyspace_path())) {
      //rmdir(get_current_keyspace_path());
      char this_keyspace[25] = "rm -r ";
      strcat(this_keyspace, get_current_keyspace_path());
      //printf("---- %s\n", this_keyspace);
      system(this_keyspace);
      printf("keyspace %s is droped\n", get_current_keyspace_path());
    } else printf("error : keyspace not exist\n");
  }
  //Drop user
  else if (CURRENT_CHILD_INST == USER_TOKEN)
  {
    interpret_drop_user();
  }
  //Drop table ou autre
  else if(keyspace_is_used) {
    switch(CURRENT_CHILD_INST) {
      case TABLE_TOKEN    : interpret_drop_table(); break;
      default : printf("interpret_drop"); break;
    }
  } else {
    print_error(UNUSED_KEYSPACE, "");
  }
}

void interpret_drop_table() {
  if(remove(get_current_table_path()) == -1) {
    perror("error : table can\'t droped");
  }
  else printf("\ntable %s is droped\n", get_current_table_path());
  current_table_path[0] = '\0';
}

void interpret_update() {
  if(keyspace_is_used) {

    json_error_t error;
    json_t *root;
    json_t *data;

    root = json_load_file(get_current_table_path(), 0, &error);
    if(!root) {
      fprintf(stderr, "error : root\n");
      fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
     //exit(1);
    }
    data = json_object_get(root, "data");
    if(!json_is_object(data)) {
      printf("error loading description");
    }

    json_t *element;
    json_t *e;
    json_t *ee;
    const char *key;
    const char *k;
    const char *v;
    const char *kk;

    json_t *selectedData = select_rows_where(data);

    json_object_foreach(data, key, element) {
      json_object_foreach(selectedData, k, e) {
        if(!strcmp(key, k)) {
          json_t *data_element = json_object_get(data, key);
          json_object_foreach(data_element, kk, ee) {
            data_insert_t *inData = updating_data; //pour conserver le updating_data
            while(inData) {
              json_t *newUpdateObj = json_object();
              json_object_set_new(newUpdateObj, inData->key, json_string(inData->value));
              json_object_update_existing(data_element, newUpdateObj);
              inData = inData->next;
            }
          }
        }
      }
    }

    FILE* table_file = fopen(get_current_table_path(), "w+");
    json_dumpf(root, table_file, JSON_INDENT(3));
    json_decref(root);
    fclose(table_file);
    printf("\ndata is updated\n");
  }

  else {
    print_error(UNUSED_KEYSPACE, "");
  }
  current_table_path[0] = '\0';
}

void interpret_truncute()
{
  if(keyspace_is_used) {

    json_error_t error;
    json_t *root;
    json_t *data;

    root = json_load_file(get_current_table_path(), 0, &error);
    if(!root) {
      fprintf(stderr, "error : root\n");
      fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
     //exit(1);
    }
    data = json_object_get(root, "data");
    if(!json_is_object(data)) {
      printf("error loading description");
    }

    json_t *element;
    json_t *e;
    const char *key;
    const char *k;
    const char *v;

    
    json_object_foreach(data, key, element) {
      json_object_del(data, key);
    }

    FILE* table_file = fopen(get_current_table_path(), "w+");
    json_dumpf(root, table_file, JSON_INDENT(3));
    json_decref(root);
    fclose(table_file);
    printf("\ndata is truncated\n");

  }else {
    print_error(UNUSED_KEYSPACE, "");
  }
  current_table_path[0] = '\0';
}


void interpret_delete() {
  if(keyspace_is_used) {

    json_error_t error;
    json_t *root;
    json_t *data;

    root = json_load_file(get_current_table_path(), 0, &error);
    if(!root) {
      fprintf(stderr, "error : root\n");
      fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
     //exit(1);
    }
    data = json_object_get(root, "data");
    if(!json_is_object(data)) {
      printf("error loading description");
    }

    json_t *element;
    json_t *e;
    const char *key;
    const char *k;
    const char *v;

    json_t *selectedData = select_rows_where(data);

    json_object_foreach(data, key, element) {
      json_object_foreach(selectedData, k, e) {
        if(!strcmp(k, key)) {
          json_object_del(data, key);
        }
      }
    }
    FILE* table_file = fopen(get_current_table_path(), "w+");
    json_dumpf(root, table_file, JSON_INDENT(3));
    json_decref(root);
    fclose(table_file);
    printf("\ndata is deleted\n");
  }

  else {
    print_error(UNUSED_KEYSPACE, "");
  }
  current_table_path[0] = '\0';
}

/*
** des fonctions pour select
*/

json_t *select_rows_where(json_t *data) {

  json_t *selectedData = json_object();

  json_t *element;
  json_t *e;
  json_t *ee;
  const char *key;
  const char *k;
  const char *v;
  const char *kk;
  const char *vv;

  switch(headConditions->op) {
    case EG_TOKEN     : 
        //Intrepretation pour where champ = condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(!strcmp(v, headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;

    case SUP_TOKEN    :
        //Intrepretation pour where champ > condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(atoi(v) > atoi(headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;

    case SUPEG_TOKEN  :
        //Intrepretation pour where champ >= condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(atoi(v) >= atoi(headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;

    case INF_TOKEN    :
        //Intrepretation pour where champ < condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(atoi(v) < atoi(headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;

    case INFEG_TOKEN  :
        //Intrepretation pour where champ <= condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(atoi(v) <= atoi(headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;

    case DIFF_TOKEN   :
        //Intrepretation pour where champ != condition
    json_object_foreach(data, key, element) {
      json_t *data_element = json_object_get(data, key);
      json_object_foreach(data_element, k , e) {
        if(!strcmp(k, headConditions->column_condition.value)) {
          v = json_string_value(e);
          if(atoi(v) != atoi(headConditions->condition)) {
            json_object_set_new(selectedData, key, data_element);
          }
        }
      }
    }
    break;
    default : break;
  }
  headConditions = headConditions->next;
  if(headConditions) return select_rows_where(selectedData);
  else return selectedData;
}

json_t *select_rows_in(json_t *data) {
  json_t *selectedData = json_object();

  json_t *element;
  json_t *e;
  json_t *ee;
  const char *key;
  const char *k;
  const char *v;
  const char *kk;
  const char *vv;

  json_object_foreach(data, key, element) {
    json_t *row = json_object_get(data, key);
    bool FOUND = false;
    token_t_list *inList = Head_liste_IN;
    json_object_foreach(row, k, e) {
      if(!strcmp(k, cloningToking.value)) {
        while(inList && !FOUND) {
          if(!strcmp(json_string_value(e), inList->token_v.value)) {
            json_object_set_new(selectedData, key, row);
            FOUND = true;
          }
          inList = inList->next;
        }
      }
    }
  }
  return selectedData;
}

void interpret_select() {
  if(keyspace_is_used) {
    json_error_t error;
    json_t *root;
    json_t *data;

    root = json_load_file(get_current_table_path(), 0, &error);
    if(!root) {
      fprintf(stderr, "error : root\n");
      fprintf(stderr, "error : on line %d: %s\n", error.line, error.text);
        //exit(1);
    }
    data = json_object_get(root, "data");
    if(!json_is_object(data)) {
      printf("error loading description");
    }

    json_t *element;
    json_t *e;
    json_t *ee;
    const char *key;
    const char *k;
    const char *v;
    const char *kk;
    const char *vv;
      /*
      **ICI POUR LE SELECT * FROM TABX WHERE CONDITION;
      **/
    if(!Out) {
        //pas de condition select * from tabx;
      if(!IS_COUNT) {
        column_t *header = load_current_table_columns();
        printf("\n-------------------------------------------------------------------------\n");
        while(header) {
          printf("%s\t", header->column_name);
          header = header->next;
        }
        if(!EXIST_CONDITION) {
          json_object_foreach(data, key, element) {
            printf("\n-------------------------------------------------------------------------\n");
            json_t *data_element = json_object_get(data, key);
            json_object_foreach(data_element, k , e) {
              v = json_string_value(e);
              printf("%s\t", v);
            }
          }
        }
        else {
            //ICI POUR select where col IN list;
          if(op == IN_TOKEN)
          { 
            json_t *selectedData = select_rows_in(data);
            json_object_foreach(selectedData, k , e) {
              printf("\n-------------------------------------------------------------------------\n");
              json_object_foreach(e, kk, ee) {
                vv = json_string_value(ee);
                printf("%s\t", vv);
              }
            }
          }else {
            json_t *selectedData = select_rows_where(data);
            json_object_foreach(selectedData, k , e) {
              printf("\n-------------------------------------------------------------------------\n");
              json_object_foreach(e, kk, ee) {
                vv = json_string_value(ee);
                printf("%s\t", vv);
              }
            } 
          }
        }
        printf("\n-------------------------------------------------------------------------\n");
      }
        //ICI POUR COUNT
      else {
        int size = 0;
        if(!EXIST_CONDITION) {
          size = (int) json_object_size(data);
          printf("%d\n", size);
        }
        else {
            //ICI POUR select where col IN list;
          if(op == IN_TOKEN)
          { 
            json_t *selectedData = select_rows_in(data);
            size = (int) json_object_size(selectedData);
            printf("%d\n", size);
          }else {
            json_t *selectedData = select_rows_where(data);
            size = (int) json_object_size(selectedData);
            printf("%d\n", size);
          }
        }
      }
    }
      /*
      **ICI POUR LE SELECT col1, col2... FROM TABX WHERE CONDITION;
      **/
    else {
        //Pas de condition where
      column_t *thisCol = Out;
      printf("\n-------------------------------------------------------------------------\n");
      while(thisCol) {
        printf("%s\t", thisCol->column_name);
        thisCol = thisCol->next;
      }
      if(!EXIST_CONDITION) {
        json_object_foreach(data, key, element) {
          json_t *data_element = json_object_get(data, key);
          column_t *Col = Out;
          printf("\n-------------------------------------------------------------------------\n");
          while(Col) {
            json_object_foreach(data_element, k , e) {
              if(Col && !strcmp(k, Col->column_name)) {
                v = json_string_value(e);
                printf("%s\t", v);
                Col = Col->next;
              }
            }
          }
        }
      }
        //avec condition where
      else {
          //ICI POUR select where col IN list;
        if(op == IN_TOKEN)
        { 
          json_t *selectedData = select_rows_in(data);
          json_object_foreach(selectedData, k , e) {
            column_t *Col = Out;
            printf("\n-------------------------------------------------------------------------\n");
            while(Col) {
              json_object_foreach(e, kk, ee) {
                if(Col && !strcmp(kk, Col->column_name)) {
                  vv = json_string_value(ee);
                  printf("%s\t", vv);
                  Col = Col->next;
                }
              }
            }
          }
        }else {
          json_t *selectedData = select_rows_where(data);
          json_object_foreach(selectedData, k , e) {
            column_t *Col = Out;
            printf("\n-------------------------------------------------------------------------\n");
            while(Col) {
              json_object_foreach(e, kk, ee) {
                if(Col && !strcmp(kk, Col->column_name)) {
                  vv = json_string_value(ee);
                  printf("%s\t", vv);
                  Col = Col->next;
                }
              }
            }
          }
        }
        printf("\n-------------------------------------------------------------------------\n");
      }
    }
    json_decref(root);
    printf("\nselect commande is finished\n");
  }
  else {
    print_error(UNUSED_KEYSPACE, "");
  }
  current_table_path[0] = '\0';
}

void interpret_create_user() {
  char u_Path[50] = {};
  strcat(u_Path, directory);
  strcat(u_Path, "/Users/Users.json");

  json_error_t error;
  json_t *r_User   = json_load_file(u_Path, 0, &error);

  json_t *e_User;
  json_t *i_User;
  const char *k_User;
  const char *ki_User;

  bool u_exist = false;

  json_object_foreach(r_User, k_User, e_User) {
    json_object_foreach(e_User, ki_User, i_User) {
      if(!strcmp(ki_User, "user_name")) {
        if(!strcmp(json_string_value(i_User), current_user->user_name)) u_exist = true;
      }
    }
  }

  if(!u_exist) {

    json_t *new_User    = json_object();
    json_t *on          = json_object();

    char n_User[15] = "User";
    char c[3];
    sprintf(c, "%d", (int)json_object_size(r_User));
    strcat(n_User, c);
    json_object_set_new(r_User, n_User, new_User);
    json_object_set_new(new_User,     "user_name",      json_string (current_user->user_name));
    json_object_set_new(new_User,     "user_password",  json_string (current_user->user_password));
    json_object_set_new(new_User,     "user_option",    json_integer(current_user->user_option));
    json_object_set_new(new_User,     "user_permission", on);
    json_object_set_new(on, "Keyspaces",  json_object());
    json_object_set_new(on, "Tables",     json_object());
    json_object_set_new(on, "Roles",      json_object());
    json_object_set_new(on, "Functions",  json_object());

    FILE* f_User = fopen(u_Path, "w+");
    json_dumpf(r_User, f_User, JSON_INDENT(3));
    fclose(f_User);
    json_decref(new_User);
    printf("user %s is created\n", current_user->user_name);
  }
  else {
    print_error(USER_ALREADY_EXISTS, current_user->user_name);
  }

  json_decref(r_User);
}

void interpret_alter_user() {
  char u_Path[50] = {};
  strcat(u_Path, directory);
  strcat(u_Path, "/Users/Users.json");

  json_error_t error;
  json_t *r_User   = json_load_file(u_Path, 0, &error);

  json_t *e_User;
  json_t *i_User;
  const char *k_User;
  const char *ki_User;

  bool u_exist = false;

  json_object_foreach(r_User, k_User, e_User) {
    json_object_foreach(e_User, ki_User, i_User) {
      if(!strcmp(ki_User, "user_name")) {
        if(!strcmp(json_string_value(i_User), current_user->user_name)) {
          u_exist = true;
          if(current_user->user_option != NULL_TOKEN) {
            json_t *u_niUser = json_object();
            json_object_set_new(u_niUser, "user_option", json_integer(current_user->user_option));
            json_object_update(e_User, u_niUser);
          }
          else {
            json_t *u_niUser = json_object();
            json_object_set_new(u_niUser, "user_password", json_string(current_user->user_password));
            json_object_update(e_User, u_niUser);
          }
        }
      }
    }
  }

  if(u_exist) {
    FILE* f_User = fopen(u_Path, "w+");
    json_dumpf(r_User, f_User, JSON_INDENT(3));
    fclose(f_User);
    printf("user %s is altered\n", current_user->user_name);
  }
  else {
    print_error(USER_IS_NOT_EXISTS, current_user->user_name);
  }
  json_decref(r_User);
}

void interpret_drop_user() {
  char u_Path[50] = {};
  strcat(u_Path, directory);
  strcat(u_Path, "/Users/Users.json");

  json_error_t error;
  json_t *r_User   = json_load_file(u_Path, 0, &error);

  json_t *e_User;
  json_t *i_User;
  const char *k_User;
  const char *ki_User;

  bool u_exist = false;

  json_object_foreach(r_User, k_User, e_User) {
    json_object_foreach(e_User, ki_User, i_User) {
      if(!strcmp(ki_User, "user_name")) {
        if(!strcmp(json_string_value(i_User), current_user->user_name)) {
          u_exist = true;
          json_object_del(r_User, k_User);
        }
      }
    }
  }

  if(u_exist) {
    FILE* f_User = fopen(u_Path, "w+");
    json_dumpf(r_User, f_User, JSON_INDENT(3));
    fclose(f_User);
    printf("user %s is droped\n", current_user->user_name);
  }
  else {
    print_error(USER_IS_NOT_EXISTS, current_user->user_name);
  }
  json_decref(r_User);
}

void interpret_grant() {
  /*printf("user  : %s\n", current_grant->user_name);
  printf("perm  : %d\n", current_grant->permission_name);
  printf("ress  : %d\n", current_grant->resource.r_key);
  printf("code  : %d\n", current_grant->resource.r_value.v_Code);
  printf("code  : %s\n", current_grant->resource.r_value.v_Others);*/

  char u_Path[50] = {};
  strcat(u_Path, directory);
  strcat(u_Path, "/Users/Users.json");

  json_error_t error;
  json_t *r_User   = json_load_file(u_Path, 0, &error);

  json_t *e_User;
  json_t *i_User;
  const char *k_User;
  const char *ki_User;

  bool u_exist = false;
  json_object_foreach(r_User, k_User, e_User) {
    json_object_foreach(e_User, ki_User, i_User) {
      if(!strcmp(ki_User, "user_name")) {
        if(!strcmp(json_string_value(i_User), current_grant->user_name)) {
          u_exist = true;
          json_t *permissions = json_object_get(e_User, "user_permission");
          json_t *desc        = json_object();
          json_t *on          = json_object();
          switch(current_grant->resource.r_key) {
            case ALL_TOKEN      : on = json_object_get(permissions, "Keyspaces");  break;
            case KEYSPACE_TOKEN : on = json_object_get(permissions, "Keyspaces");  break;
            case TABLE_TOKEN    : on = json_object_get(permissions, "Tables");     break;
            default : break;
          }
          char n_Perm[15] = "Permession";
          char c[3];
          sprintf(c, "%d", (int)json_object_size(on));
          strcat(n_Perm, c);
          json_object_set_new(on, n_Perm, desc);
          json_t *perm_name = json_object();
          json_object_set_new(perm_name, "permission_name", json_integer(current_grant->permission_name));
          json_object_update(desc, perm_name);
          json_t *perm_on = json_object();
          if(strlen(current_grant->resource.r_value.v_Others) < 1) 
            json_object_set_new(perm_on, "On", json_integer(current_grant->resource.r_value.v_Code));
          else
            json_object_set_new(perm_on, "On", json_string(current_grant->resource.r_value.v_Others));
            json_object_update(desc, perm_on);
        }
      }
    }
  }

  if(u_exist) {
    FILE* f_User = fopen(u_Path, "w+");
    json_dumpf(r_User, f_User, JSON_INDENT(3));
    fclose(f_User);
    printf("user %s is granted\n", current_grant->user_name);
  }
  else {
    print_error(USER_IS_NOT_EXISTS, current_grant->user_name);
  }
  json_decref(r_User);
}