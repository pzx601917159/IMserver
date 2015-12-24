/*********************************************************************
	> File Name: wheel.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 12:25:10 PM CST
 ********************************************************************/
#include "wheel.h"
#include "wheelnode.h"
#include <stdio.h>

//构造,参数1为wheelnode的个数，参数2为时间间隔
Wheel::Wheel(uint32_t size,uint32_t time)
{
    m_unitTime = time;
    m_nodes.resize(size);
    for(int i = 0; i < size; ++i)
    {
        m_nodes[i] = new WheelNode();//新建size个槽
    }
    m_passed = 0;
}

//析构
Wheel::~Wheel()
{
}

//获取单元时间
uint32_t Wheel::GetUnitTime() const
{
    return m_unitTime;
}

//获取从时间
uint32_t Wheel::GetTotalTime() const
{
    return m_unitTime * m_nodes.size();
}

//获取剩余时间
uint32_t Wheel::GetRemainTime() const
{
    return m_unitTime * (m_nodes.size() - m_passed);
}

//获取过去的时间,轮子运转过去的时间,小于轮子的周期
uint32_t Wheel::GetPassedTime() const
{
    return m_unitTime * m_passed;
}

void Wheel::InsertTimer(uint32_t pos,Timer* timer)
{
    //不能插入到当前的槽中
    if(pos % m_nodes.size() == 0)
    {
        pos = 1;
    }
    //看时间相当于多少个槽，插入合适的槽中
    uint32_t newpos = (m_passed + pos) % m_nodes.size();
    m_nodes[newpos]->InsertTimer(timer);
}

//切换槽
WheelNode* Wheel::Click(WheelNode* newNode)
{
    //过去的时间+1
    m_passed = (m_passed + 1) % m_nodes.size();
    WheelNode *node = m_nodes[m_passed];
    m_nodes[m_passed] = newNode;
    return node;
}

