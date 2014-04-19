#include "BhMutex.hpp"

BhMutex::BhMutex()
{
    Init();
}
BhMutex::~BhMutex()
{
    Clear();
}
int BhMutex::Init()
{
    if (!m_pMutex)
    {
        m_pMutex = new pthread_mutex_t;
        if (m_pMutex)
        {
            return ::pthread_mutex_init(m_pMutex, NULL);
        }
    }
    return -1;
}
void BhMutex::Clear()
{
    if (m_pMutex)
    {
        ::pthread_mutex_destroy(m_pMutex);
        delete m_pMutex;
        m_pMutex = NULL;
    }
}
int BhMutex::Lock()
{
    if (m_pMutex)
    {
        return ::pthread_mutex_lock(m_pMutex);
    }
    return -1;
}
int BhMutex::UnLock()
{
    if (m_pMutex)
    {
        return ::pthread_mutex_unlock(m_pMutex);
    }
    return -1;
}
int BhMutex::TryLock()
{
    if (m_pMutex)
    {
        return ::pthread_mutex_trylock(m_pMutex);
    }
    return -1;
}
const pthread_mutex_t* BhMutex::GetHandle() const
{
    return m_pMutex;
}
