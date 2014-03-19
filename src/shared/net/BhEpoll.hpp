#ifndef _BHEPOLL_HPP_
#define _BHEPOLL_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

enum class BhEpollEvent{Read, Write};
enum class BhEpollFlag{Level, Edge};

class BhEpoll
{
public:
	BhEpoll();
	~BhEpoll();
public:
	bool Init(int nMaxEvents);
	void Clear();
	bool AddEvent(BhTcpSocket *pSock, BhEpollEvent event = BhEpollEvent::Read, BhEpollFlag flag = BhEpollFlag::Level);
    bool DelEvent(BhTcpSocket *pSock, BhEpollEvent event = BhEpollEvent::Read, BhEopllFlag flag = BhEpollFlag::Level);
    bool AddConnection(BhListenSocket *pSock);
    bool DelConnection(BhListenSocket *pSock);
    bool WaitEvents(int nTimeOut = -1);
protected:
	int m_nEpoll;
	struct epoll_event *m_pEventList;
	int nMaxEvents;
};

#endif
