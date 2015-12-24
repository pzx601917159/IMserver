#ifndef _CLIENT_H
#define _CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <map>

#include "connection.h"
#include "wheeltimer.h"

class Connection;
 
#define EPOLLMAX    1024    //接受的最大长度
#define LISTENQ     20      //连接请求队列的最大长度
#define HANDLEMAX   100     //同时需要处理的事件的数量

class EPollSelector;
class SendMsg;
class TimerTask;
class WheelTimer;
class Timer;

class Client:public IConnection
{
    public:
    Client();
    ~Client();

    //初始化服务器
    void Init();

    //启动服务器
    void Start();

    //删除连接
    void DeleteConn(uint64_t);

    //获取套接字
    int GetSockfd() const{return 0;};

    EPollSelector* GetSelector() const {return  m_selector;}

    //写
    void OnWrite();

    //读
    void OnRead();

    //广播消息
    void BroadcastMsg(SendMsg* msg);

    Connection* Connect(int& sockfd);

    Connection* CreateConn();

    //获取定时器
    WheelTimer* GetWheelTimer(){return m_wheeltimer;}

    private:
    //生成套接字,并设置为非阻塞
    void InitSockfd(int& sockfd);

    /***功能:设置为非阻塞***/
    /***参数:套接字***/
    void SetNonblock(int& sockfd);

    private:
    bool m_clientState;             //服务器状态
    EPollSelector* m_selector;      //封装epoll的处理
    std::map<uint64_t,Connection*> m_connections; //连接map
    WheelTimer* m_wheeltimer;        //时间轮定时器
    Timer* m_timer;                 //定时器
    
};

#endif //_CLIENT_H
