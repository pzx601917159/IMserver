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
    char* tmp = NULL;
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

    if(g_daemon)
    {
        Daemon daemon;
    }

    Server server(2222);

    server.Init();

    server.Start();

    return 0;
}


