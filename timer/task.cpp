/*********************************************************************
	> File Name: Task.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 10 December 2015 05:04:12 PM CST
 ********************************************************************/

#include "task.h"
#include <stdio.h>
#include <time.h>

void Task::OnTask()
{
    printf("==================On Task!\n");
    time_t t = time(NULL);
    printf("shijian: %s\n",ctime(&t));
}
