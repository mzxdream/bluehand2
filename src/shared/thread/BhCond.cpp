#include "BhCond.hpp"

BhCond::BhCond()
{
    Init();
}
~BhCond::BhCond()
{
    Clear();
}
int BhCond::Init()
{
    if (m_pCond)
    {
        m_pCond = new pthread_cond_t;
        if (m_pCond)
        {
            return ::pthread_cond_init(m_pCond, NULL);
        }
    }
    return -1;
}
void BhCond::Clear()
{
    if (m_pCond)
    {
        ::pthread_cond_destroy(m_pCond);
        delete m_pCond;
        m_pCond = NULL;
    }
}
int BhCond::Wait(const BhMutex& mtx)
{
    if (m_pCond && mtx.GetMutex())
    {
        return ::pthread_cond_wait(m_pCond, mtx.GetHandle());
    }
    return -1;
}
int BhCond::Notify()
{
    if (m_pCond)
    {
        return ::pthread_cond_signal(m_pCond);
    }
    return -1;
}
int BhCond::NotifyAll()
{
    if (m_pCond)
    {
        return ::pthread_cond_broadcast(m_pCond);
    }
    return -1;
}
