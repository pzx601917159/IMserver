/*********************************************************************
	> File Name: connection.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 26 August 2015 04:17:31 PM CST
 ********************************************************************/


#include <unistd.h>
#include <string.h>
#include <map>

#include "connection.h"
#include "logger.h"
#include "singleton.h"
#include "client.h"
#include "request.h"
#include "reguserreq.pb.h"
#include "reguserres.pb.h"

//mysql
#include "mysqldatabase.h"
#include "db_pool.cpp"
#include "readconfig.h"
#include "circularbuffer.h"
#include "epollselector.h"
#include "timertask.h"
#include "timer.h"

#include "loginreq.pb.h"
#include "loginres.pb.h"
#include "sendmsg.pb.h"
#include "online.pb.h"
#include "offline.pb.h"

const uint32_t BUFFER_LEN=4096;
const uint32_t MAX_REQ_LEN=1024;
const uint32_t MAX_SQL_LEN =1024;
const uint32_t HEADER_LEN=8;
const uint32_t MSGTYPE_LEN=4;


Connection::Connection(int sockfd,sockaddr_in addr,Client* client)
{
    m_userid = 0;
    log::log(Info,"new Connection!");
    m_sockfd = sockfd;
    m_addr = addr;
    m_inputbuffer = new CircularBuffer(BUFFER_LEN);
    //init才分配内存
    m_inputbuffer->Init();
    m_outputbuffer = new CircularBuffer(BUFFER_LEN);
    //init才分配内存
    m_outputbuffer->Init();
    m_client = client;
    m_enable = false;
    m_reconnTimer = NULL;
    m_pingTimer = NULL;
}

Connection::~Connection()
{
    log::log(Info,"destroy Connection!");
    //关闭套接字
    close(m_sockfd);
    //释放缓冲区的内存
    delete m_inputbuffer;
    delete m_outputbuffer;
}


//处理输入事件
void Connection::OnRead()
{
    log::log(Info,"on read");
    //接收数据
    if(m_enable)
    {
        RecvData();
    }
}

//处理输出事件
void Connection::OnWrite()
{
    log::log(Info,"On write--------");
    if(m_enable)
    {
        //继续发送数据
    }
    else
    {
        if(!CheckConnect(this))
        {
            log::log(Info,"connect failed");
            OnClose();
        }
        else
        {
            log::log(Info,"connect success");
            m_enable = true;
            OnConnected();
        }
    }
}

//接收数据
size_t Connection::RecvData()
{
    int ret=0;
    //用inputbuffer接收,循环接受直接接受完成
    while((m_inputbuffer->AvailableSize() > 0) && ((ret = recv(m_sockfd, m_inputbuffer->Tail(),m_inputbuffer->RecvSize(),0)) > 0))
    {
        //接受到对面发送的数据
        log::log(Info,"recv data lenth = ",ret);
        //接受完成后buffer的size_不能自动更新，所以，这里使用函数更新size_
        m_inputbuffer->UpdateSize(ret);
    }
    //接受完成之后对buffer中的数据进行解析直到解析完成
    while(m_inputbuffer->Size() > HEADER_LEN)
    {
        //获取数据的长度
        uint32_t len =  m_inputbuffer->ReadUint32Only();
        log::log(Info,"Msg len:",len);
        if(m_inputbuffer->Size() < len )
        {
            //还没有接受到一个完整的请求
            //break;
        }
        //读取消息的类型,消息的类型不能放在protobuf中，因为字节不固定
        uint32_t type = m_inputbuffer->ReadUint32Only(MSGTYPE_LEN);
        log::log(Info,"Msg type:",type);
        m_inputbuffer->SkipData(HEADER_LEN);
        HandleReq((ReqType)type,len);

    }
    log::log(Info,"ret:",ret);
    if(ret == 0)
    {
        //连接断开
        OnClose();
    }
    return ret;
}

//发送数据
size_t Connection::SendData()
{
    size_t ret = 0;
    //发送，没发送完的放到发送buffer中
    return ret;
}


