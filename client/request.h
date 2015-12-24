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
    REGUSER_REQ,    //注册用户
    REGUSER_RES,
    LOGIN_REQ,      //登陆
    LOGIN_RES,
    CHAT_MSG,       //聊天
    REGSERVER_REQ,  //注册服务器
    REGSERVER_RES 
};

#define HEADER_LEN 8


#endif //_REQUEST_H
