#include "mysqldatabase.h"
#include "db_pool.cpp"
#include "readconfig.h"

int main()
{
	CDatabase database;

    CDatabase* pdb = CDBPool<CDatabase>::Instance()->Acquire();

    if (NULL == pdb)
    {
       printf("Fail to connect DB when creating user");
       return 0;
    }
    printf("connect success!\n");


    //查询数据
    char str[]="select * from tab_base";

    pdb->SelectBySql(str);
    //列出数据
    pdb->ListRecord();

    //遍历记录

   while(pdb->GetRecord())
	{
		char buffer[100]={0};
		STRNCPY(buffer,pdb->row[1],sizeof(buffer));
		printf("buffer = %s\n",buffer);
	}
    pdb->FreeRecord();
    CDBPool<CDatabase>::Instance()->Release(pdb);

	return 0;
}
