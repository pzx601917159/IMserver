
#ifndef _TOSTRING_H
#define _TOSTRING_H

#include <sstream>
#include <string>

namespace log
{
template < class T1 >
inline std::string tostring(const T1 & t1)
{
	std::ostringstream ss;
	ss << t1;
	return ss.str();
}

template < class T1, class T2 >
inline std::string tostring(const T1 & t1, const T2 & t2)
{
	std::ostringstream ss;
	ss << t1 << t2;
	return ss.str();
}

template < class T1, class T2, class T3 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3;
	return ss.str();
}

template < class T1, class T2, class T3, class T4 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5, const T6 & t6)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5 << t6;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5, const T6 & t6, const T7 & t7)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5 << t6 << t7;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7
	, class T8 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7
	, class T8, class T9 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7
	, class T8, class T9, class T10 >
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
	const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9,
	const T10 & t10)
{
	std::ostringstream ss;
	ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10;
	return ss.str();
}

template < class T1, class T2, class T3, class T4, class T5, class T6, class T7
, class T8, class T9, class T10 , class T11>
inline std::string tostring(const T1 & t1, const T2 & t2, const T3 & t3, const T4 & t4,
							const T5 & t5, const T6 & t6, const T7 & t7, const T8 & t8, const T9 & t9,
							const T10 & t10, const T11& t11)
{
		std::ostringstream ss;
		ss << t1 << t2 << t3 << t4 << t5 << t6 << t7 << t8 << t9 << t10 << t11;
		return ss.str();
}

} // namespace log

#endif //_TOSTRING_H
