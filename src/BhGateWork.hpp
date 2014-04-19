#ifndef _BHGATEWORK_HPP_
#define _BHGATEWORK_HPP_
#include <shared/module/IBhActiveModule.hpp>
#include <shared/msg/IBhMsg.hpp>
#include <shared/net/BhEpoll.hpp>

class BhGateWork
	:public IBhActiveModule<IBhMsg>
{
public:
	BhGateWork(unsigned uMaxEvents);
	virtual ~BhGateWork();
public:
	virtual int Init();
	virtual void Clear();
	virtual void Run();
	void ListenSocket(BhListenSocket* pSock);
	BhListenSocket* ListenSocket();
	void ListenMutex(BhListenMutex* pMutex);
	BhListenMutex* ListenMutex();
private:
	unsigned m_uMaxEvents;
	BhListenSocket* m_pListenSock;
	BhMutex* m_pListenMutex;
};

#endif //_BHGATEWORK_HPP_
