/*********************************************************************
	> File Name: Iconn.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Monday 18 January 2016 03:59:07 PM CST
 ********************************************************************/

//接口类
class IConnection
{
    public:
    virtual void OnRead() = 0;
    virtual void OnWrite() = 0;
    virtual int GetSockfd() const = 0;
    virtual ~IConnection(){};
};
