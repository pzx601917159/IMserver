#ifndef EIMD_LOGGER_H
#define EIMD_LOGGER_H

#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <string>
#include <fstream>
#include "tostring.h"
#include "loghelper.h"
#include <syslog.h>


// 历史原因，把这个放在名字空间外面，否则，要改的地方很多。

class Global
{
    public:
    static time_t now;
};

enum
{
	Debug = 0,
	Info  = 1,
	Warn  = 2,
	Error = 3,
	Godx  = 4,
};

enum 
{
	Syslog = 0,
	Standard = 1,
	SFile = 2
};

namespace log
{

extern int syslog_level;

extern int stringToLevel(const char * name);
extern const char * levelToString(int l);

#define LevelToName(l) (l)

#define SOXSYSLOG(a, b) linux_log_mt(a, b);

bool setlog(const char* file,unsigned int sizek, bool mt = false);
void linux_log_mt(int a, const std::string &b);
void linux_log(int a, const std::string &b);
void linux_update_timestr();

inline std::string NowStr() { time_t t = time(0); return FormatTime(t); }
inline void setSysLogLevel(const char * level) { syslog_level = stringToLevel(level); }


template < class T1 >
inline void log(int l, const T1 & t1)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1).c_str());
}

template < class T1, class T2 >
inline void log(int l, const T1 & t1, const T2 & t2)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2).c_str());
}

template < class T1, class T2, class T3 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3).c_str());
}

template < class T1, class T2, class T3, class T4 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4)
{

	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4).c_str());
}

template < class T1, class T2, class T3, class T4, class T5>
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5).c_str());
}

template < class T1, class T2, class T3, class T4, class T5, class T6 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5, const T6 & t6)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6).c_str());
}

template < class T1, class T2, class T3, class T4, class T5, class T6,
	class T7 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6, t7).c_str());
}

template < class T1, class T2, class T3, class T4, class T5, class T6,
	class T7, class T8 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7,
	const T8 & t8)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6, t7, t8).c_str());
}

template < class T1, class T2, class T3, class T4, class T5, class T6,
	class T7, class T8, class T9 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7,
	const T8 & t8, const T9 & t9)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6, t7, t8, t9).c_str());
}

template < class T1, class T2, class T3, class T4, class T5, class T6,
	class T7, class T8, class T9,  class T10 >
inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
	const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7,
	const T8 & t8, const T9 & t9, const T10 & t10)
{
	if (syslog_level <= l)
		SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10).c_str());
}
template < class T1, class T2, class T3, class T4, class T5, class T6,
class T7, class T8, class T9,  class T10 , class T11>
		inline void log(int l, const T1 & t1, const T2 & t2, const T3 & t3,
		const T4 & t4, const T5 & t5, const T6 & t6, const T7 & t7,
		const T8 & t8, const T9 & t9, const T10 & t10,  const T11 & t11)
{
		if (syslog_level <= l)
				SOXSYSLOG(LevelToName(l), tostring(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11).c_str());
}

extern int levelToSysLevel(int l);
extern void rslog(int l, const char* format, ...);
extern void setLogStyle(int style);

} // namespace log

/////////////////////////////////////////////////////////////
// L:	loglevel
// S:	log statement: << t1 << t2

#define _SRC_POS_  __FILE__<<":"<<__LINE__<<":"

#define SYS_LOG(L, S) \
{ \
	if (log::syslog_level <= L) { \
		std::ostringstream ss; ss << _SRC_POS_ S; \
		syslog(LOG_DEBUG, "%s%s", LevelToName(L), ss.str().c_str()); \
	} \
}

#endif // EIMD_LOGGER_H
