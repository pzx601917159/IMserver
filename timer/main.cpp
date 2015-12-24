/*********************************************************************
	> File Name: main.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 09 December 2015 05:35:48 PM CST
 ********************************************************************/

#include "wheeltimer.h"
#include "timer.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    //创建时间轮定时器
    WheelTimer wheeltimer;

    //创建定时器，传入参数为时间
    Task task;
    printf("------\n");
    Timer* timer = wheeltimer.CreateTimer(&task,1000);
    Timer* timer2 = wheeltimer.CreateTimer(&task,10000);

    printf("------\n");
    //插入定时器
    wheeltimer.InsertTimer(timer);
    wheeltimer.InsertTimer(timer2);

    while(1)
    {
        //检测定时器信号
        wheeltimer.Click();
    }
    
    return 0;
}
