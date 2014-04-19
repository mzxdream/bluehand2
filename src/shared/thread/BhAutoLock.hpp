#ifndef _BHAUTOLOCK_HPP_
#define _BHAUTOLOCK_HPP_

#include <thread/BhMutex.hpp>

class BhAutoLock
{
public:
    explicit BhAutoLock(BhMutex* pMutex)
        :m_pMutex(pMutex)
    {
        if (m_pMutex)
        {
            m_pMutex->Lock();
        }
    }
    ~BhAutoLock()
    {
        if (m_pMutex)
        {
            m_pMutex->UnLock();
        }
    }
private:
    BhAutoLock(const BhAutoLock&);
    BhAutoLock& operator=(const BhAutoLock&);
private:
    BhMutex *m_pMutex;
};

#endif
