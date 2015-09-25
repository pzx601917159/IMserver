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

class Daemon
{
    public:
    Daemon()
    {
        pid_t pid;
        pid = fork();
        if(pid < 0)
        {
            exit(1);
        }
        else if(pid > 0)
        {
            exit(0);
        }
        setsid();

        pid = fork();
        if(pid < 0)
        {
            exit(1);
        }
        else if(pid > 0)
        {
            exit(0);
        }
    }

    ~Daemon()
    {
    }
};
#endif //_DAEMON_H
