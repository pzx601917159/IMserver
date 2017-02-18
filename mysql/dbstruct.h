#ifndef __DB_STRUCT_H__
#define __DB_STRUCT_H__
#define MAX_STRING_LEN 1024
#include <string.h>
struct Data_Param   
{
    char db_name[MAX_STRING_LEN];
    char tab_name[MAX_STRING_LEN];
    char col_name[MAX_STRING_LEN];
    char select_exp[MAX_STRING_LEN];
    char where_def[MAX_STRING_LEN];
    char insert_val[MAX_STRING_LEN];
    char set_exp[MAX_STRING_LEN];
public:
    Data_Param()
    {
        memset(this, 0, sizeof(Data_Param));
    }
};

struct Database_param
{
    char host[MAX_STRING_LEN];
    char user[MAX_STRING_LEN];
    char password[MAX_STRING_LEN];
    char db[MAX_STRING_LEN];
    unsigned int port;
    char unix_socket[MAX_STRING_LEN];
    unsigned int client_flag;
public:
    Database_param()
    {
        memset(this, 0, sizeof(Database_param));
    }
};
#endif // __DB_STRUCT_H__
