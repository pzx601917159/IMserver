#ifndef _VOS_TYPES_H
#define _VOS_TYPES_H

#include <pthread.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/un.h>
#ifndef S8
typedef char S8;
#endif

#ifndef U8
typedef unsigned char U8;
#endif

#ifndef S16
typedef short S16;
#endif

#ifndef U16
typedef unsigned short U16;
#endif

#ifndef S32
typedef int S32;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef ULL64
#ifndef __ADD_MY_MIB__
typedef long long ULL64;
typedef long long U64;
#endif
#endif

#ifndef CH8
typedef char CH8;
#endif

#ifndef BOOLVAL
typedef enum
{
	BOOLFALSE = 0,
	BOOLTRUE  =1,
}BOOLVAL;
#endif

//zfei added, for cross compile timer
#ifndef TIMER_T
typedef timer_t TIMER_T;
#endif

#ifndef PTHREAD_T
typedef pthread_t PTHREAD_T;
#endif

#ifndef SEM_T
typedef sem_t SEM_T;
#endif

#ifndef MQD_T
typedef mqd_t MQD_T;
#endif


typedef  unsigned int             uint;
typedef  unsigned char            byte;
typedef  unsigned short           word;
typedef  unsigned char             BCD;
#ifndef 	WIN32
typedef  unsigned short           WORD;
typedef  unsigned int             DWORD;
typedef  unsigned char		   BYTE;
typedef  unsigned long		   DWORD64;
typedef  int						BOOL;
typedef  unsigned short USHORT;

#define  FALSE           0
#define  TRUE            1
#endif

typedef unsigned long		Lenptr;


#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define INVALID_NUMBER                      -1      /*无效数字*/
#define MAX_STRING_LEN                      10240  //最大字符串长度
#define MAX_MONITOR_NUM                     1024  //最大监测对象
#ifndef WIN32
#define INVALID_HANDLE_VALUE                -1
#endif
#define INTERNAL_BUFFER_LEN                 1024 * 64

#ifndef MAX_PATH
#define MAX_PATH          		            260
#endif

//时间常量
#define HALF_MINUTE (30*1000)
#define ONE_MINUTE (60*1000)  //分
#define TEN_SECOND (10*1000)
#define FIVE_SECOND (5*1000)
#define QUARTER_MINUTE (15*1000)
#define ONE_HOUR (3600*1000) //hour
#define TIMEOUT                            10      //超时时间(单位:秒)
#define ONE_SECOND                         1000  //1秒
#define ONE_DAY_SECOND       (24*60*60)          //天把天转换为秒，注意不能为毫秒
#define MIN_RATE_DUR         0                   //最短可计费时长
#define ACCTCYCLEDATE		 1                   //帐期切换时间
#define  MAX_SEND_FAIL_NUM   10                  //最大发送失败数

// RADIUS包最大重发次数:
#define MAX_RETRANSMIT_COUNT    20
//已处理RADIUS包保存最大时间
#define MAX_COUNTPACKETRESERVE   3

//EIA设备最大端口数目
#define MAX_EIA_PORTS            32

//允许在dog失败多少次以内，认证继续运行
#define MAX_COUNTREADDOGFAILOR   10

//定义100万张/月的同时在线用户数目
#define MAX_ONLINE_USER_COUNT    300
#define DEFAULT_MAX_ONLINE_USER_COUNT MAX_ONLINE_USER_COUNT


// 检查一个文件系统对象是否目录类型(相对与文件类型).
//#define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)

// 安全的字符串拷贝函数.
#define STRNCPY(szDst,szSrc,nLen)  strncpy(szDst,szSrc,nLen); \
	(szDst)[nLen - 1] = '\0'

// 表达式为false时执行return操作,并记录assert消息.
#define RETURN_ON_FAIL(expr) \
    if(!(expr)) \
        { \
        TT_ASSERT(0); \
        return; \
        }

#define RETURN_VALUE_ON_FAIL(expr,ret_value) \
    if(!(expr)) \
        { \
        TT_ASSERT(0); \
        return (ret_value); \
        }

#endif /* _VOS_TYPES_H */

