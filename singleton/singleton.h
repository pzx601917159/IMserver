/*********************************************************************
	> File Name: singleton.h
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Wednesday 09 September 2015 02:37:44 PM CST
 ********************************************************************/

#ifndef _SIGLETION_H
#define _SIGLETION_H
template <typename T>
T* Singleton()
{
    static T instance;
    return &instance;
}
#endif //_SIGLETION_H
