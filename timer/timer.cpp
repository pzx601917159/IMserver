/*********************************************************************
	> File Name: timer.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 09 December 2015 05:36:30 PM CST
 ********************************************************************/

#include "timer.h"
#include "task.h"
#include "wheelnode.h"
#include "singleton.h"
#include "wheeltimer.h"

//构造
Timer::Timer(Task* task,uint32_t remain)
{
    m_task = task;
    m_node = NULL;
    m_remain = remain;
    m_prev = NULL;
    m_next = NULL;
}
 
//析构
Timer::~Timer()
{
    if(m_node != NULL)
    {
        //从槽中删除这个定时器
        m_node->DeleteTimer(this);
    }
}

void Timer::SetTask(Task* task)
{
    m_task = task;
}

Task* Timer::GetTask()
{
    return m_task;
}

//得到上一个定时器
Timer* Timer::GetPrev()
{
    return m_prev;
}

//得到下一个定时器
Timer* Timer::GetNext()
{
    return m_next;
}

//设置上一个定时器
void Timer::SetPrev(Timer* prev)
{
    m_prev = prev;
}

//设置下一个定时器
void Timer::SetNext(Timer* next)
{
    m_next = next;
}

//定时器出发调用的函数
void Timer::OnTimer()
{
    if(m_task != NULL)
    {
        m_task->OnTask(this);
    }
}

//获取定时器的剩余时间
uint32_t Timer::GetRemain() const
{
    return m_remain;
}

//设置定时器的剩余时间
void Timer::SetRemain(uint32_t remain)
{
    m_remain = remain;
}

//设置槽,不能简单的进行设置，要根据顺序进行插入,暂时不做处理把
void Timer::SetNode(WheelNode* node)
{
    m_node = node;
}

//获取槽
WheelNode* Timer::GetNode()
{
    return m_node;
}

//插入定时器任务到时间轮中
bool Timer::Schedule(Task* task,uint32_t minsec)
{
    m_task = task;
    m_remain = minsec;
    Singleton<WheelTimer>()->InsertTimer(this);
    return true;
}

//销毁定时器
void Timer::Destroy()
{
    delete this;
}