//连接断开的处理
void Connection::OnClose()
{
    //删除对应的定时器任务
    if(m_reconnTimer != NULL)
    {
        //删除定时器
        m_reconnTimer->Destroy();
        m_reconnTimer = NULL;
    }
    if(m_pingTimer != NULL)
    {
        m_pingTimer->Destroy();
        m_pingTimer = NULL;
    }

    m_enable = false;
    log::log(Info,"On socket close!");
    //删除server中记录的连接中的元素
    m_client->DeleteConn(m_sockfd);
    delete this;
}

int Connection::GetSockfd() const
{
    return m_sockfd;
}


//传入参数：请求类型，请求长度
void Connection::HandleReq(ReqType type,size_t len)
{
    switch(type)
    {
        //根据消息类型解析
        //注册用户请求
        case REGUSER_RES:
            {
                log::log(Info,"recv register user req");
                //处理完成之后就跳过这些数据
                RegUserReq req;
                //从一个string中构造请求
                //这里先读取1024个字节吧，这个buffer还是修改了比较好
                if(len < MAX_REQ_LEN)
                {
                    char buffer[MAX_REQ_LEN];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-HEADER_LEN);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    log::log(Info,"username:",req.name());
                    log::log(Info,"password:",req.password());
                    HandleRegReq(&req);
                }
                m_inputbuffer->SkipData(len-HEADER_LEN);
                log::log(Info,"m_inputbuffer->Size():",m_inputbuffer->Size());
            }
            break;
        case LOGIN_RES:
            {
                log::log(Info,"recv login res");
                LoginRes res;
                if(len < MAX_REQ_LEN)
                {
                    char buffer[MAX_REQ_LEN];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-HEADER_LEN);
                    std::string str = buffer;
                    res.ParseFromString(str);
                    HandleLoginRes(&res);
                } 
            }
            break;
        //聊天消息
        case CHAT_MSG:
            {
                log::log(Info,"recv char msg");
                SendMsg req;
                if(len < MAX_REQ_LEN)
                {
                    char buffer[MAX_REQ_LEN];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-HEADER_LEN);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    HandleChatMsg(&req);
                }
            }
            break;
        //上线消息
        case ONLINE_MSG:
            {
                log::log(Info,"recv online msg");
                Online req;
                if(len < MAX_REQ_LEN)
                {
                    char buffer[MAX_REQ_LEN];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-HEADER_LEN);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    log::log(Info,"online name:",req.name(),"id:",req.id());
                }
            }
            break;
        //离线消息
        case OFFLINE_MSG:
            {
                log::log(Info,"recv offline msg");
                Offline req;
                if(len < MAX_REQ_LEN)
                {
                    char buffer[MAX_REQ_LEN];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-HEADER_LEN);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    log::log(Info,"offline id:",req.id());
                }
            }
            break;
        //others:
        default:
            {
                //接受到一些非法消息怎么处理?,这个放在后面吧
                log::log(Info,"recv unknown msg!");
                //处理完成之后就跳过这些数据
                m_inputbuffer->SkipData(len);
                log::log(Info,"m_inputbuffer->Size():",m_inputbuffer->Size());
            }
            break;
    }
    //处理请求
}

//注册用户
void Connection::HandleRegReq(RegUserReq* req)
{
    RegUserRes res;
    res.set_rescode(FAILED);

    //判断用户名是否存在，如果不存在，操作数据库，注册，如果存在，返回错误码
    CDatabase *pdb = CDBPool<CDatabase>::Instance()->Acquire();
    if(pdb != NULL)
    {
        char sql[MAX_SQL_LEN];
        memset(sql,0,sizeof(sql));
        snprintf(sql,sizeof(sql),"select * from tab_user where username='%s'",req->name().c_str());
        if(pdb->SelectBySql(sql))
        {
            if(pdb->GetRowNum())
            {
                log::log(Info,"username have been Register");
                //查找到了
            }
            else
            {
                //注册用户
                memset(sql,0,sizeof(sql));
                snprintf(sql,sizeof(sql),"insert into tab_user(username,password) values('%s','%s')",req->name().c_str(),req->password().c_str());
                if(pdb->SelectBySql(sql))
                {
                    //注册成功
                    log::log(Info,"Register success!");
                    res.set_rescode(SUCCESS);
                }
                else
                {
                    log::log(Info,"database error");
                    res.set_rescode(INNER_ERR);
                }
            }
            //释放结果集
            pdb->FreeRecord();
        }
        CDBPool<CDatabase>::Instance()->Release(pdb);
    }
    SendRequest(&res,REGUSER_REQ);
}


