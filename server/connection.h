/*********************************************************************
	> File Name: connection.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 26 August 2015 04:17:31 PM CST
 ********************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/epoll.h>
#include "circularbuffer.h"//buffer

class Connection
{
    public:
   
    enum ConnType{
        ACCEPT,
        CONNECT
    };

    ConnType cType;

    Connection();
    ~Connection();
    void HandleEvents(epoll_event* ev);

    void HandleInputEvents();
    void HandleOutputEvents();

    bool SendBin(const char* data,size_t size,uint32_t uri);

    void SetTimeOut(int);

    void OnTimeOut();

    void OnClose();

    size_t RecvData();

    size_t SendData();

    public:
    int m_sockfd;
    struct sockaddr_in m_addr;

    CircularBuffer* m_inputbuffer;
    CircularBuffer* m_outputbuffer;

};

