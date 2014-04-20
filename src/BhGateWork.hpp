#ifndef _BHGATEWORK_HPP_
#define _BHGATEWORK_HPP_
#include <shared/module/IBhActiveModule.hpp>
#include <shared/msg/IBhMsg.hpp>
#include <shared/net/BhEpoll.hpp>
#include <inttype.h>

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
	void ListenMutex(BhListenMutex* pMutex);
	void* OnSockRead(void* pParam);
	void* OnSockWrite(void* pParam);
	void* OnSockError(void* pParam);
	void* OnSockClose(void* pParam);
private:
	BhListenSocket* m_pListenSock;
	BhMutex* m_pListenMutex;
	BhEpoll m_epoll;
	unsigned m_uListenCount;
	int m_nAcceptDisabled;
	uint64_t m_timer;
};

#endif //_BHGATEWORK_HPP_
