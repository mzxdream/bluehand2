#ifndef _IBHMODULE_HPP_
#define _IBHMODULE_HPP_

#include <shared/thread/IBhThread.hpp>
#include <shared/thread/BhMutex.hpp>
#include <shared/thread/BhCond.hpp>

template <typename T>
class IBhModule
    :public IBhThread
{
public:
    IBhModule(){}
    virtual ~IBhModule()
	{
		std::list<T*>::iterator iter = m_sendMsgList.begin();
		for (; iter != m_sendMsgList.end(); )
		{
			delete *iter;
			iter = m_sendMsgList.erase(iter);
		}
		for (iter = m_recvMsgList.begin(); iter != m_recvMsgList.end();)
		{
			delete *iter;
			iter = m_recvMsgList.erase(iter);
		}
	}
public:
    virtual int Init(){return 0;}
    virtual void Clear()
	{
		std::list<T*>::iterator iter = m_sendMsgList.begin();
		for (; iter != m_sendMsgList.end(); )
		{
			delete *iter;
			iter = m_sendMsgList.erase(iter);
		}
		for (iter = m_recvMsgList.begin(); iter != m_recvMsgList.end();)
		{
			delete *iter;
			iter = m_recvMsgList.erase(iter);
		}
	}
	virtual int Start()
	{
		m_bWantRun = true;
		return this->IBhThread::Start();
	}
    virtual void Stop()
	{
		m_bWantRun = false;
	}
	virtual void OnRecvMessage(T* pMsg) = 0;
	virtual void OnRecvMessageList(const std::list<T*>& msgList) = 0;
	T* GetSendMessage()
	{
		T* pMsg = NULL;
		m_sendMsgMutex.Lock();
		if (!m_sendMsgList.empty())
		{
			pMsg = m_sendMsgList.front();
			m_sendMsgList.pop_front();
		}
		m_sendMsgMutex.UnLock();
		return pMsg;
	}
	void GetSendMessageList(std::list<T*>& msgList)
	{
		m_sendMsgMutex.Lock();
		std::list<T*>::iterator iter = m_sendMsgList.begin();
		for (; iter != m_sendMsgList.end();)
		{
			if (*iter)
			{
				msgList.push_back(*iter);
			}
			iter = m_sendMsgList.erase(iter);
		}
		m_sendMsgMutex.UnLock();
	}
protected:
	bool WantRun() const
	{
		return m_bWantRun;
	}
	void SendMessage(T* pMsg)
	{
		if (!pMsg)
		{
			return;
		}
		m_sendMsgMutex.Lock();
		m_sendMsgList.push_back(pMsg);
		m_sendMsgMutex.UnLock();
	}
	void SendMessageList(const std::list<T*>& msgList)
	{
		m_sendMsgMutex.Lock();
		std::list<T*>::const_iterator iter = msgList.begin();
		for (; iter != msgList.end(); ++iter)
		{
			if (*iter)
			{
				m_sendMsgList.push_back(*iter);
			}
		}
		m_sendMsgMutex.UnLock();
	}
	virtual T* GetRecvMessage() = 0;
	virtual void GetRecvMessageList(std::list<T*>& msgList) = 0;
protected:
	std::list<T*> m_recvMsgList;
    BhMutex m_recvMsgMutex;
	std::list<T*> m_sendMsgList;
	BhMutex m_sendMsgMutex;
	volatile bool m_bWantRun;
};

#endif //_IBHMODULE_HPP_
