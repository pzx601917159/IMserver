/*********************************************************************
	> File Name: wheeltimer.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 12:23:24 PM CST
 ********************************************************************/

//时间轮定时器
#ifndef _WHEELTIMER_H
#define _WHEELTIMER_H
#include <inttypes.h>
#include <list>

class Timer;
class Task;
class Wheel;
class WheelNode;

class WheelTimer
{
    public:
    //构造
    WheelTimer();
    //析构
    ~WheelTimer();
    void InsertTimer(Timer* timer);
    Timer* InsertTask(Task* task,uint32_t time);
    bool Click();
    //回调函数的固定格式
    static void OnAlarm(int);
    Timer* CreateTimer(Task* task,uint32_t time);

    private:
    void InitTimer();
    //这里只能是静态函数，不能是成员函数
    static uint32_t GetAlarmCount();

    typedef std::list<Wheel*> WheelList;
    WheelList m_wheelList;  //时间轮链表
    uint32_t m_unitTime;    //单元时间
    uint32_t m_count;       //时间轮的数量
    WheelNode* m_node;      //当前的时间槽
    static uint32_t m_alarmCount;//alarm信号出发了几次
};
#endif //_WHEELTIMER_H

