#ifndef _IBHACTIVEMODULE_HPP_
#define _IBHACTIVEMODULE_HPP_

#include "IBhModule.hpp"

class IBhActiveModule
    :public IBhModule
{
public:
    IBhActiveModule(){}
    virtual ~IBhActiveModule(){}
public:
    virtual void OnMsg(IBhMsg* pMsg)
    {
        m_recvMutex.Lock();
        m_recvBuf.Push(pMsg);
        m_recvMutex.UnLock();
    }
    virtual void OnMsgEnd(){}
};

#endif
