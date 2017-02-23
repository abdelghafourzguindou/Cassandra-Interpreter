#ifndef DCL_PARSER_H
#define DCL_PARSER_H

#include "parser_tools.h"

//grant_statement
void _grant_statement();
void _grant_role_statement();
void _grant_permission_statement();

//revoke_statement
void _revoke_statement();
void _revoke_role_statement();
void _revoke_permission_statement();

//list_statement
void _list_statement();
void _list_roles_statement();
void _list_permissions_statement();
void _list_users_statement();




#endif // DCL_PARSER_H
