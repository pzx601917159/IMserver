/*********************************************************************
	> File Name: daemon.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Sunday 06 September 2015 10:58:50 AM CST
 ********************************************************************/

#include "daemon.h"

//设置core文件的大小
static void set_core_limit(int cur, int max)
{
    rlimit rl;
    rl.rlim_cur = cur;
    rl.rlim_max = max;
    setrlimit(RLIMIT_CORE, &rl);
}

//设置文件描述父的限制
static void set_file_limit(int cur, int max)
{
    rlimit rl;
    rl.rlim_cur = cur;
    rl.rlim_max = max;
    setrlimit(RLIMIT_NOFILE, &rl);
    rlimit limit;
    getrlimit(RLIMIT_NOFILE, &limit);
}


//守护进程类
Daemon::Daemon(bool daemon,int sz,char*args[]):m_old(0)
{
    /*
    if(sz < 2)
    {
        printf("please input file name\n");
        exit(0);
    }
    */
    pid_t pid,sid;

    if(daemon)
    {
        pid=fork();
        if(pid > 0)
        {
            //是父进程，结束父进程
            exit(EXIT_SUCCESS);
        }
        if(pid < 0)
        {
            //fork失败，退出
            exit(EXIT_FAILURE);
        }
                
        umask(0);

        //第一子进程成为新的会话组的组长和进程组长，并于控制终端分离
        sid = setsid();
        if(sid < 0)
        {
            exit(EXIT_FAILURE);
        }
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
       
    char *fn;
    //传入
    if(sz >= 2)
    {
        fn = args[1];
    }
    else
    {
        fn = const_cast<char*>("mylog.txt");
    }
    if(!log::setlog(fn,100*1024))
    {
        exit(1);
    }
    set_file_limit(65535, 1000000);
    set_core_limit(1024 * 1024 * 1024, 1024 * 1024 * 1024);
}

Daemon::~Daemon()
{
    if(m_newFile)
    {
        fclose(m_newFile);
        dup2(m_old,1);
    }
}
