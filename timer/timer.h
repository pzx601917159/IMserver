/*********************************************************************
	> File Name: timer.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 09 December 2015 05:36:30 PM CST
 ********************************************************************/

#ifndef _TIMER_H
#define _TIMER_H

#include <inttypes.h>
#include <stdlib.h>

class WheelNode;
class Task;

//时间轮定时器
class Timer
{
    public:
    Timer(Task* task = NULL,uint32_t remain = 1000);//默认1s的定时器
    ~Timer();

    Timer* GetPrev();
    Timer* GetNext();

    void SetPrev(Timer* prev);
    void SetNext(Timer* next);

    void SetTask(Task* task);
    Task* GetTask();

    void OnTimer();

    uint32_t GetRemain() const;
    void SetRemain(uint32_t remain);
    void SetNode(WheelNode* node);
    WheelNode* GetNode();

    bool Schedule(Task* task,uint32_t minsec);
    void Destroy();

    private:
    Task* m_task;  //定时任务
    WheelNode* m_node;  //轮子的槽
    uint32_t m_remain;  //剩余的时间
    Timer* m_prev; //前一个定时器
    Timer* m_next; //后一个定时器

};

#endif //_TIMER_H
