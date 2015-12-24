#ifndef _DB_MASTER_POOL_H
#define _DB_MASTER_POOL_H
#include "vostypes.h"
class Peng_Lock
{
public:
	Peng_Lock()
	{
		pthread_mutex_init(&mlock,NULL);
		pthread_mutex_lock(&mlock);
	}

	~Peng_Lock()
	{
		pthread_mutex_unlock(&mlock);
		pthread_mutex_destroy(&mlock);
	}
private:
	static pthread_mutex_t mlock;
};
class Peng_Recursive_Lock
{
public:
	Peng_Recursive_Lock()
	{
		pthread_mutexattr_init(&mattr);
		pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&mlock,&mattr);
		pthread_mutex_lock(&mlock);
	}
	~Peng_Recursive_Lock()
	{
		pthread_mutex_unlock(&mlock);
		pthread_mutex_destroy(&mlock);
        pthread_mutexattr_destroy(&mattr);
	}
private:
	pthread_mutex_t mlock;
	pthread_mutexattr_t mattr;
};
class Peng_Single_Lock
{
public:
    void Lock()
    {
        pthread_mutex_lock(&m_lock);
    }
    void Unlock()
    {
        pthread_mutex_unlock(&m_lock);
    }
    static Peng_Single_Lock* Instance()
    {
        static Peng_Single_Lock lock;
        return &lock;
    }
private:
    Peng_Single_Lock()
    {
		pthread_mutexattr_init(&m_attr);
		pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_lock,&m_attr);
    }
    ~Peng_Single_Lock()
    {
        pthread_mutex_destroy(&m_lock);
        pthread_mutexattr_destroy(&m_attr);
    }
    pthread_mutex_t m_lock;
    pthread_mutexattr_t m_attr;
};
template <class T>
class CDBPool
{
    std::vector<T*> _pool;

    static CDBPool* singleDBConnectPool;

public:
    static CDBPool* Instance();
    static void Destroy();

public:
	void Release(T* cnn,bool Succ);
    CDBPool();
    virtual ~CDBPool();

    bool Init(int num);
    T* Acquire();
    void Release(T* cnn);
protected:
    bool Increase(int num);
};

template <class T>
class CMasterDBGuard
{
	T* _cnn;
public:
	CMasterDBGuard(T* cnn) : _cnn(cnn)
	{}

	~CMasterDBGuard()
	{
		if(_cnn)
			CDBPool<T>::Instance()->Release(_cnn,_cnn->Succ);
	}
};
#endif

