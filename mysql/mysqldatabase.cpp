
#include "dbstruct.h"
#include "mysqldatabase.h"
#include "readconfig.h"
CDatabase::CDatabase()
{
    mysql=(MYSQL *) new MYSQL;
    mysql_init(mysql);
    m_pdb_param = new Database_param;
    para = new Data_Param;
    Succ = true;
}
CDatabase::~CDatabase()
{
	mysql_close(mysql);
    delete mysql;
    mysql=NULL;

	delete m_pdb_param;
	m_pdb_param = NULL;
	delete para;
}
bool CDatabase::SelectAll(Data_Param *para)
{
	char str[MAX_STRING_LEN];
    memset(str, 0, MAX_STRING_LEN);
	sprintf(str,"select * from %s",para->tab_name);
	if(!FindSave(str))
	{
		return false;
	}
	return true;
}
bool CDatabase::SelectBySql(const char* sqlstr)
{
   if(!sqlstr){
        return false;
    }

	if(!FindSave(sqlstr))
	{
		return false;
	}
	return true;
}
bool CDatabase::DelRecord(Data_Param *para)
{
	char str[MAX_STRING_LEN];
	memset(str, 0, MAX_STRING_LEN);
	sprintf(str,"delete from %s where %s",para->tab_name,para->where_def);
	if(mysql_query(mysql,str))
	{
		return false;
	}
	return true;
}
int CDatabase::InsertRecord(Data_Param *para)
{
    char str[MAX_STRING_LEN];
    memset(str, 0, MAX_STRING_LEN);
    char error_str[MAX_STRING_LEN];
    memset(error_str, 0, MAX_STRING_LEN);
    sprintf(str,"INSERT LOW_PRIORITY INTO %s VALUES(%s)",para->tab_name,para->insert_val);

    if(mysql_query(mysql,str))
    {
        strcpy(error_str,OutErrors());
        if(strstr(error_str,"Duplicate")==NULL)
        {
            return FALSE;
        }
        else
        {
            return DUP;
        }
    }
    return TRUE;
}
bool CDatabase::SelectRecord(Data_Param *para)
{
	char str[MAX_STRING_LEN];
	memset(str, 0, MAX_STRING_LEN);
	sprintf(str,"select %s from %s where %s",para->select_exp,para->tab_name,para->where_def);

	if(!FindSave(str))
	{
		Succ = false;
		return false;
	}
	Succ = true;
	return true;
}
bool CDatabase::SelectDB(Data_Param *para)
{
	if (mysql_select_db(mysql,para->db_name))
	{
		return false;
	}
	else
	{
		return true;
	}
}
char * CDatabase::OutErrors()
{
	char *rec;
	rec=(char *)mysql_error(mysql);
 	return rec;
}
bool CDatabase::FindSave(const char *str)
{
    if(mysql_query(mysql,str))
	 {
		return false;
	 }
    query = mysql_store_result(mysql);

    return true;
}
bool CDatabase::UpdateRecord(Data_Param *para)
{
	char str[MAX_STRING_LEN];
	memset(str, 0, MAX_STRING_LEN);
	sprintf(str,"update %s set %s where %s",para->tab_name,para->set_exp,para->where_def);

	if(mysql_query(mysql,str))
	{
		return false;
	}

	return true;
}
my_ulonglong CDatabase::GetRowNum()
{
	my_ulonglong rec;
	if (query)
	{
		rec=mysql_num_rows(query);
		return rec;
	}
	return 0;
}
MYSQL_ROW CDatabase::GetRecord()
{
    return (row = mysql_fetch_row(query));
}
bool CDatabase::ConnectDB(Database_param *p)
{
	my_bool blConnect = true;
	mysql_options(mysql, MYSQL_OPT_RECONNECT,&blConnect);
	mysql_options(mysql, MYSQL_OPT_LOCAL_INFILE, 0);
	p->client_flag = CLIENT_LOCAL_FILES;
	if (!mysql_real_connect(mysql,p->host,p->user,p->password,p->db,p->port,p->unix_socket,p->client_flag))
	{
		OutErrors();
		return false;
	}
	m_pdb_param=p;
	mysql_set_character_set(mysql,"utf8");
	mysql_query(mysql,"SET NAMES 'UTF8'");

	return true;
}
unsigned int CDatabase::GetFieldNum()
{
	unsigned int rec;
	rec=mysql_num_fields(query);
	return rec;
}
void CDatabase::FreeRecord()
{
	mysql_free_result(query);
}
int CDatabase::CreateDB(char *db)
{
	return 0;
}
void CDatabase::SeekData(int offset)
{
	mysql_data_seek(query,offset);
}
int CDatabase::DropDB(char *db)
{
	return 0;
}
BOOL CDatabase::IsEnd()
{
	BOOL rec;
	rec=mysql_eof(query);

	return rec;
}
char* CDatabase::GetFieldName(int FieldNum)
{
	field=mysql_fetch_field_direct(query,FieldNum);

	return field->name;
}
char * CDatabase::GetClientInfo()
{
	char *rec;
	rec=(char *)mysql_get_client_info();

	return rec;
}
char* CDatabase::GetHostInfo()
{
	char *rec;
    rec=(char *)mysql_get_host_info(mysql);
	return rec;
}
int CDatabase::GetProtocolInfo()
{
	int rec;
	rec=mysql_get_proto_info(mysql);

	return rec;
}
char* CDatabase::GetServerInfo()
{
	char *rec;
	rec=(char *)mysql_get_server_info(mysql);

	return rec;
}
char* CDatabase::GetState()
{
	char* state=(char *)mysql_stat(mysql);
	if(!state)
	{
		return OutErrors();
	}

	return state;
}

