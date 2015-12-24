/*********************************************************************
	> File Name: wheel.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 12:25:10 PM CST
 ********************************************************************/

#ifndef _WHEEL_H
#define _WHEEL_H
#include <vector>
#include <inttypes.h>
class WheelNode;
class Timer;
class Wheel
{
    public:
    Wheel(uint32_t size,uint32_t time);
    ~Wheel();
    //获取单元时间
    uint32_t GetUnitTime() const;
    //获取从时间
    uint32_t GetTotalTime() const;
    //获取剩余时间
    uint32_t GetRemainTime() const;
    //获取过去的时间
    uint32_t GetPassedTime() const;
    //插入定时器
    void InsertTimer(uint32_t pos,Timer* timer);
    //切换槽
    WheelNode* Click(WheelNode* node);

    private:
    typedef std::vector<WheelNode*> WheelNodeArray;
    WheelNodeArray m_nodes;
    uint32_t m_passed;    //当前过去单元数量
    uint32_t m_unitTime;  //一个单元时间
};
#endif //_WHEEL_H
