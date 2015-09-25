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
 
#define EPOLLMAX    1024
#define LISTENQ     20      
#define HANDLEMAX   100     

class Server
{
    public:
    Server(uint16_t port);
    ~Server();

    void Init();

    void Start();

    void HandleServerEvents();

    private:
    void InitSockfd();

    void SetNonblock(int sockfd);

    void Bind();

    void Listen();

    private:
    struct sockaddr_in m_addr;         
    int m_sockfd;
    epoll_event m_events[HANDLEMAX];
    int m_epollfd;                  
    epoll_event m_ev;               
    bool m_serverState;             
    int m_eventsCount;                
    std::map<uint64_t,Connection*> m_connections; 
    
};

