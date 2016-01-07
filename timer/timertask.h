/*********************************************************************
	> File Name: timertask.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Monday 21 December 2015 04:31:00 PM CST
 ********************************************************************/

#ifndef _TIMERTASK_H
#define _TIMERTASK_H
#include "task.h"
class TimerTask:public Task
{
    public:
    virtual void OnTask(Timer* timer) = 0;
    virtual ~TimerTask(){};
};
#endif //_TIMERTASK_H

