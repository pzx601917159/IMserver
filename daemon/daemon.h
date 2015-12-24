/*********************************************************************
	> File Name: daemon.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Sunday 06 September 2015 10:58:50 AM CST
 ********************************************************************/

#ifndef _DAEMON_H
#define _DAEMON_H
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <fcntl.h>
#include "../log/logger.h"

//守护进程类
class Daemon
{
    FILE* m_newFile;
    int m_old;
    public:
    Daemon(bool daemon,int sz,char*args[]);

    ~Daemon();
};
#endif //_DAEMON_H
