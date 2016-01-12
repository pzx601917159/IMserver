#include "server.h"
#include "logger.h"
#include "connection.h"
#include "epollselector.h"

#include "sendmsg.pb.h"

Server::Server(uint16_t port)
{
    m_serverState = false;
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons(port);
    m_selector = new EPollSelector();
}

Server::~Server()
{
    delete m_selector;
};

//初始化服务器
bool Server::Init()
{
    log::log(Info,"Init server...");
    //都为true表示初始化成功
    if(InitSockfd() && Bind() && Listen() && m_selector->Init())
    {
        return true;
    }
    return false;
}

//启动服务器
void Server::Start()
{
    log::log(Info,"start server...");
    m_serverState = true;
    //m_ev.data.fd = m_sockfd;
    //m_ev.events = EPOLLIN|EPOLLET;
    //注册事件
    //epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_sockfd, &m_ev);

    //相当于epoll_ctl_add
    m_selector->Select(this,0,0);
    log::log(Info,"m_selector.size:",m_selector->Size());

    log::log(Info,"run server");
    while(m_serverState)
    {
        m_selector->Select();
    }
}

//服务器侦听套接字上的事件
void Server::OnRead()
{
    log::log(Info,"handle accept evets!");
    //建立新的连接
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr);
    int sockfd = accept(m_sockfd,(sockaddr *)&addr,&len);
           
    if(sockfd<0)
    {
        log::log(Error,"connfd < 0");
        return;
    }
    //设置为非阻塞
    SetNonblock(sockfd);
   
    //创建连接
    Connection* conn = new Connection(sockfd,addr,this);
 
    std::map<uint64_t, Connection*>::iterator it = m_connections.find(sockfd);
    if(it != m_connections.end())
    {
        Connection* connection =it->second;
        //这个连接已经存在，其实是不可能的，但是还是删除把
        m_selector->Remove(connection);
        m_connections.erase(it);
        delete connection;
    }
    //插入m_connections中,以IP为键，这样不合理，同一个IP会登陆多个客户端,所以还是用套接字作为键吧
    m_connections[sockfd] = conn;

    //打印一些信息
    log::log(Info,"m_connections.size() = ",m_connections.size());
    char *str = inet_ntoa(addr.sin_addr);
    unsigned short port = ntohs(addr.sin_port);
    log::log(Info,"accept a connection from ip:",str,"port",port);

    //添加到epoll中
    m_selector->Select(conn,0,0);
    log::log(Info,"m_selector.size:",m_selector->Size());
}

//生成套接字,并设置为非阻塞
bool Server::InitSockfd()
{
    log::log(Info,"init sock fd");
    if((m_sockfd = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        return false;
    }
    //设置非阻塞

    return SetNonblock(m_sockfd);
}

/***功能:设置为非阻塞***/
/***参数:套接字***/
bool Server::SetNonblock(int sockfd)
{
    int opts;
    opts = fcntl(sockfd,F_GETFL);
    if(opts < 0)
    {
        log::log(Error,"fcntl(sock,GETFL)");
        return false;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,opts)<0)
    {
        log::log(Error,"fcntl(sock,SETFL,opts)");
        return false;
    }
    return true;
}

//绑定套接字
bool Server::Bind()
{
    log::log(Info,"bind");
    int opt = 1;
    if(setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) != 0)
    {
        log::log(Info,"setsockopt Error");
        return false;
    }
    socklen_t len = sizeof(m_addr);
    if(bind(m_sockfd,(sockaddr *)&m_addr, len) == -1)
    {
        log::log(Info,"bind error");
        return false;
    }
    return true;
}

//侦听套接字
bool Server::Listen()
{
    log::log(Info,"listen");
    if( listen(m_sockfd,LISTENQ) == -1 )//连接请求队列的最大长度为20
    {
        log::log(Info,"listen failed errno:",errno);
        return false;
    }
    return true;
}

void Server::DeleteConn(uint64_t key)
{
    std::map<uint64_t,Connection*>::iterator it = m_connections.find(key);

    if(it != m_connections.end())
    {
        //删除epoll侦听的套接字
        m_selector->Remove(it->second);

        m_connections.erase(it);
    }
    log::log(Info,"m_connections.size() = ",m_connections.size());
}


int Server::GetSockfd() const
{
    return m_sockfd;
}

//广播消息,广播给所有用户包括自己
void Server::BroadcastMsg(::google::protobuf::Message* msg,ReqType type)
{
    std::map<uint64_t,Connection*>::iterator it = m_connections.begin();

    while(it != m_connections.end())
    {
        it->second->SendRequest(msg,type);
        ++it;
    }
}