void CDatabase::ListRecord()
{
    int i,x,l;
    char aszFlds[25][25];

    i=(int)GetRowNum();
    i=(int)GetFieldNum();
    for(x=0;x<i;x++)
	{
		strcpy(aszFlds[x],GetFieldName(x));
	}
    l=1;
    while (GetRecord())
	{
		i=GetFieldNum();
		for(x=0;x<i;x++)
			(((row[x]==NULL)||(!strlen(row[x])))?"NULL":row[x]) ;
		puts( "==============================\n" ) ;
	}
}
bool CDatabase::Connect()
{
	char inputPassward[DBUSER_PWD_LEN];
	char outputPassward[DBUSER_PWD_LEN];
	memset(inputPassward, 0, DBUSER_PWD_LEN);
	memset(outputPassward, 0, DBUSER_PWD_LEN);
	char err_sql[DBFETCH_SQL_LEN];
	GetProfileString("Config.ini","DATABASE","dbuser",m_pdb_param->user);
	GetProfileString("Config.ini","DATABASE","db",m_pdb_param->db);
    GetProfileString("Config.ini","DATABASE","dbpwd",m_pdb_param->password);
	GetProfileString("Config.ini","DATABASE","host",m_pdb_param->host);
	char port[10]={0};
	GetProfileString("Config.ini","DATABASE","dbport",port);
	m_pdb_param->port=atoi(port);

	if(ConnectDB(m_pdb_param))
	{
		STRNCPY(err_sql,"数据库连接成功！",DBFETCH_SQL_LEN);
        Data_Param dataParam;
		strcpy(dataParam.db_name,m_pdb_param->db);
		SelectDB(&dataParam);
		return true;
	}
	STRNCPY(err_sql,"数据库连接不成功！",DBFETCH_SQL_LEN);
	return false;
}
bool CDatabase::ReConnect()
{
	return !(mysql_ping(mysql));
}
void  CDatabase::Close()
{
    mysql_close(mysql);
}
bool CDatabase::ExecSql(char *sqlstr)
{
	if (strstr(sqlstr,"%In")==NULL && strstr(sqlstr,"% In")==NULL)
	{
	}
	if(mysql_query(mysql,sqlstr))
	{
		return false;
	}

	return true;
}
bool CDatabase::InsertSql(char *sqlstr, my_ulonglong& insert_id)
{
    if (ExecSql(sqlstr))
    {
        insert_id = mysql_insert_id(mysql);
        return true;
    }

    return false;
}


