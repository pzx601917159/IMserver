/*********************************************************************
	> File Name: main.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 02 September 2015 09:57:36 AM CST
 ********************************************************************/

#include "server.h"
#include "daemon.h"
#include "singleton.h"

#include <sys/param.h>
#include <getopt.h>
#include <unistd.h>
#include <rpc/types.h>

#define PROGRAM_VERSION "1.0"
bool g_daemon = false;

//命令行出入的是int型,本来是uint16_t，为了兼容命令行选项先这么写吧
int g_port = 2222;


static const struct option long_options[] = 
{
    {"daemon",no_argument,NULL,'d'},
    {"port",required_argument,&g_port,'p'},
    {"help",no_argument,NULL,'h'},
    {"version",no_argument,NULL,'V'}
};

void useage(const char* arg)
{
    fprintf(stderr,
            "%s [option]:\n"
            "-d|--daemon    run as a daemon\n"
            "-p|--port      sever port(default:2222)\n"
            "-h|--help      show help\n"
            "-v|--version   show version\n"
            ,arg);
}

void version()
{
    fprintf(stderr,"my char server version:%s\n",PROGRAM_VERSION);
}

bool checkOptions(int argc,char** argv)
{
    int opt = 0;
    int options_index = 0;
    while((opt = getopt_long(argc,argv,"dp:hv",long_options,&options_index)) != EOF)
    {
        switch(opt)
        {
            case 'd':
                g_daemon = true;
                break;
            case 'p':
                g_port = atoi(optarg);
                break;
            case 'h':
                useage(argv[0]);
                return false;
            case 'v':
                version();
                return false;
        }
    }
    return true;
}

int main(int argc,char** argv)
{
    if(!checkOptions(argc,argv))
    {
        return 0;
    }
    
    //是否以守护进程方式启动
    Daemon daemon(g_daemon,argc,argv);

    //Server *server = Singleton<Server,g_port>();
    Server server(g_port);
    
    if(server.Init())
    {
        server.Start();
    }
    else
    {
        log::log(Info,"server init err");
    }

    return 0;
}


