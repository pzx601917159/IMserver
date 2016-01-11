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
#include "server.h"
#include "request.h"
#include "reguserreq.pb.h"
#include "reguserres.pb.h"

//mysql
#include "mysqldatabase.h"
#include "db_pool.cpp"
#include "readconfig.h"
#include "circularbuffer.h"
#include "epollselector.h"

#include "loginreq.pb.h"
#include "loginres.pb.h"
#include "sendmsg.pb.h"

Connection::Connection(int sockfd,sockaddr_in addr,Server* server)
{
    log::log(Info,"new Connection!");
    m_sockfd = sockfd;
    m_addr = addr;
    m_inputbuffer = new CircularBuffer(4096);
    //init才分配内存
    m_inputbuffer->Init();
    m_outputbuffer = new CircularBuffer(4096);
    //init才分配内存
    m_outputbuffer->Init();
    m_server = server;
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
    RecvData();
}

//处理输出事件
void Connection::OnWrite()
{
    log::log(Info,"On write");
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
    while(m_inputbuffer->Size() > 8)
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
        uint32_t type = m_inputbuffer->ReadUint32Only(4);
        log::log(Info,"Msg type:",type);
        m_inputbuffer->SkipData(HEADER_LEN);
        HandleReq((ReqType)type,len);

    }
    log::log(Info,"ret:",ret);
    if(ret == 0)
    {
        OnClose();
    }
    return ret;
}

//发送数据
size_t Connection::SendData()
{
    size_t ret = 0;
    return ret;
}

//连接断开的处理
void Connection::OnClose()
{
    log::log(Info,"On socket close!");
    //删除server中记录的连接中的元素
    m_server->DeleteConn(m_sockfd);
    //广播离线消息，就不用广播给自己了
    //TODO
    


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
        case REGUSER_REQ:
            {
                log::log(Info,"recv register user req");
                //处理完成之后就跳过这些数据
                RegUserReq req;
                //从一个string中构造请求
                //这里先读取1024个字节吧，这个buffer还是修改了比较好
                if(len < 1032)
                {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-8);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    log::log(Info,"username:",req.name());
                    log::log(Info,"password:",req.password());
                    HandleRegReq(&req);
                }
                m_inputbuffer->SkipData(len-8);
                log::log(Info,"m_inputbuffer->Size():",m_inputbuffer->Size());
            }
            break;
        case LOGIN_REQ:
            {
                log::log(Info,"recv login msg");
                LoginReq req;
                if(len < 1032)
                {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-8);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    log::log(Info,"username:",req.name());
                    log::log(Info,"password:",req.password());
                    HandleLoginReq(&req);
                }
                
            }
            break;
        case CHAT_MSG:
            {
                log::log(Info,"recv char msg");
                SendMsg req;
                if(len < 1032)
                {
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    m_inputbuffer->Read(buffer,len-8);
                    std::string str = buffer;
                    req.ParseFromString(str);
                    HandleChatMsg(&req);
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
        char sql[1024];
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
    SendRequest(&res,REGUSER_RES);
}

//登陆请求，同一个用户不能登陆两次
void Connection::HandleLoginReq(LoginReq* req)
{
    //返回userid把
    LoginRes res;
    res.set_rescode(FAILED);

    CDatabase *pdb = CDBPool<CDatabase>::Instance()->Acquire();
    if(pdb != NULL)
    {
        char sql[1024];
        memset(sql,0,sizeof(sql));
        snprintf(sql,sizeof(sql),"select * from tab_user where username='%s'",req->name().c_str());
        if(pdb->SelectBySql(sql))
        {
            if(pdb->GetRecord())
            {
                if(strncmp(req->password().c_str(),pdb->row[2],strlen(pdb->row[2]))==0)
                {
                    log::log(Info,"login SUCCESS!!!");
                    log::log(Info,"userid:",pdb->row[0]);
                    res.set_rescode(SUCCESS);
                    //查找到了
                    res.set_userid(atoi(pdb->row[0]));
                }
            }
            //释放结果集
            pdb->FreeRecord();
        }
        CDBPool<CDatabase>::Instance()->Release(pdb);
    }
    SendRequest(&res,LOGIN_RES);

}

void Connection::HandleChatMsg(SendMsg* msg)
{
    log::log(Info,"recv msg:",msg->msg());
    //broadcast
    m_server->BroadcastMsg(msg,CHAT_MSG);
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


void Connection::SendData(const char* data,size_t size)
{
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
        m_server->GetSelector()->Select(this,0,SEL_WRITE);
    }
}
