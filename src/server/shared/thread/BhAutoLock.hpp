#ifndef _BHAUTOLOCK_HPP_
#define _BHAUTOLOCK_HPP_

#include "BhMutex.hpp"
#include "../BhNonCopy.hpp"

class BhAutoLock
    :public BhNonCopy
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
    BhMutex *m_pMutex;
};

#endif
