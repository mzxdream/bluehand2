#ifndef _IBHPASSIVEMODULE_HPP_
#define _IBHPASSIVEMODULE_HPP_

#include <shared/module/IBhModule.hpp>

template <typename T>
class IBhPassiveModule
	:public IBhModule<T>
{
public:
	IBhPassiveModule(){}
	virtual ~IBhPassiveModule(){}
public:
	virtual void Stop()
	{
		this->IBhModule::Stop();
		m_recvMsgCond.Lock();
		m_recvMsgCond.Notify();
		m_recvMsgCond.UnLock();
	}
	virtual void OnRecvMessage(T* pMsg)
	{
		m_recvMsgMutex.Lock();
		m_recvMsgList.push_back(pMsg);
		m_recvMsgCond.Notify();
		m_recvMsgMutex.UnLock();
	}
	virtual void OnRecvMessageList(const std::list<T*>& msgList)
	{
		m_recvMsgMutex.Lock();
		std::list<T*>::const_iterator iter = msgList.begin();
		for (; iter != msgList.end(); ++iter)
		{
			if (*iter)
			{
				m_recvMsgList.push_back(*iter);
			}
		}
		m_recvMsgCond.Notify();
		m_recvMsgMutex.UnLock();
	}
protected:
	virtual T* GetRecvMessage()
	{
		T* pMsg = NULL;
		m_recvMsgMutex.Lock();
		while (WantRun() && m_recvMsgList.empty())
		{
			m_recvMsgCond.Wait(m_recvMsgMutex);
		}
		if (!m_recvMsgList.empty())
		{
			pMsg = m_recvMsgList.front();
			m_recvMsgList.pop_front();
		}
		m_recvMsgMutex.UnLock();
		return pMsg;
	}
	virtual void GetRecvMessageList(std::list<T*>& msgList)
	{
		m_recvMsgMutex.Lock();
		while (WantRun() && m_recvMsgList.empty())
		{
			m_recvMsgCond.Wait(m_recvMsgMutex);
		}
		std::list<T*>::iterator iter = m_recvMsgList.begin();
		for (; iter != m_recvMsgList.end();)
		{
			if (*iter)
			{
				msgList.push_back(*iter);
			}
			iter = m_recvMsgList.erase(iter);
		}
		m_recvMsgMutex.UnLock();
	}
protected:
	BhCond m_recvMsgCond;
};
