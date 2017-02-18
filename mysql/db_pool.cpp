#include "db_pool.h"
template<class T>
CDBPool<T>* CDBPool<T>::singleDBConnectPool = NULL;
template<class T>
CDBPool<T>* CDBPool<T>::Instance()
{
    if(!singleDBConnectPool)
    {
        singleDBConnectPool = new CDBPool();
    }
    return singleDBConnectPool;
}

template<class T>
void CDBPool<T>::Destroy()
{
    if(singleDBConnectPool)
    {
        delete singleDBConnectPool;
        singleDBConnectPool = NULL;
    }
}
template<class T>
CDBPool<T>::CDBPool()
{
    Init(1);
}
template<class T>
CDBPool<T>::~CDBPool()
{
    Peng_Single_Lock::Instance()->Lock();
    while (_pool.size() > 0)
    {
    	T* cnn = _pool.back();
    	_pool.pop_back();
    	delete cnn;
    }
    Peng_Single_Lock::Instance()->Unlock();
}
template<class T>
bool CDBPool<T>::Init(int num)
{
    return Increase(num);
}
template<class T>
T* CDBPool<T>::Acquire()
{
    Peng_Single_Lock::Instance()->Lock();
    if(_pool.size() == 0)
    {
        if(Increase(1) == false)
        {
            return NULL;
        }
    }
    T* cnn = _pool.back();
    _pool.pop_back(); 
    Peng_Single_Lock::Instance()->Unlock();
    if(cnn->ReConnect())
        return cnn;
    else
        return Acquire();
}
template<class T>
void CDBPool<T>::Release(T* cnn)
{
    Peng_Single_Lock::Instance()->Lock();
    Release(cnn, cnn->Succ);
    Peng_Single_Lock::Instance()->Unlock();
}
template<class T>
bool CDBPool<T>::Increase(int num)
{
    Peng_Single_Lock::Instance()->Lock();
    int i;
    for(i=0;i<num;i++)
    {
        T *pRcDBFetch;
        pRcDBFetch = new T();
        if(pRcDBFetch->Connect() == true)
            _pool.push_back(pRcDBFetch);
        else
        {
            delete pRcDBFetch;
            return false;
        }
        pRcDBFetch = NULL;
    }
    Peng_Single_Lock::Instance()->Unlock();
    return true;
}
template<class T>
void CDBPool<T>::Release(T *cnn, bool Succ)
{
    if(Succ == true)
    {
        Peng_Single_Lock::Instance()->Lock();
        _pool.push_back(cnn);
        Peng_Single_Lock::Instance()->Unlock();
    }
    else
    {
        delete cnn;
    }
}
