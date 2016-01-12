/*********************************************************************
	> File Name: request.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 16 December 2015 10:36:12 AM CST
 ********************************************************************/
#ifndef _REQUEST_H
#define _REQUEST_H

//错误码可以进一步细化
enum ResCode
{
    SUCCESS = 200,  //成功
    FAILED = 400,   //失败
    INNER_ERR = 500 //内部错误
};

enum ReqType
{
    REGUSER_REQ,    //注册用户请求
    REGUSER_RES,    //注册用户响应
    LOGIN_REQ,      //登陆请求
    LOGIN_RES,      //登陆响应
    CHAT_MSG,       //聊天消息(后面添加屏蔽字)
    ONLINE_MSG,     //用户上线消息
    OFFLINE_MSG     //用户下线消息
};


#endif //_REQUEST_H
