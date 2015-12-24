/*********************************************************************
	> File Name: wheelnode.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 11:52:56 AM CST
 ********************************************************************/

#include "wheelnode.h"
#include "timer.h"

WheelNode::WheelNode()
{
    m_timer = NULL;
}

WheelNode::~WheelNode()
{
}

//插入定时器
void WheelNode::InsertTimer(Timer* timer)
{
    //设置属于这个槽
    timer->SetNode(this);
    //第一次插入
    if(m_timer == NULL)
    {
        timer->SetPrev(NULL);
        timer->SetNext(NULL);
        m_timer = timer;
    }
    //直接插入到头部
    else
    {
        timer->SetPrev(NULL);
        timer->SetNext(m_timer);
        m_timer->SetPrev(timer);
        m_timer = timer; 
    }
}

//删除槽中的定时器
void WheelNode::DeleteTimer(Timer* timer)
{
    if(m_timer = timer)
    {
        m_timer = m_timer->GetNext();
        if(m_timer != NULL)
        {
            m_timer->SetPrev(NULL);
        }
    }
    else
    {
        if(timer->GetPrev() != NULL)
        {
            timer->GetPrev()->SetNext(timer->GetNext());
        }
        if(timer->GetNext() != NULL)
        {
            timer->GetNext()->SetPrev(timer->GetPrev());
        }
    }
}

Timer* WheelNode::Begin()
{
    return m_timer;
}

Timer* WheelNode::Pop()
{
    Timer* timer = m_timer;
    if(m_timer != NULL)
    {
        m_timer = m_timer->GetNext();
        if(m_timer != NULL)
        {
            m_timer->SetPrev(NULL);
        }
    }
    timer->SetPrev(NULL);
    timer->SetNext(NULL);
}

void WheelNode::Reset()
{
    m_timer = NULL;
}
