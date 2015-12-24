#include "mutex.h"
#include <assert.h>
#include <errno.h>

namespace log{
//namespace_begin

mutex::mutex()
{
    int res = 0;
    res = pthread_mutex_init(&m_mutex, 0);
    if (res != 0) {
        
	}
}

mutex::~mutex()
{
    int res = 0;
    res = pthread_mutex_destroy(&m_mutex);
    if (res == EPERM) {
	}
    assert(res == 0);
}

void mutex::lock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mutex);
    if (res == EDEADLK) {
		
	}
    assert(res == 0);
}

void mutex::unlock()
{
    int res = 0;
    res = pthread_mutex_unlock(&m_mutex);
    if (res == EPERM) {

	}
    assert(res == 0);
}

scope_lock::scope_lock(mutex& mx, bool initially_locked):m_mutex(mx), m_locked(false)
{
	if (initially_locked) lock();
}

scope_lock::~scope_lock()
{
	if (m_locked) unlock();
}

void scope_lock::lock()
{
	if (m_locked) return;
	m_mutex.lock();
	m_locked = true;
}

void scope_lock::unlock()
{
	if (!m_locked) return;
	m_mutex.unlock();
	m_locked = false;
}

//namespace_end
}
