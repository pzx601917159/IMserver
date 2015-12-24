/*********************************************************************
	> File Name: wheeltimer.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 12:23:24 PM CST
 ********************************************************************/


//这里的顺序要注意
#include "wheeltimer.h"

#include <stdio.h>
//定时器相关的头文件
#include <time.h>
#include <sys/time.h>

//信号相关的头文件
#include <signal.h>

#include "wheel.h"
#include "wheelnode.h"
#include "timer.h"
#include "task.h"

uint32_t WheelTimer::m_alarmCount = 0;
//得到过去的时间
uint32_t WheelTimer::GetAlarmCount()
{
    uint32_t time = m_alarmCount;
    m_alarmCount = 0;//重置已经过去的时间
    return time;
}

//启动计时
void WheelTimer::InitTimer()
{
    itimerval time;
    time.it_value.tv_sec = 0;
    time.it_value.tv_usec = m_unitTime * 1000;//0.1秒的定时器
    time.it_interval = time.it_value;

    signal(SIGALRM,OnAlarm);
    setitimer(ITIMER_REAL, &time, NULL);
    m_alarmCount = 0;
}

//构造
WheelTimer::WheelTimer()
{
    m_count = 10;               //轮子的数量20个  
    m_node = new WheelNode();   //新建一个槽
    m_unitTime = 100;           //0.1秒的定时器
    //启动定时器
    InitTimer();
}

//析构
WheelTimer::~WheelTimer()
{
    delete m_node;
}


//轮子运转
bool WheelTimer::Click()
{
    //获取alarm出发的次数
    int click = GetAlarmCount();
    if(click <= 0)
        return false;
    for(int time = 0; time < click; ++time)
    {
        //遍历轮子
        for(WheelList::iterator it = m_wheelList.begin(); it != m_wheelList.end(); ++it)//大部分情况为时间复杂度为o(1)
        {
            //得到这个轮子的单元时间
            uint32_t unitTime = (*it)->GetUnitTime();
            //得到时间槽,并用空的槽替换原来的槽,就是相当于移动一个槽
            m_node = (*it)->Click(m_node);
           
            while(m_node->Begin() != NULL)
            {
                //得到时间槽中的定时器
                Timer* timer = m_node->Pop();
                timer->SetRemain(timer->GetRemain() % unitTime);
                //如果剩余的时间小于遍历一次的时间
                if(timer->GetRemain() < m_unitTime)
                {
                    timer->OnTimer();
                }
                else
                {
                    InsertTimer(timer);
                }
            } 
        
            m_node->Reset();//重置这个node；
            if((*it)->GetPassedTime() != 0)
            {
                break;
            }
            
        }
    }
}

//对定时器出发的次数+1
void WheelTimer::OnAlarm(int)
{
    m_alarmCount += 1;
}


//时间轮中提供创建定时器的函数
Timer* WheelTimer::CreateTimer(Task* task,uint32_t time)
{
    return  new Timer(task,time);
}

//插入任务
Timer* WheelTimer::InsertTask(Task* task,uint32_t time)
{
    Timer* timer = CreateTimer(task,time);
    InsertTimer(timer);
    return timer;
}

//插入定时器
void WheelTimer::InsertTimer(Timer* timer)
{
    if(timer == NULL)
    {
        return ;
    }
    //单位时间
    uint32_t time = m_unitTime;
    //第一次插入的时候还没有创建轮子
    for(WheelList::iterator it = m_wheelList.begin(); it != m_wheelList.end(); it++)//大部分情况为时间复杂度o(1)
    {
        //获取剩余时间
        uint32_t remain = timer->GetRemain();
        //获取总时间
        time = (*it)->GetTotalTime();
        //剩余的时间小于轮子的从时间
        if(remain < time)
        {
            //插入定时器
            (*it)->InsertTimer(remain/(*it)->GetUnitTime(),timer);//o(1)的时间复杂度
            return;
        }
        else
        {
            //设置剩余时间加上已经过去的时间
            timer->SetRemain(timer->GetRemain() + (*it)->GetPassedTime());
        }
    }
    while(true)
    {
        //一个轮子的时间跨度不够了，新建轮子
        Wheel *wheel = new Wheel(m_count,time); //新建的轮子的时间跨度为上轮子的一个周期
        time = wheel->GetTotalTime();           //更新一下总时间，如果还需要新建轮子用
        //插入到轮子链表中
        m_wheelList.push_back(wheel);
        //获取timer的剩余时间
        if(timer->GetRemain() < wheel->GetTotalTime())
        {
            //插入吧
            wheel->InsertTimer(timer->GetRemain()/wheel->GetUnitTime(),timer);
            return;
        }
        else
        {
            //时间不够啊，还得继续
            timer->SetRemain(timer->GetRemain() + wheel->GetPassedTime());
        }
    }
}
