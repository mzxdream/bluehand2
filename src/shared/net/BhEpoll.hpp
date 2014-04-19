#ifndef _BHEPOLL_HPP_
#define _BHEPOLL_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <shared/net/BhTcpSocket.hpp>
#include <shared/net/BhListenSocket.hpp>
#include <shared/util/BhSignal.hpp>

#define BHEPOLL_IN 0x01
#define BHEPOLL_OUT 0x02
#define BHEPOLL_ET 0x04

class BhEpoll
{
public:
	explicit BhEpoll(int nMaxEvents);
	~BhEpoll();
public:
	int Init();
	void Clear();
	bool AddEvent(BhTcpSocket* pSock, unsigned uEvents);
    bool DelEvent(BhTcpSocket* pSock, unsigned uEvents);
    bool AddConnection(BhListenSocket* pSock);
    bool DelConnection(BhListenSocket* pSock);
    int WaitEvents(int nTimeOut = -1);
    BhSignal& ReadSignal();
    BhSignal& WriteSignal();
    BhSignal& ErrorSignal();
    BhSignal& CloseSignal();
protected:
	int m_nEpoll;
	struct epoll_event* m_pEventList;
	int m_nMaxEvents;
    BhSignal m_sigRead;
    BhSignal m_sigWrite;
    BhSignal m_sigError;
    BhSignal m_sigClose;
};

#endif
