#ifndef _IBHPASSIVEMODULE_HPP_
#define _IBHPASSIVEMODULE_HPP_

#include "IBhModule.hpp"
#include "../shared/thread/BhCond.hpp"

class IBhPassiveModule
    :public IBhModule
{
public:
    IBhPassiveModule(){}
    virtual ~IBhPassiveModule(){}
public:
    virtual void OnMsg(IBhMsg* pMsg)
    {
        m_recvBuf.Push(pMsg);
    }
    virtual void OnMsgEnd()
    {
        m_recvMutex.Lock();
        m_recvCond.Notify();
        m_recvMutex.UnLock();
    }
    virtual void Stop()
    {
        m_bWantRun = false;
        m_recvCond.Notify();
    }
protected:
    virtual void Run()
    {
        m_bWantRun = true;
        IBhMsg* pMsg = NULL;
        while (m_bWantRun)
        {
            m_recvMutex.Lock();
            while (m_bWantRun && 0 == m_recvBuf.GetPushSize())
            {
                m_recvCond.Wait(m_recvMutex);
            }
            if (m_recvBuf.GetPushSize() > 0)
            {
                m_recvBuf.Exchange();
            }
            m_recvMutex.UnLock();
            
            while (m_bWantRun && m_recvBuf.Pop(pMsg))
            {
                OnHandleMsg(pMsg);
            }
        }
    }
    virtual void OnHandleMsg(IBhMsg* pMsg) = 0;
protected:
    BhCond m_recvCond;
    volatile bool m_bWantRun;
};

#endif