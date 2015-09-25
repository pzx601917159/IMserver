/*********************************************************************
	> File Name: connection.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 26 August 2015 04:17:31 PM CST
 ********************************************************************/

#include "connection.h"
#include "circularbuffer.h"
#include <unistd.h>
#include "log.h"

Connection::Connection()
{
    LOG_INFO("new Connection!");
    m_sockfd = -1;
    m_inputbuffer = new CircularBuffer(4096);
    m_inputbuffer->Init();
    m_outputbuffer = new CircularBuffer(4096);
    m_outputbuffer->Init();
}

Connection::~Connection()
{
    LOG_INFO("destroy Connection!");
    close(m_sockfd);
    delete m_inputbuffer;
    delete m_outputbuffer;
}

void Connection::HandleEvents(epoll_event* ev)
{
    LOG_INFO("handle events!");
    if(ev->events & EPOLLIN)
    {
        HandleInputEvents();
    }
    else if(ev->events & EPOLLOUT)
    {
        HandleOutputEvents();
    }
}

void Connection::HandleInputEvents()
{
    LOG_INFO("handle input events!");
    RecvData();
}

void Connection::HandleOutputEvents()
{
    LOG_INFO("handle output events!");
}

size_t Connection::RecvData()
{
    size_t ret=0;
    if((ret = recv(m_sockfd, m_inputbuffer->Tail(),m_inputbuffer->RecvSize(),0)) > 0)
    {
        LOG_INFO("recv data lenth = %lu!",ret);
    }
    else
    {
        OnClose();
    }
    return ret;
}

size_t Connection::SendData()
{
    size_t ret = 0;
    return ret;
}

void Connection::OnClose()
{
    LOG_INFO("On socket close!");
    delete this;
}
