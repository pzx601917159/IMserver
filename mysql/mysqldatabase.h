
#ifndef _MYSQLDB_ACCESS_H
#define _MYSQLDB_ACCESS_H
#include	<stdio.h>
#include	<string.h>

#include <time.h>
#include <map>
#include <list>
#include  <mysql/mysql.h>

#include "dbstruct.h"


#define DUP       2
#define DBUSER_PWD_LEN 200
#define DBFETCH_SQL_LEN 2000

class CDatabase
{
public:
    void ListRecord();                 	
    char* GetState();			
    char* GetServerInfo();		
    int GetProtocolInfo();			
    char* GetHostInfo();	
    char * GetClientInfo();			
    char* GetFieldName(int FieldNum);	
    bool IsEnd();				
    int DropDB(char *db);			
    void SeekData(int offset);	
    int CreateDB(char *db);			
    void FreeRecord();		
    unsigned int GetFieldNum();	
    bool ConnectDB(Database_param *p);	
    MYSQL_ROW GetRecord();			
    my_ulonglong GetRowNum();	
    bool SelectDB(Data_Param *para);
    bool UpdateRecord(Data_Param *para);
    bool SelectRecord(Data_Param *para);
    int InsertRecord(Data_Param *para);	

    bool DelRecord(Data_Param *para);	
    bool SelectAll(Data_Param *para);	
    bool SelectBySql(const char* sqlstr);
    char * OutErrors();				
    CDatabase();
    virtual ~CDatabase();

    bool Connect();
    bool ReConnect();
    void Close();

    bool ExecSql(char *sqlstr);
    bool InsertSql(char *sqlstr, my_ulonglong& insert_id); 
private:
    void ReadDbName();

public:
    Data_Param *para;   
    MYSQL *mysql;		
    MYSQL_RES *query;	
    MYSQL_ROW row;		
    MYSQL_FIELD *field;					
    Database_param *m_pdb_param;
    bool FindSave(const char *str);
    bool Succ;
    bool GetExeSqlState() {return Succ;};

    bool IsAvail();
public:

};
#endif

