#include "client.h"
#include "logger.h"
#include "connection.h"
#include "epollselector.h"
#include "wheeltimer.h"
#include "singleton.h"

#include "sendmsg.pb.h"

Client::Client()
{
    m_clientState = false;
    m_selector = new EPollSelector();
    //定时器
    m_wheeltimer = Singleton<WheelTimer>();
}

Client::~Client()
{
    delete m_selector;
};

void Client::Init()
{
    //初始化epoll
    m_selector->Init();
}

//初始化服务器
Connection* Client::CreateConn()
{
    int sockfd;
    //初始化套接字
    InitSockfd(sockfd);
    return Connect(sockfd);
}

//启动服务器
void Client::Start()
{
    //connect
    log::log(Info,"start Client...");
    m_clientState = true;

    //相当于epoll_ctl_add
    log::log(Info,"m_selector.size:",m_selector->Size());

    log::log(Info,"run Client");

    //创建连接
    CreateConn();

    while(m_clientState)
    {
        //非阻塞的connect这里不触发事件
        m_selector->Select();
        m_wheeltimer->Click();
    }
}

//生成套接字,并设置为非阻塞
void Client::InitSockfd(int& sockfd)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);//tcp
    //设置非阻塞
    SetNonblock(sockfd);
}

/***功能:设置为非阻塞***/
/***参数:套接字***/
void Client::SetNonblock(int& sockfd)
{
    int opts;
    opts = fcntl(sockfd,F_GETFL);
    if(opts < 0)
    {
        log::log(Error,"fcntl(sock,GETFL)");
        return;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,opts)<0)
    {
        log::log(Error,"fcntl(sock,SETFL,opts)");
        return;
    }
}

//连接服务器
Connection* Client::Connect(int& sockfd)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(5555);   
    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0)
    {
        log::log(Info,"inet_pton error");
        return NULL;
    }
    //连接
    if(connect(sockfd, (sockaddr *)&addr,sizeof(addr)) < 0)
    {
        if(errno != EINPROGRESS)
        {
            log::log(Info,"errno is not EINPROGRESS");
            return NULL;
        }
        log::log(Info,"errno is EINPROGRESS");
        //log::log(Info,"connect error");
        //return NULL;
    }

    //创建连接
    Connection* conn  = new Connection(sockfd,addr,this);
    //添加到epoll中
    m_selector->Select(conn,0,SEL_RW);
    
    std::map<uint64_t,Connection*>::iterator it = m_connections.find(sockfd);

    if(it != m_connections.end())
    {
        Connection* connection = it->second;
        //删除epoll侦听的套接字
        m_selector->Remove(connection);
        m_connections.erase(it);
        delete connection;        
    }
    m_connections[sockfd] = conn;

    //创建定时任务
    conn->SetReconnTimer(m_wheeltimer->InsertTask(conn,5000));
    return conn;
}

void Client::DeleteConn(uint64_t key)
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

void Client::BroadcastMsg(SendMsg* msg)
{
    std::map<uint64_t,Connection*>::iterator it = m_connections.begin();

    while(it != m_connections.end())
    {
        it->second->SendRequest(msg,CHAT_MSG);
        ++it;
    }
}

//写
void Client::OnWrite()
{
    log::log(Info,"on write");
};

//读
void Client::OnRead()
{
    log::log(Info,"On read");
};

