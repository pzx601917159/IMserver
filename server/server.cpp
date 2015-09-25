#include "server.h"
#include "log.h"

Server::Server(uint16_t port)
{
    m_eventsCount = 0;
    m_serverState = false;
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons(port);
    m_epollfd = epoll_create(EPOLLMAX);
}

Server::~Server()
{
};

void Server::Init()
{
    LOG_INFO("Init server...");
    InitSockfd();
    Bind();
    Listen();
}

void Server::Start()
{
    LOG_INFO("start server...");
    m_serverState = true;
    m_ev.data.fd = m_sockfd;
    m_ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_sockfd, &m_ev);
    LOG_INFO("run server");
    while(m_serverState)
    {
        m_eventsCount = epoll_wait(m_epollfd, m_events, HANDLEMAX, -1);
        for(int i=0; i<m_eventsCount; i++)
        {
            if(m_events[i].data.fd == m_sockfd)
            {
                HandleServerEvents();
            }
            else if(m_events[i].events & EPOLLIN | m_events[i].events & EPOLLOUT)
            {
                epoll_event ev = m_events[i];
                ((Connection*)(ev.data.ptr))->HandleEvents(&ev);
            }
        } 
        
    }
}

void Server::HandleServerEvents()
{
    LOG_INFO("handle accept evets!");
    Connection* conn = new Connection();
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr);
    conn->m_sockfd = accept(m_sockfd,(sockaddr *)&addr,&len);
           
    if(conn->m_sockfd<0)
    {
        perror("connfd<0");
        return;
    } 
    conn->m_addr.sin_addr = addr.sin_addr;
    conn->m_addr.sin_port = ntohs(addr.sin_port);
    std::map<uint64_t, Connection*>::iterator it = m_connections.find(conn->m_addr.sin_addr.s_addr);
    if(it != m_connections.end())
    {
        Connection* connection =it->second;
        m_connections.erase(it);
        delete connection;
    }
    m_connections[conn->m_addr.sin_addr.s_addr] = conn;

    char *str = inet_ntoa(addr.sin_addr);

    unsigned short port = ntohs(addr.sin_port);
    LOG_INFO("accept a connection from ip:",str,"port:",port);

    SetNonblock(conn->m_sockfd);
    m_ev.data.ptr = conn;

    m_ev.events=EPOLLIN | EPOLLET;

    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,conn->m_sockfd,&m_ev);
}

void Server::InitSockfd()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);//tcp
    SetNonblock(m_sockfd);
}

void Server::SetNonblock(int sockfd)
{
    int opts;
    opts = fcntl(sockfd,F_GETFL);
    if(opts < 0)
    {
        perror("fcntl(sock,GETFL)");
        return;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return;
    }
}

void Server::Bind()
{
    socklen_t len = sizeof(m_addr);
    bind(m_sockfd,(sockaddr *)&m_addr, len);
}

void Server::Listen()
{
    listen(m_sockfd,LISTENQ);
}


