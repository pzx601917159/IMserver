/*********************************************************************
	> File Name: wheelnode.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 11:52:56 AM CST
 ********************************************************************/

#ifndef _WHEELNODE_H
#define _WHEELNODE_H
//时间轮槽
class Timer;
class WheelNode
{
    public:
    WheelNode();
    ~WheelNode();
    //插入定时器
    void InsertTimer(Timer* timer);
    void DeleteTimer(Timer* timer);
    Timer* Begin();
    Timer* Pop();
    void Reset();
    private:
    //存储定时器链表的头节点
    Timer *m_timer;
};
#endif //_WHEELNODE_H
