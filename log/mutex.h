#pragma once
#include <unistd.h>
#include <pthread.h>

namespace log{
//namespace_begin

class mutex
{
public:
	mutex();
	virtual ~mutex();

	void lock();
	void unlock();

private:
	pthread_mutex_t* pmutex;
	pthread_mutex_t m_mutex;
};

class scope_lock
{
public:
	explicit scope_lock(mutex& mx, bool initially_locked=true);
	virtual ~scope_lock();

	void lock();
	void unlock();

private:
    mutex& m_mutex;
    bool m_locked;
};

//namespace_end
}
