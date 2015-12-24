
#include "logger.h"
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string>

#include "mutex.h"

#define LINE_MAX_LOG 1024
#define MAX_LOG_SIZE	(1024*1024*1024)

#define memzero(a)  memset(&a, 0 , sizeof(a))

time_t Global::now;

namespace log {

int syslog_level = Debug;

std::string logfile;
unsigned int maxsize;
std::ofstream fs;
bool mutlithread;
char g_timeStr[256];	
char g_format[2048];	
static log::mutex logmutex; // mutex for timeStr and log
static int g_lastupdatetime;
static int g_style = SFile;

int levelToSysLevel(int l) {
	switch(l) {
		case Debug:
			return LOG_DEBUG;
		case Info:
			return LOG_INFO;
		case Warn:
			return LOG_WARNING;
		case Error:
			return LOG_ERR;
		default:
			return LOG_NOTICE;
	}
}



const char * levelnames[Godx+1] = { "debug - ", "info  - ", "WARN  - ", "ERROR - ",
		"      - " };

int stringToLevel(const char * name) {
		if (log::strcasecmp(name, "debug") == 0)
					return Debug;
			else if (log::strcasecmp(name, "info") == 0)
						return Info;
				else if (log::strcasecmp(name, "warn") == 0)
							return Warn;
					else
								return Error;
}

const char * levelToString(int l) {
		return levelnames[l];
}


void recursive_mkdir(const char * path, size_t len) {
	if (len == 0)
		return;

	std::string s;
	if (path[0] == '/') {
		s.append(1, '/');
		--len;
		++path;
	}
	size_t n;
	while ((n = log::find(path, len, '/')) > 0) {
		s.append(path, n);
		path += n;
		len -= n;
		// ignore any error
::		mkdir(s.c_str(), S_IRWXU); // errno != EEXIST
	}
}

std::string _getTime()
{
	time_t tt = time(NULL);
	tm *t = localtime(&tt);
	char tm_buf[255] = {0};
	sprintf(tm_buf, "%.4d-%.2d-%.2d_%.2d:%.2d:%.2d",
			t->tm_year+1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec);
	return std::string(tm_buf);
}

bool setlog(const char* file,unsigned int sizek, bool mthread)
{
	maxsize=std::min( (unsigned int)(sizek*1024), (unsigned int)MAX_LOG_SIZE );
	logfile=file;
	fs.open(logfile.c_str(),std::ios::app);
	if(!fs.is_open()) return false;
	fs.seekp(0,std::ios::end);
	mutlithread = mthread;
	return true;
}

void _linux_update_timestr() {
    Global::now = time(NULL);
    if(g_lastupdatetime != Global::now){
		memzero(g_timeStr);
        char* timestr = ctime(&Global::now);
		if(timestr){
			char * p =  (char *)memccpy(g_timeStr, timestr, 0, 255);
			if(p) p[-2] = '\0'; // remove "\r"
		}
		g_lastupdatetime = Global::now;
		return;
	}
}

void linux_update_timestr()
{
	if (mutlithread) {
		log::scope_lock xx(logmutex);	
		_linux_update_timestr();
	} else {
		_linux_update_timestr();
	}
	
}

void linux_log_mt(int level, const std::string &b)
{
	if (g_style == Syslog) {
		syslog(levelToSysLevel(level), "%s", b.c_str());
		return;
	}
	if (b.size() > LINE_MAX_LOG)
		return;
	if (mutlithread) {
		static log::mutex logmutex;
		log::scope_lock xx(logmutex);	
		linux_log(level, b);
	} else {
		linux_log(level, b);
	}
}

void linux_log(int l, const std::string &b)
{
    linux_update_timestr();
    //标准输出
	if(g_style == Standard || !fs.is_open()) {
		std::cout << levelToString(l) << g_timeStr << b << std::endl;
		std::cout.flush();
		return;
	}
	unsigned int cursize = fs.tellp();
    //如果文件大小超限了就新建文件
    if(cursize>=maxsize) {
		std::string strNew=_getTime();
		strNew+=".log";
		fs.close();
		rename(logfile.c_str(),strNew.c_str());
		fs.open(logfile.c_str());
		if(!fs.is_open()) return;
	}   
	fs << g_timeStr << ":" << levelToString(l) << b << std::endl;
    std::cout << g_timeStr << ":" << levelToString(l) << b << std::endl;
    
	//fs.flush();
}

void rslog(int l, const char* format, ...) {
	if(g_style == Standard) {
		std::string sFormat; 	
		const char* level = levelToString(l);
		if (level)
			sFormat.assign(level);
		sFormat += " ";
		sFormat += format;
		sFormat += "\r\n";
		va_list list;
		va_start(list, format);
		vprintf(sFormat.c_str(), list);
		va_end(list);

	}
	if (syslog_level <= l) {
		va_list list;
		va_start(list, format);
		vsyslog(levelToSysLevel(l), format, list);
		va_end(list);
	}
}

void setLogStyle(int style) {
	g_style = style;
}
} // namespace log
