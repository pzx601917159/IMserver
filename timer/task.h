/*********************************************************************
	> File Name: Task.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 05:04:12 PM CST
 ********************************************************************/

#ifndef _TASK_H
#define _TASK_H
class Timer;
class Task
{
    public:
    virtual void OnTask(Timer*) = 0;
    virtual ~Task(){};
};
#endif //_TASK_H
