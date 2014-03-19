#ifndef _IBHMODULE_HPP_
#define _IBHMODULE_HPP_

#include "../shared/thread/IBhThread.hpp"
#include "../shared/BhPtrDblBuf.hpp"
#include "../shared/thread/BhMutex.hpp"
#include "../msg/IBhMsg.hpp"

class IBhModule
    :public IBhThread
{
public:
    IBhModule(){}
    virtual ~IBhModule(){}
public:
    IBhMsg* GetSendMsg()
    {
        IBhMsg* pMsg = NULL;
        if (!m_sendBuf.Pop(pMsg))
        {
            m_sendMutex.Lock();
            m_sendBuf.Exchange();
            m_sendMutex.UnLock();
        }
        return pMsg;
    }
    virtual void OnMsg(IBhMsg* pMsg) = 0;
    virtual void OnMsgEnd() = 0;
    virtual void Stop() = 0;
protected:
    void SendMsg(IBhMsg* pMsg)
    {
        m_sendMutex.Lock();
        m_sendBuf.Push(pMsg);
        m_sendMutex.UnLock();
    }
protected:
    BhPtrDblBuf<IBhMsg> m_sendBuf;
    BhPtrDblBuf<IBhMsg> m_recvBuf;
    BhMutex m_sendMutex;
    BhMutex m_recvMutex;
};

#endif
