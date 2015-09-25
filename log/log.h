/*********************************************************************
	> File Name: log.cpp
	> Author: 彭治湘
	> Mail: 345109788@qq.com 
	> Created Time: Thursday 17 September 2015 03:50:53 PM CST
 ********************************************************************/

#ifndef _LOG_H
#define _LOG_H
#include <string>
#include <fstream>
#define DEBUG
#include "assert.h"


#ifdef DEBUG
#define LOG_LINE(fmt, args...)  printf("\033[31m[%s:%d]"#fmt"\r\033[0m\n",__FILE__,  __LINE__, ##args)
#define LOG_ERR(fmt, args...)   printf("\033[46;31m[%s:%s:%d]\033[0m "#fmt" errno=%d\r\n",__FILE__, __func__, __LINE__, ##args, errno)
#define LOG_INFO(fmt, args...)  printf("\033[33m[%s:%d]\033[0m "#fmt"\r\n",__FILE__,  __LINE__, ##args)

#else

#define LOG_LINE(fmt,...)
#define LOG_ERR(fmt, ...)
#define LOG_INFO(fmt, ...)
#endif

#endif //_LOG_H
