/*********************************************************************
	> File Name: epollselector.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 17 December 2015 02:44:14 PM CST
********************************************************************/

#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "epollselector.h"
#include "iconnection.h"


const int MAX_EVENTS = 500;
const int EPOLL_SIZE = 10240;
const int MAX_SHORT = 65535;


inline void SetupEpoll(int epollfd,int option,int sockfd,epoll_event &ev)
{
    int ret = epoll_ctl(epollfd,option,sockfd,&ev);
    if(ret != 0)
    {
        //错误处理
    }
}

EPollSelector::EPollSelector()
{
    m_epollfd = -1;
}

EPollSelector::~EPollSelector()
{
    if(m_epollfd != -1)
    {
        close(m_epollfd);
    }
}

bool EPollSelector::Init()
{
    if(m_epollfd == -1)
    {
        signal(SIGPIPE,SIG_IGN);
        m_epollfd = epoll_create(MAX_SHORT);
    }
    return m_epollfd != -1;
}


void EPollSelector::Select(IConnection* conn,int remove,int add)
{
    epoll_event ev;
    memset(&ev,0,sizeof(ev));
    //存储这个连接
    ev.data.ptr = conn;
    ev.events = EPOLLIN | EPOLLET;
    std::pair<SockFdSet::iterator, bool> p = m_sockfds.insert(conn->GetSockfd());
    if(p.second)
    {
        if(SEL_WRITE & add)
        {
            ev.events |= EPOLLOUT;
        }
        SetupEpoll(m_epollfd, EPOLL_CTL_ADD,conn->GetSockfd(), ev);
    }
    else
    {
        if(SEL_READ & remove)
        {
            ev.events &= ~EPOLLIN;
        }
        if(SEL_WRITE & remove)
        {
            ev.events &= ~EPOLLOUT;
        }
        if(SEL_READ & add)
        {
            ev.events |= ~EPOLLIN;
        }
        if(SEL_WRITE & add)
        {
            ev.events |= ~EPOLLOUT;
        }
        SetupEpoll(m_epollfd, EPOLL_CTL_MOD, conn->GetSockfd(), ev);
    }
}

//epoll_select
void EPollSelector::Select()
{
    epoll_event events[EPOLL_SIZE];
    int ret = epoll_wait(m_epollfd,events,EPOLL_SIZE,0);
    for(int i = 0; i < ret; ++i)
    {
        IConnection* conn = (IConnection*)events[i].data.ptr;
        if(events[i].events & (EPOLLIN|EPOLLERR|EPOLLHUP))
        {
            conn->OnRead();
        }
        if(events[i].events & EPOLLOUT)
        {
            conn->OnWrite();
        }
    }
}

size_t EPollSelector::Size() const
{
    return m_sockfds.size();
}

//从epoll中移除
void EPollSelector::Remove(IConnection* conn)
{
    epoll_event ev;
    SetupEpoll(m_epollfd, EPOLL_CTL_DEL,conn->GetSockfd(),ev);
    m_sockfds.erase(conn->GetSockfd());
}

