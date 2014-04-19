#ifndef _IBHACTIVEMODULE_HPP_
#define _IBHACTIVEMODULE_HPP_

#include <shared/module/IBhModule.hpp>

template <typename T>
class IBhActiveModule
{
public:
	IBhActiveModule(){}
	virtual ~IBhActiveModuel(){}
public:
	virtual void OnRecvMesssage(T* pMsg)
	{
		m_recvMutex.Lock();
		m_recvMsgList.push_back(pMsg);
		m_recvMutex.UnLock();
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
		m_recvMsgMutex.UnLock();
	}
protected:
	virtual T* GetRecvMessage()
	{
		T* pMsg = NULL;
		m_recvMsgMutex.Lock();
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
};

#endif
