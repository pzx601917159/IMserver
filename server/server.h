#ifndef _SERVER_H
#define _SERVER_H

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

class Connection;
 
#define LISTENQ     20      //连接请求队列的最大长度

class EPollSelector;
class SendMsg;

class Server:public IConnection
{
    public:
    Server(uint16_t port=5555);
    ~Server();

    //初始化服务器
    void Init();

    //启动服务器
    void Start();

    //删除连接
    void DeleteConn(uint64_t);

    //获取套接字
    int GetSockfd() const;

    EPollSelector* GetSelector() const {return  m_selector;}

    //写
    void OnWrite(){};

    //读
    void OnRead();

    //广播消息
    void BroadcastMsg(::google::protobuf::Message* msg,ReqType type);

    private:
    //生成套接字,并设置为非阻塞
    void InitSockfd();

    /***功能:设置为非阻塞***/
    /***参数:套接字***/
    void SetNonblock(int sockfd);

    //绑定套接字
    void Bind();

    //侦听套接字
    void Listen();


    private:
    struct sockaddr_in m_addr;      //地址
    int m_sockfd;
    bool m_serverState;             //服务器状态
    EPollSelector* m_selector;      //封装epoll的处理
    //64位系统中long占8位
    std::map<uint64_t,Connection*> m_connections; //连接map

    
};

#endif //_SERVER_H
