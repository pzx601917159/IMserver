/*********************************************************************
	> File Name: epollselector.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 17 December 2015 02:44:14 PM CST
 ********************************************************************/
#ifndef _EPOLLSELECTOR_H
#define _EPOLLSELECTOR_H

#include <set>

class IConnection;
class Server;

enum
{
    SEL_READ = 1,
    SEL_WRITE,
    SEL_RW
};

class EPollSelector
{
    public:
    EPollSelector();
    virtual ~EPollSelector();
    bool Init();
    void Select(IConnection* conn,int remove,int add);
    void Select();
    void Remove(IConnection *conn);
    size_t Size() const;

    private:
    typedef std::set<int> SockFdSet;
    SockFdSet m_sockfds;
    int m_epollfd;
};

#endif //_EPOLLSELECTOR_H