void Connection::HandleLoginRes(LoginRes* res)
{
    //返回userid把
    log::log(Info,"rescode:",res->rescode());
    if(res->rescode() == SUCCESS)
    {
        //登陆成功
        log::log(Info,"login success,userid:",res->userid());
    
        //发送测试的聊天消息
        SendMsg msg;
        msg.set_msg("hello,my name is 007");
        msg.set_userid(res->userid());
        SendRequest(&msg,CHAT_MSG);
    }
}

void Connection::HandleChatMsg(SendMsg* msg)
{
    log::log(Info,"recv msg:","userid:",msg->userid(),"msg:",msg->msg());
    //broadcast
    //m_server->BroadcastMsg(msg);
}

void Connection::SendRequest(::google::protobuf::Message* req,ReqType type)
{
    //发送请求
    std::string sreq;
    req->SerializeToString(&sreq);
    uint32_t length = htonl(sreq.size() + HEADER_LEN);
    uint32_t newtype = htonl(type);
    
    sreq.insert(0,(char*)&newtype,sizeof(uint32_t));
    sreq.insert(0,(char*)&length,sizeof(uint32_t));
    SendData(sreq.c_str(),sreq.size());
}


bool Connection::CheckConnect(Connection* conn)
{
    log::log(Info,"check connect");
    int err = 0;
    socklen_t errlen = sizeof(err);

    if (getsockopt(conn->GetSockfd(), SOL_SOCKET, SO_ERROR, &err, &errlen) == -1) 
    {
        log::log(Info,"inner err");
        return false;
    }
    log::log(Info,"err:",err);
    if(err != 0)
    {
        return  false;
    }
    return true;
}

void Connection::SendData(const char* data,size_t size)
{
    log::log(Info,"send data");
    //发送之前判断连接是否可用

    //直接发送还是放在buffer中呢
    //发送数据
    char* dt = const_cast<char*>(data);

    size_t sendlen;
    do
    {
        sendlen = 0;
        //这里可以使用send
        sendlen = write(m_sockfd,dt,size);
        //更新需要发送的长度
        size -= sendlen;
        dt += sendlen;
        if(sendlen == 0 || size == 0)
        {
            break;
        }
    }while(true);

    //没发送完的放到outputbuffer中
    if(size != 0)
    {
        //放入到buffer中
        m_outputbuffer->Write(data,size);
        //没发送完成,添加epoll_write时间
        m_client->GetSelector()->Select(this,0,SEL_WRITE);
    }
}

//连接上之后删重连任务
void Connection::OnConnected()
{
    m_client->GetSelector()->Select(this,SEL_WRITE,0);
    //连接上了就删除重连务
    if(m_reconnTimer != NULL)
    {
        log::log(Info,"Destroy timer");
        m_reconnTimer->Destroy();
        m_reconnTimer = NULL;
    }
    log::log(Info,"on connected");
    //发送登陆请求
    log::log(Info,"send login request");
    LoginReq req;
    req.set_name("pengzhixiang");
    req.set_password("123456");
    SendRequest(&req,LOGIN_REQ);
}

void Connection::OnTask(Timer* timer)
{
    log::log(Info,"on task");
    if(timer == NULL)
    {
        return ;
    }
    if(timer == m_reconnTimer)
    {
        log::log(Info,"connect time out");
        //暂时先删除，如果是心跳包就不能删除了
        m_reconnTimer->Schedule(this,RECONN_INTERVAL);
    }
    else if(timer == m_pingTimer)
    {
        log::log(Info,"on ping timer");
        m_pingTimer->Schedule(this,PING_INTERVAL);
    }
}
