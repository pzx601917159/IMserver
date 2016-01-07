/*********************************************************************
	> File Name: connection.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 26 August 2015 04:17:31 PM CST
 ********************************************************************/
#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string>

#include "request.h"
#include "reguserreq.h"
#include "task.h"
#include "timertask.h"

class LoginReq;
class LoginRes;
class SendMsg;

class CircularBuffer;
class Client;

#define PING_INTERVAL 20000
#define RECONN_INTERVAL 5000

//接口类
class IConnection
{
    public:
    virtual void OnRead() = 0;
    virtual void OnWrite() = 0;
    virtual int GetSockfd() const = 0;
    virtual ~IConnection(){};
};

class Connection:public IConnection,public TimerTask
{
    public:
   
    enum ConnType
    {
        ACCEPT,//接受的连接
        CONNECT//连接别人的
    };

    ConnType cType;

    Connection(int sockfd,sockaddr_in addr,Client* client);
    ~Connection();

    //定时任务
    void  OnTask(Timer* timer);

    //处理输入事件
    void OnRead();
    //处理输出事件
    void OnWrite();

    //发送数据
    void SendData(const char* data,size_t size);

    //设置连接的超时
    void SetTimeOut(int);

    //超时的处理
    void OnTimeOut();

    //关闭连接
    void OnClose();

    //接收数据
    size_t RecvData();

    //发送数据
    size_t SendData();

    int GetSockfd() const;

    //处理请求
    void HandleReq(ReqType reqtype,size_t reqlen);

    //注册用户
    void HandleRegReq(RegUserReq* req);
    //用户登陆
    void HandleLoginRes(LoginRes* res);
    //聊天
    void HandleChatMsg(SendMsg* msg);

    //发送请求
    void SendRequest(::google::protobuf::Message* req,ReqType type);

    //连接上之后的处理
    void OnConnected();

    //检查连接
    bool CheckConnect(Connection* conn);

    //设置重连的定时器
    void SetReconnTimer(Timer* timer){m_reconnTimer = timer;}
    //设置心跳包的定时器
    void SetPingTimer(Timer* timer){m_pingTimer = timer;}


    private:
    int m_sockfd;//套接字
    struct sockaddr_in m_addr;//连接的地址信息(IP地址和端口号)
    std::string m_name;//记录聊天的人的名字
    //接收缓冲区
    CircularBuffer* m_inputbuffer;
    //发送缓冲区
    CircularBuffer* m_outputbuffer;
    //对应的server
    Client* m_client;
    //判断连接是否可用
    bool m_enable;
    Timer* m_reconnTimer;   //定时器
    Timer* m_pingTimer;     //心跳包定时器
    int m_userid;

};

#endif //_CONNECTION_H
